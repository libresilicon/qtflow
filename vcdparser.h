#ifndef VCDPARSER_H
#define VCDPARSER_H

#include <QMap>

#include "common.h"
#include "vcd.ll.h"

class VCDBufferGuard
{
    YY_BUFFER_STATE* mState;
public:
    VCDBufferGuard(YY_BUFFER_STATE *state);
    ~VCDBufferGuard();
};

class VcdParser
{
    vcd_t vcd;
public:
    VcdParser(QByteArray data);
    vcd_t& getVcd();
};

#endif // VCDPARSER_H
