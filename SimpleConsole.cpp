/*! \file SimpleConsole.cpp
 * \brief implementation of SimpleConsole
 * \author "Melven Zoellner" <melven@topen.org>
 *
*/
#include "SimpleConsole.h"

#include <QScrollBar>
#include <QTextBlock>
#include <QKeyEvent>
#include <QCompleter>
#include <QAbstractItemView>
#include <QStringListModel>
#include <QMenu>
#include <QEventLoop>

#include <QDebug>


SimpleConsole::SimpleConsole(QWidget *parent) :
    QPlainTextEdit(parent)
{
    _prompt  = " > ";
    _prompt2 = "...";
    _inputIndex = 0;

    _historyPosition = 0;

    _completer = NULL;

    _state = UndefinedConsoleState;
    _userInputEventLoop = new QEventLoop(this);

    setUndoRedoEnabled(false);

    // set up font and cursor
    QFont font;
    font.setStyleHint(QFont::Monospace);
    font.setFamily("Monospace");
    font.setPointSize(font.pointSize());
    setFont(font);

    connect(this, SIGNAL(cursorPositionChanged()),
            this, SLOT(updateLastValidCursor()));

    // resize so it is not as small
    resize(500,200);
}


void SimpleConsole::keyPressEvent(QKeyEvent *event)
{
    // don't handle any input if not in user input state
    if( !(_state == ShowCommandPrompt || _state == WaitingForUserInput) )
        return;

    bool handled = true;


    bool completing = (
            state() == ShowCommandPrompt &&
            _completer != NULL &&
            _completer->popup()->isVisible() );


    if( completing )
    {
        // let the completer handle some stuff
        if( event->key() == Qt::Key_Enter   ||
            event->key() == Qt::Key_Return  ||
            event->key() == Qt::Key_Escape  ||
            event->key() == Qt::Key_Tab     ||
            event->key() == Qt::Key_Backtab )
        {
            event->ignore();
            return;
        }


        // check if we need to hide the completer...
        int notCtrlOrShift = ~(Qt::ControlModifier | Qt::ShiftModifier | Qt::NoModifier);
        QString c = event->text();
        bool beginNewWord = !(c.isEmpty() || (c.length() == 1 && ( c[0].isLetterOrNumber() || c == "_" )) );
        if( event->modifiers() & notCtrlOrShift     ||
            beginNewWord                            )
        {
            _completer->popup()->hide();
            completing = false;
        }

    }


    // check if cursor is invalid, then reset it to a valid cursor
    QTextCursor cursor = textCursor();
    if( !modificationAllowed(cursor) )
    {
        setTextCursor(_lastValidCursor);
        setReadOnly(false);
        cursor = textCursor();
    }

    if( (!completing) && event->matches(QKeySequence::InsertLineSeparator) )
    {
        // command over multiple lines
        if( _state == ShowCommandPrompt )
            extendMultilineCommand();
        else // _state == WaitingForUserInput
            _userInputEventLoop->exit();
    }

    else if( (!completing) && event->matches(QKeySequence::InsertParagraphSeparator) )
    {
        // on return emit signal to execute line
        if( _state == ShowCommandPrompt )
            executeCurrentCommand();
        else // _state == WaitingForUserInput
            _userInputEventLoop->exit();
    }

    // look up in history
    else if( event->matches(QKeySequence::MoveToPreviousLine) &&
             _state == ShowCommandPrompt && (!completing) )
        historyUp();

    // look down in history
    else if( event->matches(QKeySequence::MoveToNextLine) &&
             _state == ShowCommandPrompt && (!completing) )
        historyDown();

    // backspace: delete previous character
    else if( event->key() == Qt::Key_Backspace &&
             (event->modifiers() == Qt::NoModifier ||
              event->modifiers() == Qt::ShiftModifier) )
    {
        // create cursor to test if allowed
        if( !cursor.hasSelection() )
            cursor.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor);

        if( modificationAllowed(cursor) )
            QPlainTextEdit::keyPressEvent(event);
    }

    // delete character
    else if( event->matches(QKeySequence::Delete) )
    {
        // create cursor to test if allowed
        if( !cursor.hasSelection() )
            cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor);

        if( modificationAllowed(cursor) )
            QPlainTextEdit::keyPressEvent(event);
    }

    // delete several characters at once
    else if( event->matches(QKeySequence::DeleteEndOfWord) ||
             event->matches(QKeySequence::DeleteEndOfLine) ||
             event->matches(QKeySequence::DeleteStartOfWord) )
    {
        if( event->matches(QKeySequence::DeleteStartOfWord) )
            cursor.movePosition(QTextCursor::StartOfWord, QTextCursor::KeepAnchor);

        if( modificationAllowed(cursor) )
            QPlainTextEdit::keyPressEvent(event);
    }

    // autocomplete
    else if( event->key() == Qt::Key_Tab &&
             event->modifiers() == Qt::NoModifier &&
             _state == ShowCommandPrompt )
    {
        emit autocompletionRequested();
        requestAutocompletion();
    }

    // only send pure text to parent
    else if( event->text().length() == 1 && event->text()[0].isPrint() )
        QPlainTextEdit::keyPressEvent(event);

    else
    {
        // at last check if the user wants some cursor movement
        handled = positionCursor(event);

        // special case for completion:
        // close completer popup if we moved for more than a word
        if( handled && completing )
        {
            bool movedOutsideOfWord = false;
            cursor.movePosition(QTextCursor::StartOfWord);
            if( textCursor() < cursor )
                movedOutsideOfWord = true;
            cursor.movePosition(QTextCursor::EndOfWord);
            if( textCursor() > cursor )
                movedOutsideOfWord = true;

            if( movedOutsideOfWord )
            {
                _completer->popup()->hide();
                completing = false;
            }

        }
    }


    // if we handled the event, scroll there
    if( handled )
        ensureCursorVisible();

    // if still completing, update completer
    if( completing )
        requestAutocompletion();

    // mark the event as handled
    event->setAccepted(handled);
}


