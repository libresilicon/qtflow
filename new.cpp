#include "new.h"
#include "ui_new.h"

#include <QFile>
#include <QLineEdit>
#include <QListWidget>
#include <QTextStream>

New::New(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::New)
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
		case Verilog:
			return "v";
		case VerilogTestbench:
			return "tb.v";
		default:
			return QString();
	}
}

void New::suggest(new_element_t e, QString name)
{
	ui->editModule->setText(name);
	ui->listWidget->setCurrentRow(e);
}

void New::on_listWidget_currentRowChanged(int row)
{
	auto e = static_cast<new_element_t>(row);
	ui->editFilename->setText(ui->editModule->text() + "." + fileExtension(e));
}

void New::on_editModule_textChanged(const QString &content)
{
	auto e = static_cast<new_element_t>(ui->listWidget->currentRow());
	ui->editFilename->setText(content + "." + fileExtension(e));
}

void New::on_buttonBox_accepted()
{
	/*QString path = qflow.value("sourcedir") + "/" + ui->editFilename->text();
	QFile file(path);
	QFileInfo info(path);

	if (file.exists())
		return;

	if (!file.open(QIODevice::ReadWrite))
		return;

	auto e = static_cast<new_element_t>(ui->listWidget->currentRow());
	QString name = ui->editModule->text();
	if (e == VerilogTestbench)
		name += "_testbench";

	QTextStream out(&file);
	out
			<< "module " << name << "();" << endl
			<< "endmodule" << endl;

	file.close();

	emit fileCreated(info);*/
}
