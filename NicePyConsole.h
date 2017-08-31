/*! \file NicePyConsole.h
 * \brief defines NicePyConsole
 * \author "Melven Zoellner" <melven@topen.org>
 *
*/

// include guard
#ifndef NICEPYCONSOLE_H
#define NICEPYCONSOLE_H


// PythonQt-includes
#include <PythonQt.h>

// local includes
#include "SimpleConsole.h"

// forward declarations
class PygmentsHighlighter;
class PythonCompleter;


//! a nice console (e.g. terminal) for python, with history, autocompletion and syntax-highlightning
class NicePyConsole : public SimpleConsole
{
    Q_OBJECT
public:
    explicit NicePyConsole(QWidget *parent,
                           const PythonQtObjectPtr& context);       //!< constructor
    ~NicePyConsole();                                               //!< destructor
    
    PythonQtObjectPtr context() const {return _context;}            //!< get the python context of this console

protected slots:
    int executePythonCommand(QString cmd);                          //!< slot to run python code

protected:
    PythonQtObjectPtr _context;                                     //!< the python context of this console
    PygmentsHighlighter *_highlighter;                              //!< for syntax highlightning
    PythonCompleter *_completer;                                  //!< for autocompletion
    
};

#endif /* NICEPYCONSOLE_H */
