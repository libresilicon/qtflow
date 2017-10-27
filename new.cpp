#include "new.h"
#include "ui_new.h"

#include <QFile>
#include <QLineEdit>
#include <QListWidget>
#include <QTextStream>

New::New(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::New),
	project(NULL)
{
	ui->setupUi(this);
}

New::~New()
{
	delete ui;
}

QString New::fileExtension(new_element_t e)
{
	switch (e)
	{
		case Magic:
			return "mag";
		case Verilog:
			return "v";
		case VerilogTestbench:
			return "tb.v";
		default:
			return QString();
	}
}

QFileInfo New::createMagicFile()
{
	QString name = ui->fileBase->text();
	QString path = QDir(project->getLayoutDir()).filePath(name+".mag");
	QFile file(path);

	if (file.exists())
		return QFileInfo(path);

	if (!file.open(QIODevice::ReadWrite))
		return QFileInfo(path);

	QFile magicFile(path);
	if(magicFile.open(QIODevice::WriteOnly)) {
		QTextStream outputStream(&magicFile);
		outputStream << "magic" << endl;
		outputStream << "tech " << project->getTechnology() << endl;
		outputStream << "magscale 1 2" << endl;
		outputStream << "timestamp " << QDateTime::currentMSecsSinceEpoch() << endl;
		outputStream << "<< end >>" << endl;
		magicFile.close();
	}

	return QFileInfo(path);
}

void New::setProject(Project *p)
{
	project = p;
}

void New::on_buttonBox_accepted()
{
	QFileInfo info;
	if(!project) return;

	info = createMagicFile();

	emit fileCreated(info.absoluteFilePath());
}