void SimpleConsole::contextMenuEvent(QContextMenuEvent *event)
{
    // create a new context menu
    QMenu *menu = new QMenu(this);


    QTextCursor cursor = textCursor();
    if( modificationAllowed(cursor) )
    {
        // cut, copy and paste
        menu->addAction(tr("Cut"), this, SLOT(cut()));
        menu->addAction(tr("Copy"),this, SLOT(copy()));
        menu->addAction(tr("Paste"), this, SLOT(paste()));
    }
    else
    {
        // only copy if in readonly part
        menu->addAction(tr("Copy"),this, SLOT(copy()));
    }

    // show the menu
    menu->exec(event->globalPos());

    // we can delete it now
    delete menu;
}


void SimpleConsole::setCompleter(QCompleter *c)
{
    // disconnect all old connections
    if( _completer )
        disconnect(_completer, 0, this, 0);

    _completer = c;

    // setup the completer
    if( _completer )
    {
        _completer->setWidget(this);
        _completer->setCompletionMode(QCompleter::PopupCompletion);
        connect(_completer, SIGNAL(activated(QString)),
                this,       SLOT(insertCompletion(QString)));
    }
}


void SimpleConsole::insertPrompt(bool newBlock)
{
    if( !setState(ShowCommandPrompt) )
        return;

    if( newBlock )
    {
        // a bit of intelligence here, reuse existing block if it is empty, prevents unncecessary new lines
        if( textCursor().block().text().length() != 0 )
            textCursor().insertBlock();
        textCursor().insertText(_prompt);
        _inputIndex = _prompt.length();
    }
    else
    {
        textCursor().beginEditBlock();
        textCursor().insertHtml("<br>"+_prompt2);
        textCursor().endEditBlock();
        _inputIndex = _prompt2.length();
    }

    ensureCursorVisible();
    _lastValidCursor = textCursor();
}


void SimpleConsole::updateLastValidCursor()
{
    QTextCursor cursor = textCursor();
    if( modificationAllowed(cursor) )
    {
        _lastValidCursor = cursor;
        setReadOnly(false);
    }
    else
    {
        setReadOnly(true);
    }
}


void SimpleConsole::executeCurrentCommand()
{
    if( !setState(ExecutingCommand) )
        return;

    // first move the cursor to the end of the document
    moveCursor(QTextCursor::End);

    // first add current line to the history
    historyAdd(currentLine());

    // extract command
    QString cmd = currentCommand();

    // put output in new block
    textCursor().insertBlock();

    // execute it
    emit execute(cmd);

    // append new prompt
    insertPrompt(true);
}


void SimpleConsole::extendMultilineCommand()
{
    // append line to history
    historyAdd(currentLine());

    // prompt for multiline command
    insertPrompt(false);
}


