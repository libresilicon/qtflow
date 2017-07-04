
#include "constants.h"
#include "project.h"

#include <QString>
#include <QTextStream>
#include <QDir>
#include <QProcess>

Qflow::Qflow() :
    IProject(),
    settings(new QtFlowSettings)
{
    executable
        = QFileDevice::ReadOwner | QFileDevice::WriteOwner | QFileDevice::ExeOwner
        | QFileDevice::ReadGroup | QFileDevice::WriteGroup | QFileDevice::ExeGroup
        | QFileDevice::ReadOther
        ;
}

Qflow::~Qflow()
{
    delete settings;
}

void Qflow::create(QString path)
{
    QDir dir(path);
    dir.mkdir("source");
    dir.mkdir("synthesis");
    dir.mkdir("layout");

    QString qflowprefix = settings->value("qflowprefix");
    QString index = settings->value(DEFAULT_VERILOG);

    QFile index_file(path + "/source/" + index + ".v");
    if (index_file.open(QIODevice::ReadWrite))
    {
        QTextStream stream(&index_file);
        stream
                << endl
                << "module index();" << endl
                << "endmodule" << endl
                << endl;
        index_file.close();
    }

    QFile project_vars(path + PROJECT_VARS);
    if (project_vars.open(QIODevice::ReadWrite))
    {
        QTextStream stream(&project_vars);
        stream
                << TCSH_SHEBANG << endl
                << "#-------------------------------------------" << endl
                << "# project variables for project " << path << endl
                << "#-------------------------------------------" << endl
                << endl
                << endl;
        project_vars.close();
        project_vars.setPermissions(executable);
    }

    QFile qflow_vars(path + QFLOW_VARS);
    if (qflow_vars.open(QIODevice::ReadWrite))
    {
        QTextStream stream(&qflow_vars);
        stream
                << TCSH_SHEBANG << endl
                << "#-------------------------------------------" << endl
                << "# qflow variables for project " << path << endl
                << "#-------------------------------------------" << endl
                << endl
                << "set projectpath=" << path << endl
                << "set techdir=" << qflowprefix << "/tech/osu035" << endl
                << "set sourcedir=" << path << "/source"    << endl
                << "set synthdir="  << path << "/synthesis" << endl
                << "set layoutdir=" << path << "/layout"    << endl
                << "set techname=osu035" << endl
                << "set scriptdir=" << qflowprefix << "/scripts" << endl
                << "set bindir=" << qflowprefix << "/bin" << endl
                << "set synthlog=" << path << "/synth.log" << endl
                << "set index=" << index << endl
                << "#-------------------------------------------" << endl
                << endl
                << endl;
        qflow_vars.close();
        qflow_vars.setPermissions(executable);
    }
}

void Qflow::synthesis(QProcess *exec, QString ident)
{
    QString qflowprefix = settings->value("qflowprefix");
    QFile file(exec->workingDirectory() + "/" + QFLOW_EXEC);
    if (file.open(QFile::ReadWrite | QFile::Truncate))
    {
        QTextStream stream(&file);
        stream
                << TCSH_SHEBANG << endl
                << qflowprefix << "/scripts/synthesize.sh"
                << " " << exec->workingDirectory()
                << " " << ident
                << " " << "|| exit 1" << endl
                << endl;
        file.close();
        file.setPermissions(executable);
    }
    exec->start(QFLOW_EXEC);
}

void Qflow::timing(QProcess *exec, QString ident)
{
    QString qflowprefix = settings->value("qflowprefix");
    QFile file(exec->workingDirectory() + "/" + QFLOW_EXEC);
    if (file.open(QFile::ReadWrite | QFile::Truncate))
    {
        QTextStream stream(&file);
        stream
                << TCSH_SHEBANG << endl
                << qflowprefix << "/scripts/vesta.sh"
                << " " << exec->workingDirectory()
                << " " << ident
                << " " << "|| exit 1" << endl
                << endl;
        file.close();
        file.setPermissions(executable);
    }
    exec->start(QFLOW_EXEC);
}

void Qflow::placement(QProcess *exec, QString ident)
{
    QString qflowprefix = settings->value("qflowprefix");
    QFile file(exec->workingDirectory() + "/" + QFLOW_EXEC);
    if (file.open(QFile::ReadWrite | QFile::Truncate))
    {
        QTextStream stream(&file);
        stream
                << TCSH_SHEBANG << endl
                << qflowprefix << "/scripts/placement.sh"
                << " " << exec->workingDirectory()
                << " " << ident
                << " " << "|| exit 1" << endl
                << endl;
        file.close();
        file.setPermissions(executable);
    }
    exec->start(QFLOW_EXEC);
}

void Qflow::routing(QProcess *exec, QString ident)
{
    QString qflowprefix = settings->value("qflowprefix");
    QFile file(exec->workingDirectory() + "/" + QFLOW_EXEC);
    if (file.open(QFile::ReadWrite | QFile::Truncate))
    {
        QTextStream stream(&file);
        stream
                << TCSH_SHEBANG << endl
                << qflowprefix << "/scripts/router.sh"
                << " " << exec->workingDirectory()
                << " " << ident
                << " " << "|| exit 1" << endl
                << endl;
        file.close();
        file.setPermissions(executable);
    }
    exec->start(QFLOW_EXEC);
}

void Qflow::buildAll(QProcess *exec, QString ident)
{
    QString qflowprefix = settings->value("qflowprefix");
    QFile file(exec->workingDirectory() + "/" + QFLOW_EXEC);
    if (file.open(QFile::ReadWrite | QFile::Truncate))
    {
        QTextStream stream(&file);
        stream
                << TCSH_SHEBANG << endl

                << qflowprefix << "/scripts/synthesize.sh"
                << " " << exec->workingDirectory()
                << " " << ident
                << " " << "|| exit 1" << endl

                << qflowprefix << "/scripts/placement.sh -d"
                << " " << exec->workingDirectory()
                << " " << ident
                << " " << "|| exit 1" << endl

                << qflowprefix << "/scripts/router.sh"
                << " " << exec->workingDirectory()
                << " " << ident
                << " " << "|| exit 1" << endl

                << endl;
        file.close();
        file.setPermissions(executable);
    }
    exec->start(QFLOW_EXEC);
}
