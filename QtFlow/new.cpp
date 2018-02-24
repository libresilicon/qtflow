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
	QMap<QString,QString> ftypes;
	QListWidgetItem *w;
	ui->setupUi(this);

	ftypes["verilog"] = "Verilog file";
	ftypes["magic"] = "Magic file";
	ftypes["symbol"] = "Symbol file";

	foreach(QString key, ftypes.keys()) {
		w = new QListWidgetItem;
		w->setData(Qt::UserRole, QVariant(key));
		w->setText(ftypes[key]);
		ui->listWidget->addItem(w);
	}
}

New::~New()
{
	delete ui;
}

QFileInfo New::createSymbolFile()
{
	QString name = ui->fileBase->text();
	QString path = QDir(project->getSourceDir()).filePath(name+".sym");
	QFile file(path);

	if (file.exists())
		return QFileInfo(path);

	if (!file.open(QIODevice::ReadWrite))
		return QFileInfo(path);

	QFile symbolFile(path);
	if(symbolFile.open(QIODevice::WriteOnly)) {
		QTextStream outputStream(&symbolFile);

		outputStream << "EESchema-LIBRARY Version 2.3" << endl;
		outputStream << "#encoding utf-8" << endl;
		outputStream << "#" << endl;
		outputStream << "# " << name << endl;
		outputStream << "#" << endl;
		outputStream << "DEF " << name << " " << name << " 0 10 N Y 1 F N" << endl;
		outputStream << "F0 \"" << name << "\" 25 100 50 H V L CNN" << endl;
		outputStream << "F1 \"" << name << "\" 25 -100 50 H V L CNN" << endl;
		outputStream << "F2 \"\" 38 -150 50 H V C CNN" << endl;
		outputStream << "F3 \"\" 0 0 50 H V C CNN" << endl;
		outputStream << "DRAW" << endl;
		outputStream << "ENDDRAW" << endl;

		symbolFile.close();
	}

	return QFileInfo(path);
}

QFileInfo New::createVerilogFile()
{
	QString name = ui->fileBase->text();
	QString path = QDir(project->getSourceDir()).filePath(name+".v");
	QFile file(path);

	if (file.exists())
		return QFileInfo(path);

	if (!file.open(QIODevice::ReadWrite))
		return QFileInfo(path);

	QFile verilogFile(path);
	if(verilogFile.open(QIODevice::WriteOnly)) {
		QTextStream outputStream(&verilogFile);
		outputStream << "module " << name << "();" << endl;
		outputStream << "endmodule" << endl;
		verilogFile.close();
	}

	return QFileInfo(path);
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
	QListWidgetItem *w;
	QFileInfo info;
	QString ftype;
	QVariant data;
	if(!project) return;

	ftype = "asic";
	foreach(w, ui->listWidget->selectedItems()) {
		if (w != 0) {
			data = w->data(Qt::UserRole);
			ftype = data.toString();
		}
	}

	if(ftype=="magic") {
		info = createMagicFile();
		emit fileCreated(info.absoluteFilePath());
	}

	if(ftype=="verilog") {
		info = createVerilogFile();
		emit fileCreated(info.absoluteFilePath());
	}

	if(ftype=="symbol") {
		info = createSymbolFile();
		emit fileCreated(info.absoluteFilePath());
	}
}
