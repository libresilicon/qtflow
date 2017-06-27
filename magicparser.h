#ifndef MAGICPARSER_H
#define MAGICPARSER_H

#include <QVector>

#include "common.h"
#include "lexmagic.h"

class YYBufferGuard
{
    YY_BUFFER_STATE* mState;
public:
    YYBufferGuard(YY_BUFFER_STATE *state);
    ~YYBufferGuard();
};

class MagicParser
{
    rects_t rectangles;
public:
    MagicParser(QByteArray data);
    rects_t& getRectangles();
};

#endif // MAGICPARSER_H
