#ifndef SETTINGS_H
#define SETTINGS_H

#include "common.h"
#include <QMap>

class Settings
{
public:
    Settings();
    QString setting(QString k);

    QString qflowprefix();
private:
    map_string_t vars;
};

#endif // SETTINGS_H
