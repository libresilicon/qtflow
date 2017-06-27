#include "project.h"

#include <QString>
#include <QTextStream>
#include <QDir>

Project::Project()
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
                << "#!/bin/tcsh -f"
                << ""
                << endl;
        project_vars.close();
    }

    QFile qflow_exec(path + "/qflow_exec.sh");
    if (qflow_exec.open(QIODevice::ReadWrite))
    {
        QTextStream stream(&qflow_exec);
        stream
                << "#!/bin/tcsh -f"
                << ""
                << endl;
        qflow_exec.close();
    }

    QFile qflow_vars(path + "/qflow_vars.sh");
    if (qflow_vars.open(QIODevice::ReadWrite))
    {
        QTextStream stream(&qflow_vars);
        stream
                << "#!/bin/tcsh -f"
                << ""
                << endl;
        qflow_vars.close();
    }
}
