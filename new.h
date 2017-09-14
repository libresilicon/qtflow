#ifndef NEW_H
#define NEW_H

#include <QDialog>
#include <QFileInfo>

enum new_element_t
{
	Verilog, VerilogTestbench
};

namespace Ui {
	class New;
}

class New : public QDialog
{
	Q_OBJECT

public:
	explicit New(QWidget *parent = 0);
	~New();

	void suggest(new_element_t = Verilog, QString = QString());

signals:
	void fileCreated(QFileInfo&);

private slots:
	void on_listWidget_currentRowChanged(int currentRow);

	void on_editModule_textChanged(const QString &arg1);

	void on_buttonBox_accepted();

private:
	Ui::New *ui;
	QString fileExtension(new_element_t);
};

#endif // NEW_H
