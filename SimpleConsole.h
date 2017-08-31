/*! \file SimpleConsole.h
 * \brief defines SimpleConsole
 * \author "Melven Zoellner" <melven@topen.org>
 *
*/

// include-guard
#ifndef SIMPLECONSOLE_H
#define SIMPLECONSOLE_H


// Qt-includes
#include <QPlainTextEdit>

// Qt-forward declarations
QT_BEGIN_NAMESPACE
class QCompleter;
class QEventLoop;
QT_END_NAMESPACE

/*! \brief a base class for console widgets (e.g. command prompts)
 * Modifies the behaviour of a QPlainTextEdit to make it suitable as a lightweight console
 *
*/
class SimpleConsole : public QPlainTextEdit
{
    Q_OBJECT
    Q_PROPERTY(QString prompt READ getPrompt WRITE setPrompt)
    Q_PROPERTY(QString prompt2 READ getPrompt2 WRITE setPrompt2)
    Q_PROPERTY(int maximumHistorySize READ getMaximumHistorySize WRITE setMaximumHistorySize)
    Q_PROPERTY(QStringList history READ getHistory WRITE setHistory)

public slots:
    void output(QString s);                                 //!< print output in console
    void htmlOutput(QString s);                             //!< print html output in console

public:
    explicit SimpleConsole(QWidget *parent = 0);            //!< the constructor

    void keyPressEvent(QKeyEvent *e);                       //!< overwritten event-handler from base class
    void contextMenuEvent(QContextMenuEvent *e);            //!< overwritten event-handler from base class

    void setCompleter(QCompleter* c);                       //!< set QCompleter for autocompletion
    const QCompleter *completer() const {return _completer;}//!< getQCompleter for autocompletion

    QString currentCommand() const;                         //!< helper function: get current command (block) string
    QString currentLine() const;                            //!< helper function: get last line of the current command (block)

    /*! prompts the user for input during execution of a command
     * \warning starts a new eventloop and returns only when the user presses return
    */
    QString waitForUserInput();

    //! the state of the console
    enum ConsoleState
    {
        UndefinedConsoleState = 0,
        ShowCommandPrompt,
        ExecutingCommand,
        WaitingForUserInput
    };
    ConsoleState state() const {return _state;}             //!< return current console state


    QString getPrompt() const {return _prompt;}             //!< getter for _prompt
    void setPrompt(QString p) {_prompt = p;}                //!< setter for _prompt
    QString getPrompt2() const {return _prompt2;}           //!< getter for _prompt2
    void setPrompt2(QString p2) {_prompt2 = p2;}            //!< setter for _prompt2
    int getMaximumHistorySize() const {return _maxHistSize;}//!< getter for _maxHistSize
    void setMaximumHistorySize(int maxSize);                //!< setter for _maxHistSize
    QStringList getHistory() const {return _history;}       //!< getter for _history
    void setHistory(QStringList newHistory);                //!< setter for _history

signals:
    void execute(QString command);                          //!< emitted on enter
    void autocompletionRequested();                         //!< emitted before the autocompletion should popup

protected:

protected slots:
    void insertPrompt(bool newBlock);                       //!< helper function: starts a new command prompt
    void updateLastValidCursor();                           //!< helper function: updates _lastValidCursor
    void insertCompletion(QString word);                    //!< helper function: replaces current word

private:
    QString _prompt;                                        //!< string at the beginning of the line (_prompt and _prompt2 must have the same length!)
    QString _prompt2;                                       //!< string at the beginning of the line for multi-line commands
    int _inputIndex;                                        //!< index in last line where the input begins (used in currentLine())
    int _maxHistSize;                                       //!< maximal number of entries in the history
    QStringList _history;                                   //!< list of previous commands
    int _historyPosition;                                   //!< current position in history
    QTextCursor _lastValidCursor;                           //!< saves the last cursor to go back to it
    QCompleter *_completer;                                 //!< for autocompletion
    ConsoleState _state;                                    //!< current state of the console
    QEventLoop *_userInputEventLoop;                        //!< event loop for user input


    void executeCurrentCommand();                           //!< helper function: emits execute
    void extendMultilineCommand();                          //!< helper function: continues current command in the next line

    void setCurrentLine(QString line);                      //!< helper function: replace last line of the current command (block)
    void historyAdd(QString line);                          //!< helper function: add string to the history
    void historyUp();                                       //!< helper function: move up in history
    void historyDown();                                     //!< helper function: move down in history

    bool modificationAllowed(const QTextCursor &c) const;   //!< helper function: checks if modification of selected text is allowed
    bool positionCursor(const QKeyEvent *e);                //!< helper function: handle cursor movements in current command
    int cursorPositionOfLastLine() const;                   //!< helper function: get position of beginning of the last line

    void requestAutocompletion();                           //!< helper function: popups QCompleter
    bool setState(ConsoleState newState);                   //!< helper function: tries to change state of the console
};

#endif /* SIMPLECONSOLE_H */
