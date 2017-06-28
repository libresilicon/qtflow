#include "magicparser.h"

#include <QDebug>
#include <QFileDialog>

#include "yymagic.h"
#include "lexmagic.h"

extern rects_t parsedElements;

MAGICBufferGuard::MAGICBufferGuard(YY_BUFFER_STATE* state) :
    mState(state)
{
}

MAGICBufferGuard::~MAGICBufferGuard() {
    magic_delete_buffer(*mState);
}


MagicParser::MagicParser(QByteArray data) :
    rectangles()
{
    YY_BUFFER_STATE buffer = magic_scan_string(data.constData());
    MAGICBufferGuard guard(&buffer);
    Q_UNUSED(guard)

    int retcode;
    try {
        retcode = magicparse();
    } catch(ParserException& e) {
        qWarning() << QString("Parse error near line %1: %2").arg(e.lineNumber).arg(e.what).toUtf8().constData();
        retcode = 1;
    }

    if (retcode == 0) {
        qDebug() << "Successfully parsed" << parsedElements.count() << "elements";
        rectangles.swap(parsedElements);
    }
}

rects_t& MagicParser::getRectangles()
{
    return rectangles;
}
