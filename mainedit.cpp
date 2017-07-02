#include "mainedit.h"
#include "ui_mainedit.h"

#include "editor.h"
#include "savechanges.h"
#include "verilog.h"

#include <QTreeView>

MainEdit::MainEdit(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainEdit),
    session(Session::Instance()),
    filesystem(new QFileSystemModel),
    opened(new QList<IEditor *>)
{
    ui->setupUi(this);
    ui->filesView->setModel(filesystem);
    for (int i = 1; i < filesystem->columnCount(); ++i)
        ui->filesView->hideColumn(i);
    loadProject(QDir::currentPath());
}

MainEdit::~MainEdit()
{
    delete ui;
    delete filesystem;
    delete opened;
}

void MainEdit::loadProject(QString path)
{
    filesystem->setRootPath(path);
    ui->filesView->setRootIndex(filesystem->index(path));
}

void MainEdit::loadFile(QString path)
{
    QFileInfo info(path);
    int found = -1;
    for (int i = 0; i < opened->count(); ++i)
        if (info.absoluteFilePath() == opened->at(i)->getFilePath())
            found = i;
    if (found < 0)
    {
        Editor *editor = new Editor(this);
        if (info.suffix() == "v")
            editor->setSyntax(new VerilogHighlight(editor->document()));

        opened->append(editor);
        ui->tabbedEditor->addTab(editor, info.fileName());
        editor->loadFile(info.absoluteFilePath());
        found = ui->tabbedEditor->count() - 1;
    }
    ui->tabbedEditor->setCurrentIndex(found);
    ui->tabbedEditor->widget(found)->setFocus();
    session.setFile(info.absoluteFilePath());
    session.getApp()->enableFile();
}

void MainEdit::saveFile(QString path)
{
    int index = ui->tabbedEditor->currentIndex();
    if (index < 0)
        return;
    opened->at(index)->saveFile(path);
}

void MainEdit::saveAndExit(int index)
{
    opened->at(index)->saveFile();
    ui->tabbedEditor->removeTab(index);
    opened->removeAt(index);
}

void MainEdit::on_filesView_doubleClicked(const QModelIndex &index)
{
    loadFile(filesystem->filePath(index));
}

void MainEdit::on_tabbedEditor_currentChanged(int index)
{
    session.setFile(opened->at(index)->getFilePath());
    session.getApp()->enableFile();
}

void MainEdit::on_tabbedEditor_tabCloseRequested(int index)
{
    if (opened->count() == 1)
        return;

    IEditor *editor = opened->at(index);
    if (editor->changes())
    {
        SaveChanges *dialog = new SaveChanges(this, index, editor->getFilePath());
        connect(dialog, SIGNAL(accepted(int)), this, SLOT(saveAndExit(int)));
        dialog->show();
    }
    else
    {
        ui->tabbedEditor->removeTab(index);
        opened->removeAt(index);
    }
}
