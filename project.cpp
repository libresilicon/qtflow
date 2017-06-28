#include "project.h"

#include <QString>
#include <QTextStream>
#include <QDir>

Project::Project() :
    settings(new Settings)
{

}

void Project::createQflow(QString path)
{
    QDir dir(path);
    dir.mkdir("source");
    dir.mkdir("synthesis");
    dir.mkdir("layout");


    QFile project_vars(path + "/project_vars.sh");
    if (project_vars.open(QIODevice::ReadWrite))
    {
        QTextStream stream(&project_vars);
        stream
                << "#!/bin/tcsh -f" << endl
                << "#-------------------------------------------" << endl
                << "# project variables for project " << path << endl
                << "#-------------------------------------------" << endl
                << endl
                << endl;
        project_vars.close();
    }

    QFile qflow_exec(path + "/qflow_exec.sh");
    if (qflow_exec.open(QIODevice::ReadWrite))
    {
        QTextStream stream(&qflow_exec);
        stream
                << "#!/bin/tcsh -f" << endl
                << "#-------------------------------------------" << endl
                << "# qflow exec script for project " << path << endl
                << "#-------------------------------------------" << endl
                << endl
                << endl;
        qflow_exec.close();
    }

    QFile qflow_vars(path + "/qflow_vars.sh");
    if (qflow_vars.open(QIODevice::ReadWrite))
    {
        QTextStream stream(&qflow_vars);
        stream
                << "#!/bin/tcsh -f" << endl
                << "#-------------------------------------------" << endl
                << "# qflow variables for project " << path << endl
                << "#-------------------------------------------" << endl
                << endl
                << "set projecpath=" << path << endl
                << "set techdir=" << settings->qflowprefix() << "/tech/osu035" << endl
                << "set sourcedir=" << path << "/source"    << endl
                << "set synthdir="  << path << "/synthesis" << endl
                << "set layoutdir=" << path << "/layout"    << endl
                << "set techname=osu035" << endl
                << "set scriptdir=" << settings->qflowprefix() << "/scripts" << endl
                << "set bindir=" << settings->qflowprefix() << "/bin" << endl
                << "set synthlog=" << path << "/synth.log" << endl
                << "#-------------------------------------------" << endl
                << endl
                << endl;
        qflow_vars.close();
    }
}