QString SimpleConsole::currentCommand() const
{
    // compose current command:
    QStringList lines = textCursor().block().text().split(QChar::LineSeparator);

    QString cmd = lines[0].mid(_prompt.length());
    for( int i = 1; i < lines.size(); i++ )
        cmd = cmd + "\n" + lines[i].mid(_prompt2.length());

    return cmd;
}


QString SimpleConsole::currentLine() const
{
    // use a cursor to get the current line
    QTextCursor cursor = textCursor();
    cursor.movePosition(QTextCursor::StartOfLine);
    cursor.movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor);

    // the current line
    QString line = cursor.selectedText().mid(_inputIndex);

    // we still need to remove the prompt
    return line;
}


QString SimpleConsole::waitForUserInput()
{
    // try to change state to user input!
    if( !setState(WaitingForUserInput) )
        return QString();

    // get length of previous text in the current line
    // and set the cursor to the end
    // and force update of lastValidCursor,
    // this indirectly sets readonly to false
    QTextCursor cursor = textCursor();
    cursor.movePosition(QTextCursor::End);
    setTextCursor(cursor);
    updateLastValidCursor();
    cursor.movePosition(QTextCursor::StartOfLine);
    _inputIndex = textCursor().position() - cursor.position();

    // start a new eventloop
    _userInputEventLoop->exec();

    // user input finished, so read current line
    QString userInput = currentLine();

    // set state back to executing
    setState(ExecutingCommand);

    // append new line after input
    htmlOutput("<br>");

    return userInput;
}


void SimpleConsole::setCurrentLine(QString newLine)
{
    // use a cursor to get the current line
    QTextCursor cursor = textCursor();
    cursor.movePosition(QTextCursor::StartOfLine);
    cursor.movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor);

    // save the current line
    QString line = cursor.selectedText();

    // we still need the prompt, so replace the rest
    line = line.left(_inputIndex) + newLine;

    // now replace the text under the cursor
    cursor.insertText(line);

    // update the cursor
    setTextCursor(cursor);
}


void SimpleConsole::historyAdd(QString line)
{
    if( line == "" )
        return;

    if( _history.empty() || _history.last() != line )
        _history.append(line);

    _historyPosition = _history.size();
}


void SimpleConsole::historyUp()
{
    if( _historyPosition > 0 )
        setCurrentLine(_history.at(--_historyPosition));
}


void SimpleConsole::historyDown()
{
    if( _historyPosition+1 < _history.size() )
        setCurrentLine(_history.at(++_historyPosition));
}


bool SimpleConsole::modificationAllowed(const QTextCursor &cursor) const
{
    if( !(_state == ShowCommandPrompt ||
          _state == WaitingForUserInput) )
        return false;

    int posOfLastLine = cursorPositionOfLastLine();

    // now check if we can modify it (it is in the last line?)
    return cursor.position() >=  posOfLastLine &&
           cursor.anchor()   >=  posOfLastLine;
}


bool SimpleConsole::positionCursor(const QKeyEvent *event)
{
    bool handled = true;

    // apply cursor movement
    QTextCursor cursor = textCursor();
    if( event->matches(QKeySequence::MoveToEndOfLine) )
        cursor.movePosition(QTextCursor::EndOfLine);

    else if( event->matches(QKeySequence::MoveToNextWord) )
        cursor.movePosition(QTextCursor::NextWord);

    else if( event->matches(QKeySequence::MoveToNextChar) )
        cursor.movePosition(QTextCursor::NextCharacter);

    else if( event->matches(QKeySequence::SelectEndOfLine) )
        cursor.movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor);

    else if( event->matches(QKeySequence::SelectNextWord) )
        cursor.movePosition(QTextCursor::NextWord, QTextCursor::KeepAnchor);

    else if( event->matches(QKeySequence::SelectNextChar) )
        cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);

    else if( event->matches(QKeySequence::MoveToPreviousWord) )
        cursor.movePosition(QTextCursor::PreviousWord);

    else if( event->matches(QKeySequence::MoveToPreviousChar) )
        cursor.movePosition(QTextCursor::PreviousCharacter);

    else if( event->matches(QKeySequence::SelectPreviousWord) )
        cursor.movePosition(QTextCursor::PreviousWord, QTextCursor::KeepAnchor);

    else if( event->matches(QKeySequence::SelectPreviousChar) )
        cursor.movePosition(QTextCursor::PreviousCharacter, QTextCursor::KeepAnchor);

    // special cases for start of line
    else if( event->matches(QKeySequence::MoveToStartOfLine) )
    {
        cursor.movePosition(QTextCursor::StartOfLine);
        cursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, _prompt.length());
    }

    else if( event->matches(QKeySequence::SelectStartOfLine) )
    {
        cursor.movePosition(QTextCursor::StartOfLine, QTextCursor::KeepAnchor);
        cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, _prompt.length());
    }
    else
    {
        handled = false;
    }


    // check if movement is allowed
    if( modificationAllowed(cursor) )
        setTextCursor(cursor);

    return handled;
}


