#include "settingsparser.h"

#include <QDebug>
#include <QFileDialog>

#include "yysettings.h"
#include "lexsettings.h"

extern map_string_t parsedVariables;

SETTINGSBufferGuard::SETTINGSBufferGuard(YY_BUFFER_STATE* state) :
    mState(state)
{
}

SETTINGSBufferGuard::~SETTINGSBufferGuard() {
    yy_delete_buffer(*mState);
}


SettingsParser::SettingsParser(QByteArray data) :
    variables()
{
    YY_BUFFER_STATE buffer = yy_scan_string(data.constData());
    SETTINGSBufferGuard guard(&buffer);
    Q_UNUSED(guard)

    int retcode;
    try {
        retcode = settingsparse();
    } catch(ParserException& e) {
        qWarning() << QString("Parse error near line %1: %2").arg(e.lineNumber).arg(e.what).toUtf8().constData();
        retcode = 1;
    }

    if (retcode == 0) {
        qDebug() << "Successfully parsed" << parsedVariables.count() << "variables";
        variables.swap(parsedVariables);
    }
}

map_string_t& SettingsParser::getVariables()
{
    return variables;
}
