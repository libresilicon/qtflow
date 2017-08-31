/*! \file PygmentsHighlighter.cpp
 * \brief implements PygmentsHighlighter
 * \author "Melven Zoellner" <melven@topen.org>
 *
*/
#include "PygmentsHighlighter.h"

#include <PythonQt.h>


PygmentsHighlighter::PygmentsHighlighter(QTextDocument *parentDoc) :
    QSyntaxHighlighter(parentDoc)
{
    // create an own pythonqt context for the highlighter
    _context = PythonQt::self()->createUniqueModule();

    // name this object
    setObjectName("pygmentsHighlighter");

    // make it accessable in pythonqt,
    // this allows to delegate the work in highlightBlock
    // to a python function
    _context.addObject(objectName(),this);

    // initialize python
    _context.evalFile(":/lib/PygmentsHighlighter.py");
}


// just delegate the protected method setFormat from base class
void PygmentsHighlighter::_setFormat(int start, int count, const QTextCharFormat &format)
{
    //qDebug() << "setFormat " << start << " " << count << " format " << format;
    setFormat(start, count, format);
}


void PygmentsHighlighter::highlightBlock(const QString &text)
{
    //qDebug() << "highlightBlock" << text;
    // call python function to do everything
    QVariantList args;
    args.append(text);
    _context.call("highlightCode", args);
}
