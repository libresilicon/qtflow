
#include "constants.h"
#include "settings.h"
#include "settingsparser.h"

#include <QDir>
#include <QFile>
#include <QTextStream>

QtFlowSettings::QtFlowSettings() :
    ISettings()
{
    QString home = QDir::homePath();
    QFile rc(home + QTFLOWRC);
    if (rc.exists())
    {
        rc.open(QIODevice::ReadOnly);
        QByteArray content(rc.readAll());

        SettingsParser settings(content);
        vars = settings.getVariables();

    }
    else
    {
        vars.insert("qflowprefix", "/usr/local/share/qflow");
        rc.open(QIODevice::ReadWrite);
        QTextStream stream(&rc);
        stream
                << "# qtflowrc" << endl
                << "set qflowprefix=" << vars.value("qflowprefix") << endl
                << endl;
    }
    rc.close();
}

QtFlowSettings::~QtFlowSettings()
{
}

QString QtFlowSettings::value(QString k)
{
    return vars.value(k);
}

void QtFlowSettings::set(QString k, QString v)
{
    vars.insert(k, v);
}

void QtFlowSettings::save()
{
    QString home = QDir::homePath();
    QFile rc(home + QTFLOWRC);
    if (rc.open(QIODevice::ReadWrite | QIODevice::Truncate))
    {
        QTextStream stream(&rc);
        for (auto begin = vars.begin(), end = vars.end(); begin != end; ++begin)
            stream << "set " << begin.key() << "=" << begin.value() << endl;
        rc.close();
    }
}


ProjectSettings::ProjectSettings(QString path) :
    ISettings(),
    cwd(path)
{
    QFile rc(cwd + PROJECT_VARS);
    if (rc.exists())
    {
        rc.open(QIODevice::ReadOnly);
        QByteArray content(rc.readAll());

        SettingsParser settings(content);
        vars = settings.getVariables();
    }
    else
    {

    }
    rc.close();
}

ProjectSettings::~ProjectSettings()
{
}

QString ProjectSettings::value(QString k)
{
    return vars.value(k);
}

void ProjectSettings::set(QString k, QString v)
{
    vars.insert(k, v);
}

void ProjectSettings::save()
{
    QFile project_vars(cwd + PROJECT_VARS);
    if (project_vars.open(QIODevice::ReadWrite | QIODevice::Truncate))
    {
        QTextStream stream(&project_vars);
        stream << "#!/bin/tcsh -f" << endl;
        for (auto begin = vars.begin(), end = vars.end(); begin != end; ++begin)
            stream << "set " << begin.key() << "=" << begin.value() << endl;
        project_vars.close();
    }
}
