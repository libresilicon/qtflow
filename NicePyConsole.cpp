/*! \file NicePyConsole.cpp
 * \brief implements NicePyConsole
 * \author "Melven Zoellner" <melven@topen.org>
 *
*/
#include "NicePyConsole.h"

#include "PygmentsHighlighter.h"
#include "PythonCompleter.h"

#include <QDebug>


// anonymous namespace to wrap the pythonqt input callback
namespace
{
QString pythonQtInputCallback(void* that)
{
    return ((NicePyConsole*)that)->waitForUserInput();
}
}

NicePyConsole::NicePyConsole(QWidget *parent, const PythonQtObjectPtr &context) :
    SimpleConsole(parent)
{
    _context = context;

    // connect output
    connect(PythonQt::self(),   SIGNAL(pythonStdOut(QString)),
            this,               SLOT(output(QString)));
    connect(PythonQt::self(),   SIGNAL(pythonStdErr(QString)),
            this,               SLOT(output(QString)));


    // connect command execution
    connect(this,               SIGNAL(execute(QString)),
            this,               SLOT(executePythonCommand(QString)));

    // setup input callback
    // ugly hack?
    PythonQtInputChangedCB *callback = pythonQtInputCallback;
    PythonQt::self()->setRedirectStdInCallback(callback, this);
    PythonQt::self()->setRedirectStdInCallbackEnabled(true);

    // create the highlighter
    _highlighter = new PygmentsHighlighter(this->document());

    // create the completer
    _completer = new PythonCompleter(this, _context);
    setCompleter(_completer);

    // insert first prompt:
    insertPrompt(true);
}


NicePyConsole::~NicePyConsole()
{
    // remove input callback
    PythonQt::self()->setRedirectStdInCallbackEnabled(true);
    PythonQt::self()->setRedirectStdInCallback(NULL);
}


int NicePyConsole::executePythonCommand(QString cmd)
{
    // skip empty strings
    if( cmd == "" )
        return 0;

    qDebug() << "Run python code: " << cmd << "\n";
    PythonQtObjectPtr p;
    PyObject* dict = NULL;
    if (PyModule_Check(_context)) {
        dict = PyModule_GetDict(_context);
    } else if (PyDict_Check(_context)) {
        dict = _context;
    }
    if (dict) {
        p.setNewRef(PyRun_String(cmd.toLatin1().data(), Py_single_input, dict, dict));
    }

    if (!p) {
        PythonQt::self()->handleError();
    }

    return 0;
}
