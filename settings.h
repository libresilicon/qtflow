#ifndef SETTINGS_H
#define SETTINGS_H

#include "common.h"
#include <QMap>
#include <QVector>

class ISettings
{
public:
    ISettings() {}
    virtual ~ISettings() {}
    virtual QString value(QString) { return ""; }
    virtual void set(QString, QString) {}
    virtual table_string_t table() { return table_string_t(); }
    virtual void swap(map_string_t) {}
    virtual void save() {}
};

class QtFlowSettings : public ISettings
{
public:
    QtFlowSettings();
    ~QtFlowSettings();
    QString value(QString);
    void set(QString, QString);
    table_string_t table();
    void swap(map_string_t);
    void save();
private:
    map_string_t vars;
};

class ProjectSettings : public ISettings
{
public:
    ProjectSettings(QString);
    ~ProjectSettings();
    QString value(QString);
    void set(QString, QString);
    table_string_t table();
    void swap(map_string_t);
    void save();
private:
    QString cwd;
    map_string_t vars;
};

class QflowSettings : public ISettings
{
public:
    QflowSettings(QString);
    ~QflowSettings();
    QString value(QString);
    void set(QString, QString);
    table_string_t table();
    void swap(map_string_t);
    void save();
private:
    QString cwd;
    map_string_t vars;
};

#endif // SETTINGS_H
