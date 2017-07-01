
#include "constants.h"
#include "project.h"

#include <QString>
#include <QTextStream>
#include <QDir>
#include <QProcess>
#include <QProcess>

Project::Project() :
    IProject(),
    settings(new QtFlowSettings)
{

}

Project::~Project()
{
    delete settings;
}

void Project::createQflow(QString path)
{
    QDir dir(path);
    dir.mkdir("source");
    dir.mkdir("synthesis");
    dir.mkdir("layout");

    QString qflowprefix = settings->value("qflowprefix");

    QFileDevice::Permissions executable
            = QFileDevice::ReadOwner | QFileDevice::WriteOwner | QFileDevice::ExeOwner
            | QFileDevice::ReadGroup | QFileDevice::WriteGroup | QFileDevice::ExeGroup
            | QFileDevice::ReadOther
            ;

    QFile index(path + "/source/index.v");
    if (index.open(QIODevice::ReadWrite))
    {
        QTextStream stream(&index);
        stream
                << endl
                << "module index();" << endl
                << "endmodule" << endl
                << endl;
        index.close();
    }

    QFile project_vars(path + PROJECT_VARS);
    if (project_vars.open(QIODevice::ReadWrite))
    {
        QTextStream stream(&project_vars);
        stream
                << "#!/bin/tcsh -f" << endl
                << "#-------------------------------------------" << endl
                << "# project variables for project " << path << endl
                << "#-------------------------------------------" << endl
                << endl
                << "set build=synthesize" << endl
                << endl
                << endl;
        project_vars.close();
        project_vars.setPermissions(executable);
    }

    QFile qflow_exec(path + QFLOW_EXEC);
    if (qflow_exec.open(QIODevice::ReadWrite))
    {
        QTextStream stream(&qflow_exec);
        stream
                << "#!/bin/tcsh -f" << endl
                << "#-------------------------------------------" << endl
                << "# qflow exec script for project " << path << endl
                << "#-------------------------------------------" << endl
                << endl
                << qflowprefix << "/scripts/synthesize.sh " << path << " index || exit 1" << endl
                << "# " << qflowprefix << "/scripts/placement.sh -d " << path << " index || exit 1" << endl
                << "# " << qflowprefix << "/scripts/vesta.sh " << path << " index || exit 1" << endl
                << "# " << qflowprefix << "/scripts/router.sh " << path << " index || exit 1" << endl
                << "# " << qflowprefix << "/scripts/placement.sh -f -d " << path << " index || exit 1" << endl
                << "# " << qflowprefix << "/scripts/router.sh " << path << " index || exit 1 $status" << endl
                << "# " << qflowprefix << "/scripts/cleanup.sh " << path << " index || exit 1" << endl
                << "# " << qflowprefix << "/scripts/display.sh " << path << " index || exit 1" << endl
                << endl;
        qflow_exec.close();
        qflow_exec.setPermissions(executable);
    }

    QFile qflow_vars(path + QFLOW_VARS);
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
                << "set techdir=" << qflowprefix << "/tech/osu035" << endl
                << "set sourcedir=" << path << "/source"    << endl
                << "set synthdir="  << path << "/synthesis" << endl
                << "set layoutdir=" << path << "/layout"    << endl
                << "set techname=osu035" << endl
                << "set scriptdir=" << qflowprefix << "/scripts" << endl
                << "set bindir=" << qflowprefix << "/bin" << endl
                << "set synthlog=" << path << "/synth.log" << endl
                << "#-------------------------------------------" << endl
                << endl
                << endl;
        qflow_vars.close();
        qflow_vars.setPermissions(executable);
    }
}

void Project::executeQflow(QProcess *exec)
{
    exec->start(QFLOW_EXEC);
}
