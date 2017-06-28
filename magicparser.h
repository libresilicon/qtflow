#ifndef MAGICPARSER_H
#define MAGICPARSER_H

#include <QVector>

#include "common.h"
#include "lexmagic.h"

class MAGICBufferGuard
{
    YY_BUFFER_STATE* mState;
public:
    MAGICBufferGuard(YY_BUFFER_STATE *state);
    ~MAGICBufferGuard();
};

class MagicParser
{
    rects_t rectangles;
public:
    MagicParser(QByteArray data);
    rects_t& getRectangles();
};

#endif // MAGICPARSER_H
