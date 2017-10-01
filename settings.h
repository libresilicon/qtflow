#ifndef Settings_H
#define Settings_H

#include "ui_settings.h"

#include <QDialog>
#include <QSettings>

#include <QDebug>
#include <QListWidget>
#include <QStackedWidget>
#include <QTextStream>
#include <QFileDialog>

#include <KTextEditor/ConfigPage>
#include <KTextEditor/Editor>

namespace Ui {
	class Settings;
}

class Settings : public QDialog
{
	Q_OBJECT

public:
	Settings(QWidget *parent, QSettings *s);

signals:
	void syncSettings();

private slots:
	void selectYosys_triggered();
	void selectGraywolf_triggered();
	void selectQRouter_triggered();
	void selectIcarus_triggered();

	void applyEnvironment();

    void on_listWidget_currentRowChanged(int currentRow);

    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::Settings *ui;
    QSettings *settings;

    KTextEditor::Editor *textEditor;

    QWidget *page1;
    KTextEditor::ConfigPage *page2;
    KTextEditor::ConfigPage *page3;
    KTextEditor::ConfigPage *page4;
    KTextEditor::ConfigPage *page5;
};

#endif // Settings_H
