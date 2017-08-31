/*! \file PygmentsHighlighter.h
 * \brief defines PygmentsHighlighter
 * \author "Melven Zoellner" <melven@topen.org>
 *
*/

// include guard
#ifndef PYGMENTSHIGHLIGHTER_H
#define PYGMENTSHIGHLIGHTER_H


// Qt-includes
#include <QSyntaxHighlighter>

// PythonQt-includes
#include <PythonQt.h>

// local includes
#include "SimpleConsole.h"


/*! \brief A syntax-highlighter for python using pygments (from python)
 *
 * \warning partly implemented in python, see PygmentsHighlighter.py
 *
*/
class PygmentsHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    explicit PygmentsHighlighter(QTextDocument *parentDoc = 0);     //!< constructor
    
public slots:
    void _setFormat(int start, int count,
                    const QTextCharFormat &format);                 //!< allows to call setFormat from within python

protected:
    void highlightBlock(const QString &text);                       //!< does the actual highlightning, overwritten method from baseclass

private:
    PythonQtObjectPtr _context;                                     //!< own context, so the python-code is not executed in the global context
};

#endif /* PYGMENTSHIGHLIGHTER_H */
