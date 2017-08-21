
#include "constants.h"
#include "project.h"

#include <QString>
#include <QTextStream>
#include <QDir>
#include <QProcess>
#include <QSettings>
#include <iostream>

Project::Project():
	IProject()
{
	executable
		= QFileDevice::ReadOwner
		| QFileDevice::WriteOwner
		| QFileDevice::ExeOwner
		| QFileDevice::ReadGroup
		| QFileDevice::WriteGroup
		| QFileDevice::ExeGroup
		| QFileDevice::ReadOther;
}

Project::Project(QString path) :
	IProject()
{
	executable
		= QFileDevice::ReadOwner
		| QFileDevice::WriteOwner
		| QFileDevice::ExeOwner
		| QFileDevice::ReadGroup
		| QFileDevice::WriteGroup
		| QFileDevice::ExeGroup
		| QFileDevice::ReadOther;

	project_settings = new QSettings(path);
}

Project::~Project()
{
	delete settings;
	delete project_settings;
}

void Project::setTechnology(QString tech)
{
	QTextStream(stdout) << "Setting technology to: " << tech << "\n";
	project_settings->setValue("technology",tech);
}

bool Project::prepareStep(QString path)
{
/*	QString Projectprefix = settings->value("Projectprefix");
	QString index = settings->value(DEFAULT_VERILOG);

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

	QFile Project_vars(path + Project_VARS);
	if (Project_vars.open(QIODevice::ReadWrite))
	{
		QTextStream stream(&Project_vars);
		stream
		    << TCSH_SHEBANG << endl
		    << "#-------------------------------------------" << endl
		    << "# Project variables for project " << path << endl
		    << "#-------------------------------------------" << endl
		    << endl
		    << "set projectpath=" << project_settings->value("projectpath","").toString() << endl
		    << "set techdir=" << project_settings->value("techdir","").toString() << endl
		    << "set sourcedir=" << project_settings->value("sourcedir","").toString() << endl
		    << "set synthdir="  << project_settings->value("synthdir","").toString() << endl
		    << "set layoutdir=" << project_settings->value("layoutdir","").toString()<< endl
		    << "set techname=" << project_settings->value("techname","osu035").toString() << endl
		    << "set scriptdir=" << project_settings->value("scriptdir","").toString() << endl
		    << "set bindir=" << project_settings->value("bindir","").toString() << endl
		    << "set synthlog=" << project_settings->value("synthlog","").toString() << endl
		    << "#-------------------------------------------" << endl
		    << endl
		    << endl;
		Project_vars.close();
		Project_vars.setPermissions(executable);
	}
	return true;*/
}

bool Project::create(QString path)
{
	QDir dir(path);
	dir.mkdir("source");
	dir.mkdir("synthesis");
	dir.mkdir("layout");

	//QString Projectprefix = settings->value("Projectprefix");
	QStringList path_pieces = path.split( '/' );
	QString project_name = path + '/' + path_pieces.value( path_pieces.length() - 1 ) + ".pro";
	QTextStream(stdout) << QString("Creating project: ") << project_name << "\n";
	project_settings = new QSettings(project_name, QSettings::NativeFormat);
	project_settings->setValue("technology", "osu035");

	return true;
}

bool Project::synthesis(QString ident, QProcess *exec)
{
	/*QString Projectprefix = settings->value("Projectprefix");
	QFile file(exec->workingDirectory() + "/" + Project_EXEC);
	if (file.open(QFile::ReadWrite | QFile::Truncate))
	{
		QTextStream stream(&file);
		stream
				<< TCSH_SHEBANG << endl
				<< Projectprefix << "/scripts/synthesize.sh"
				<< " " << exec->workingDirectory()
				<< " " << ident
				<< " " << "|| exit 1" << endl
				<< endl;
		file.close();
		file.setPermissions(executable);
	}
	exec->start(settings->value("yosys_path","/usr/bin/yosys"));*/

	return true;
}

bool Project::timing(QString ident, QProcess *exec)
{
	/*QString Projectprefix = settings->value("Projectprefix");
	QFile file(exec->workingDirectory() + "/" + Project_EXEC);
	if (file.open(QFile::ReadWrite | QFile::Truncate))
	{
		QTextStream stream(&file);
		stream
				<< TCSH_SHEBANG << endl
				<< Projectprefix << "/scripts/vesta.sh"
				<< " " << exec->workingDirectory()
				<< " " << ident
				<< " " << "|| exit 1" << endl
				<< endl;
		file.close();
		file.setPermissions(executable);
	}
	exec->start(Project_EXEC);*/

	return true;
}

bool Project::placement(QString ident, QProcess *exec)
{
	/*QString Projectprefix = settings->value("Projectprefix");
	QFile file(exec->workingDirectory() + "/" + Project_EXEC);
	if (file.open(QFile::ReadWrite | QFile::Truncate))
	{
		QTextStream stream(&file);
		stream
				<< TCSH_SHEBANG << endl
				<< Projectprefix << "/scripts/placement.sh"
				<< " " << exec->workingDirectory()
				<< " " << ident
				<< " " << "|| exit 1" << endl
				<< endl;
		file.close();
		file.setPermissions(executable);
	}
	exec->start(Project_EXEC);*/

	return true;
}

bool Project::routing(QString ident, QProcess *exec)
{
	/*QString Projectprefix = settings->value("Projectprefix");
	QFile file(exec->workingDirectory() + "/" + Project_EXEC);
	if (file.open(QFile::ReadWrite | QFile::Truncate))
	{
		QTextStream stream(&file);
		stream
				<< TCSH_SHEBANG << endl
				<< Projectprefix << "/scripts/router.sh"
				<< " " << exec->workingDirectory()
				<< " " << ident
				<< " " << "|| exit 1" << endl
				<< endl;
		file.close();
		file.setPermissions(executable);
	}
	exec->start(Project_EXEC);*/

	return true;
}

bool Project::buildAll(QString ident, QProcess *exec)
{
	/*QString Projectprefix = settings->value("Projectprefix");
	QFile file(exec->workingDirectory() + "/" + Project_EXEC);
	if (file.open(QFile::ReadWrite | QFile::Truncate))
	{
		QTextStream stream(&file);
		stream
				<< TCSH_SHEBANG << endl

				<< Projectprefix << "/scripts/synthesize.sh"
				<< " " << exec->workingDirectory()
				<< " " << ident
				<< " " << "|| exit 1" << endl

				<< Projectprefix << "/scripts/placement.sh -d"
				<< " " << exec->workingDirectory()
				<< " " << ident
				<< " " << "|| exit 1" << endl

				<< Projectprefix << "/scripts/router.sh"
				<< " " << exec->workingDirectory()
				<< " " << ident
				<< " " << "|| exit 1" << endl

				<< endl;
		file.close();
		file.setPermissions(executable);
	}
	exec->start(Project_EXEC);*/

	return true;
}
