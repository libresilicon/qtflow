#include "PythonCompleter.h"

#include <QStringListModel>
#include "PythonCompleterPopup.h"
#include "SimpleConsole.h"


PythonCompleter::PythonCompleter(SimpleConsole *parent, PythonQtObjectPtr context) :
    QCompleter(parent)
{
    // use given pythonqt-context
    _context = PythonQt::self()->createUniqueModule();
    _parentContext = context;

    // initialize python
    _context.evalFile(":/lib/PythonCompleter.py");

    // generate string list model
    // (which we will change dynamically later)
    setModel( new QStringListModel(this) );

    // generate the popup
    // (usually this is done automatically, but
    //  we have some small modifications)
    PythonCompleterPopup *pythonCompleterPopup = new PythonCompleterPopup;
    setPopup( pythonCompleterPopup );

    // notify me when the popup is shown/hidden...
    connect(pythonCompleterPopup, SIGNAL(aboutToShow()),
            this,                 SLOT(popupAboutToShow()));
    connect(pythonCompleterPopup, SIGNAL(aboutToHide()),
            this,                 SLOT(popupAboutToHide()));

    // and finally update once when the autocompletion is started
    connect(parent, SIGNAL(autocompletionRequested()),
            this,   SLOT(updateCompletions()));
}


void PythonCompleter::updateCompletions()
{
    //qDebug() << "updateCompletions called";
    SimpleConsole* textEditParent = qobject_cast<SimpleConsole*>(parent());


    // get the code and truncate it to the cursor position
    QString codeBlock = textEditParent->currentCommand();

    QTextCursor cursor = textEditParent->textCursor();
    cursor.movePosition(QTextCursor::End);
    int cursorPosFromRight = cursor.position() - textEditParent->textCursor().position();

    codeBlock.truncate( codeBlock.length() - cursorPosFromRight );


    // as a precaution disconnect connection (endless loop on error!)
    bool wasConnected =
        disconnect(textEditParent, SIGNAL(cursorPositionChanged()),
                   this,   SLOT(updateCompletions()));

    // call python function to determine list of possible completions
    QVariantList args;
    args.append(codeBlock);
    args.append(QVariant::fromValue(_parentContext));
    QVariant completions  = _context.call("autocompleteCode", args);

    if( wasConnected )
        connect(textEditParent, SIGNAL(cursorPositionChanged()),
                this,   SLOT(updateCompletions()));

    // set possible completions
    QStringListModel* listModel = qobject_cast<QStringListModel*>( model() );
    //qDebug() << "PythonCompleter: code = " << codeBlock << "\n completions = " << completions.toStringList() << "\n";
    listModel->setStringList( completions.toStringList() );
}


void PythonCompleter::popupAboutToShow()
{
    //qDebug() << "popup about to show";
    SimpleConsole* textEditParent = qobject_cast<SimpleConsole*>(parent());

    // update completions when the cursor changes
    connect(textEditParent, SIGNAL(cursorPositionChanged()),
            this,           SLOT(updateCompletions()));
}

void PythonCompleter::popupAboutToHide()
{
    //qDebug() << "popup about to hide";
    SimpleConsole* textEditParent = qobject_cast<SimpleConsole*>(parent());

    // stop updating completions
    disconnect(textEditParent, SIGNAL(cursorPositionChanged()),
               this,           SLOT(updateCompletions()));
}
