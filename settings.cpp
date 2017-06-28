#include "settings.h"
#include "settingsparser.h"

#include <QDir>
#include <QFile>
#include <QTextStream>

Settings::Settings()
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

QString Settings::setting(QString k)
{
    return vars.value(k);
}

QString Settings::qflowprefix()
{
    return setting("qflowprefix");
}
