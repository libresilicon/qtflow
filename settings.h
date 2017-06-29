#ifndef SETTINGS_H
#define SETTINGS_H

#include "common.h"
#include <QMap>

class ISettings
{
public:
    ISettings() {}
    virtual ~ISettings() {}
    virtual QString value(QString) {}
};

class QtFlowSettings : public ISettings
{
public:
    QtFlowSettings();
    ~QtFlowSettings();
    QString value(QString);
private:
    map_string_t vars;
};

class ProjectSettings : public ISettings
{
public:
    ProjectSettings(QString);
    ~ProjectSettings();
    QString value(QString);
private:
    map_string_t vars;
};
#endif // SETTINGS_H
