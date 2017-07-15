#include "vcdparser.h"

#include <QDebug>
#include <QFileDialog>

#include "yyvcd.h"
#include "lexvcd.h"

extern vcd_t parsedVcd;

VCDBufferGuard::VCDBufferGuard(YY_BUFFER_STATE* state) :
    mState(state)
{
}

VCDBufferGuard::~VCDBufferGuard() {
    vcd_delete_buffer(*mState);
}


VcdParser::VcdParser(QByteArray data)
    : vcd()
{
    YY_BUFFER_STATE buffer = vcd_scan_string(data.constData());
    VCDBufferGuard guard(&buffer);
    Q_UNUSED(guard)

    int retcode;
    try
    {
        retcode = vcdparse();
    }
    catch(ParserException& e)
    {
        qWarning() << QString("Parse error near line %1: %2").arg(e.lineNumber).arg(e.what).toUtf8().constData();
        retcode = 1;
    }

    if (retcode == 0) {
        vcd = parsedVcd;
        qDebug()
                << "VCD PARSED" << endl
                << "Date:" << vcd.date << endl
                << "Version:" << vcd.version << endl
                << "Timescale:" << vcd.timescale << endl
                << "Distinct signals:" << vcd.changes.count() << endl;
    }
}

vcd_t& VcdParser::getVcd()
{
    return vcd;
}