void SimpleConsole::insertCompletion(QString word)
{
    // replace word under cursor
    QTextCursor cursor = textCursor();
    cursor.select(QTextCursor::WordUnderCursor);
    if( modificationAllowed(cursor) )
    {
        cursor.insertText( word );
    }
}


void SimpleConsole::requestAutocompletion()
{
    // check if we have a completer
    if( !_completer )
        return;

    // select current word
    QTextCursor cursor = textCursor();
    cursor.movePosition(QTextCursor::StartOfWord, QTextCursor::KeepAnchor);

    // popup / update the completer
    if( modificationAllowed(cursor) )
    {
        // update completion prefix
        if( _completer->completionPrefix() != cursor.selectedText() ||
            !_completer->popup()->isVisible()                       )
        {
            _completer->setCompletionPrefix( cursor.selectedText() );
            _completer->popup()->setCurrentIndex(_completer->completionModel()->index(0, 0));
        }
        //qDebug() << "requestAutocompletion with prefix: " << cursor.selectedText();


        // position the _completer popup correctly
        QRect cr = cursorRect(cursor);
        cr.setWidth(_completer->popup()->sizeHintForColumn(0) +
                    _completer->popup()->verticalScrollBar()->sizeHint().width());
        _completer->complete(cr);
    }
    else
    {
        // hide the completer
        _completer->popup()->hide();
    }
}


bool SimpleConsole::setState(SimpleConsole::ConsoleState newState)
{
    // if the new state is the same as before, everything is ok...
    if( newState == _state )
        return true;

    // at first we must show a command prompt
    if( _state == UndefinedConsoleState && newState == ShowCommandPrompt )
    {
        _state = newState;
        return true;
    }

    // if a command prompt is shown, we can execute something
    if( _state == ShowCommandPrompt && newState == ExecutingCommand )
    {
        _state = newState;
        return true;
    }

    // an executed command may need user input
    if( _state == ExecutingCommand && newState == WaitingForUserInput )
    {
        _state = newState;
        return true;
    }

    // after user input, execution continues
    if( _state == WaitingForUserInput && newState == ExecutingCommand )
    {
        _state = newState;
        return true;
    }

    // when the command execution finishes, we show a command prompt
    if( _state == ExecutingCommand && newState == ShowCommandPrompt )
    {
        _state = newState;
        return true;
    }

    // no other transitions possible
    qDebug() << "in SimpleConsole::insertPrompt: cannot switch from ConsoleState " << _state << " to " << newState << "!";
    return false;
}


int SimpleConsole::cursorPositionOfLastLine() const
{
    // get position of last line
    QTextCursor helperCursor = textCursor();
    helperCursor.movePosition(QTextCursor::End);
    helperCursor.movePosition(QTextCursor::StartOfLine);
    if( _state == ShowCommandPrompt || _state == WaitingForUserInput )
        helperCursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, _inputIndex);
    return helperCursor.position();
}


void SimpleConsole::setMaximumHistorySize(int maxSize)
{
    _maxHistSize = maxSize;

    // truncate history if necessary
    if( _history.size() > _maxHistSize )
    {
        QStringList::iterator start = _history.begin();
        QStringList::Iterator end = start + (_history.size() - _maxHistSize);
        _history.erase(start, end);

        _historyPosition = _history.size();
    }
}


void SimpleConsole::setHistory(QStringList newHistory)
{
    _history = newHistory;

    // truncate history if necessary
    setMaximumHistorySize(_maxHistSize);

    _historyPosition = _history.size();
}


void SimpleConsole::output(QString s)
{
    if( _state == ExecutingCommand || _state == UndefinedConsoleState )
        textCursor().insertText(s);
    else
        qDebug() << "Cannot handle console output when not in ExecutingCommand state! Output:\n" << s;
}


void SimpleConsole::htmlOutput(QString s)
{
    if( _state == ExecutingCommand || _state == UndefinedConsoleState )
        textCursor().insertHtml(s);
    else
        qDebug() << "Cannot handle console output when not in ExecutingCommand state! Output:\n" << s;
}
