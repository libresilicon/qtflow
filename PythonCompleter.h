/*! \file PythonCompleter.h
 * \brief defined PythonCompleter
 * \author "Melven Zoellner" <melven@topen.org>
 *
*/

// include
#ifndef PYTHONCOMPLETER_H
#define PYTHONCOMPLETER_H

// Qt-includes
#include <QCompleter>

// PythonQt-includes
#include <PythonQt.h>

// forward declarations
class SimpleConsole;


//! a completer for python, using pygments in the background
class PythonCompleter : public QCompleter
{
    Q_OBJECT
public:
    explicit PythonCompleter(SimpleConsole *parent,
                               PythonQtObjectPtr context);

protected slots:
    void updateCompletions();                                   //!< dynamically updates possible completion strings
    void popupAboutToShow();                                    //!< sets up dynamic updates of possible completions
    void popupAboutToHide();                                    //!< stops dynamic updates of possible completions

protected:
    PythonQtObjectPtr _context;                                 //!< python context to run the completer
    PythonQtObjectPtr _parentContext;                           //!< python context to complete

};

#endif /* PYTHONCOMPLETER_H */
