
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
        if (!vars.contains(DEFAULT_VERILOG))
            set(DEFAULT_VERILOG, DEFAULT_VERILOG);
    }
    else
    {
		set("qflowprefix", "/usr/share/qflow");
        set(DEFAULT_VERILOG, DEFAULT_VERILOG);
        set("terminal", "urxvt -e");
        save();
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

table_string_t QtFlowSettings::table()
{
   table_string_t list;
   for (auto begin = vars.begin(), end = vars.end(); begin != end; ++begin)
        list.append(QPair<QString, QString>(begin.key(), begin.value()));
   return list;
}

void QtFlowSettings::swap(map_string_t re)
{
    vars.swap(re);
}

void QtFlowSettings::save()
{
    QString home = QDir::homePath();
    QFile rc(home + QTFLOWRC);
    if (rc.open(QIODevice::ReadWrite | QIODevice::Truncate))
    {
        QTextStream stream(&rc);
        stream << TCSH_SHEBANG << endl << "# qtflowrc" << endl;
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
        rc.close();
    }
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

table_string_t ProjectSettings::table()
{
   table_string_t list;
   for (auto begin = vars.begin(), end = vars.end(); begin != end; ++begin)
        list.append(QPair<QString, QString>(begin.key(), begin.value()));
   return list;
}

void ProjectSettings::swap(map_string_t re)
{
    vars.swap(re);
}

void ProjectSettings::save()
{
    QFile project_vars(cwd + PROJECT_VARS);
    if (project_vars.open(QIODevice::ReadWrite | QIODevice::Truncate))
    {
        QTextStream stream(&project_vars);
        stream << TCSH_SHEBANG << endl;
        for (auto begin = vars.begin(), end = vars.end(); begin != end; ++begin)
            stream << "set " << begin.key() << "=" << begin.value() << endl;
        project_vars.close();
    }
}


QflowSettings::QflowSettings(QString path) :
    ISettings(),
    cwd(path)
{
    QFile rc(cwd + QFLOW_VARS);
    if (rc.exists())
    {
        rc.open(QIODevice::ReadOnly);
        QByteArray content(rc.readAll());
        SettingsParser settings(content);
        vars = settings.getVariables();
        rc.close();
    }
    if (!vars.contains(DEFAULT_VERILOG))
        set(DEFAULT_VERILOG, DEFAULT_VERILOG);
}

QflowSettings::~QflowSettings()
{
}

QString QflowSettings::value(QString k)
{
    return vars.value(k);
}

void QflowSettings::set(QString k, QString v)
{
    vars.insert(k, v);
}

table_string_t QflowSettings::table()
{
   table_string_t list;
   for (auto begin = vars.begin(), end = vars.end(); begin != end; ++begin)
       list.append(QPair<QString, QString>(begin.key(), begin.value()));
   return list;
}

void QflowSettings::swap(map_string_t re)
{
    vars.swap(re);
}

void QflowSettings::save()
{
    QFile qflow_vars(cwd + QFLOW_VARS);
    if (qflow_vars.open(QIODevice::ReadWrite | QIODevice::Truncate))
    {
        QTextStream stream(&qflow_vars);
        stream << TCSH_SHEBANG << endl;
        for (auto begin = vars.begin(), end = vars.end(); begin != end; ++begin)
            stream << "set " << begin.key() << "=" << begin.value() << endl;
        qflow_vars.close();
    }
}
