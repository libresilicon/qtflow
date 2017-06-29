#include "settings.h"
#include "settingsparser.h"

#include <QDir>
#include <QFile>
#include <QTextStream>

QtFlowSettings::QtFlowSettings() :
    ISettings()
{
    QString home = QDir::homePath();
    QFile rc(home + "/.qtflowrc");
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


ProjectSettings::ProjectSettings(QString path) :
    ISettings()
{
    QFile rc(path + "/project_vars.sh");
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
