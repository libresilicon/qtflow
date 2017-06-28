#ifndef SETTINGSPARSER_H
#define SETTINGSPARSER_H

#include <QMap>

#include "common.h"
#include "lexsettings.h"

class SETTINGSBufferGuard
{
    YY_BUFFER_STATE* mState;
public:
    SETTINGSBufferGuard(YY_BUFFER_STATE *state);
    ~SETTINGSBufferGuard();
};

class SettingsParser
{
    map_string_t variables;
public:
    SettingsParser(QByteArray data);
    map_string_t& getVariables();
};

#endif // SETTINGSPARSER_H
