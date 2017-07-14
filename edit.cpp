#include "edit.h"
#include "ui_edit.h"

#include "editor.h"
#include "savechanges.h"
#include "settings.h"
#include "verilog.h"

#include <QTreeView>
#include <QTabBar>
#include <QMenu>
#include <QAction>
#include <QProcess>

Edit::Edit(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Edit),
    createWidget(new New),
    session(Session::Instance()),
    opened(new QList<IEditor *>),
    filesystem(new QFileSystemModel),
    projects(new ProjectsTreeModel(this)),
    modules(new ModulesTreeModel)
{
    ui->setupUi(this);
    ui->filesView->setModel(filesystem);
    ui->projectsView->setModel(projects);
    ui->modulesView->setModel(modules);
    ui->tabbedTree->setCurrentIndex(1);
    ui->treeSelection->setCurrentIndex(1);

    for (int i = 1; i < filesystem->columnCount(); ++i)
        ui->filesView->hideColumn(i);
    loadProject(session.project());

    connect(createWidget, SIGNAL(fileCreated(QFileInfo&)), this, SLOT(onLoadFile(QFileInfo&)));

    ui->filesView->setContextMenuPolicy(Qt::CustomContextMenu);
    filesContext = new QMenu(ui->filesView);
    connect(ui->filesView, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(onFilesContextMenu(const QPoint&)));

    openTcsh = new QAction("Open tcsh here...", filesContext);
    filesContext->addAction(openTcsh);
    connect(openTcsh, SIGNAL(triggered(bool)), this, SLOT(onOpenTcsh(bool)));

    ui->projectsView->setContextMenuPolicy(Qt::CustomContextMenu);
    projectsContext = new QMenu(ui->projectsView);
    connect(ui->projectsView, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(onProjectsContextMenu(const QPoint&)));

    createTestbench = new QAction("Add Testbench...", projectsContext);
    projectsContext->addAction(createTestbench);
    connect(createTestbench, SIGNAL(triggered(bool)), this, SLOT(onCreateTestbench(bool)));
}

Edit::~Edit()
{
    delete ui;
    delete createWidget;
    delete opened;
    delete filesystem;
    delete projects;
    delete modules;
}

void Edit::loadProject(const QString &dir)
{
    filesystem->setRootPath(dir);
    ui->filesView->setRootIndex(filesystem->index(dir));

    projects->setRootPath(dir);
    modules->setRootPath(dir);
}

void Edit::loadFile(QString path)
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

void Edit::saveFile(QString path)
{
    int index = ui->tabbedEditor->currentIndex();
    if (index < 0)
        return;
    opened->at(index)->saveFile(path);
}

void Edit::saveAndExit(int index)
{
    opened->at(index)->saveFile();
    ui->tabbedEditor->removeTab(index);
    opened->removeAt(index);
}

void Edit::onFilesContextMenu(const QPoint &point)
{
    QModelIndex index = ui->filesView->indexAt(point);
    if (index.isValid())
        filesContext->exec(ui->filesView->mapToGlobal(point));
}

void Edit::onProjectsContextMenu(const QPoint &point)
{
    QModelIndex index = ui->projectsView->indexAt(point);
    if (!index.isValid())
        return;
    projectsContext->setDisabled(projects->filePath(index) == QString());
    projectsContext->exec(ui->filesView->mapToGlobal(point));
}

void Edit::onOpenTcsh(bool)
{
    QtFlowSettings settings;
    QFileInfo info = filesystem->fileInfo(ui->filesView->currentIndex());
    QProcess *exec = new QProcess(this);
    connect(exec, SIGNAL(errorOccurred(QProcess::ProcessError)), this, SLOT(onTcshError(QProcess::ProcessError)));
    exec->setWorkingDirectory(info.absoluteDir().absolutePath());
    exec->start(settings.value("terminal") + " tcsh");
}

void Edit::onCreateTestbench(bool)
{
    QFileInfo info(projects->filePath(ui->projectsView->currentIndex()));
    createWidget->suggest(VerilogTestbench, info.baseName());
    createWidget->show();
}

void Edit::onLoadFile(QFileInfo &info)
{
    loadFile(info.absoluteFilePath());
}

void Edit::onTcshError(QProcess::ProcessError)
{
    session.getApp()->error("Could not run terminal, check terminal variable in ~/.qtflowrc");
}

void Edit::on_filesView_doubleClicked(const QModelIndex &index)
{
    loadFile(filesystem->filePath(index));
}

void Edit::on_tabbedEditor_currentChanged(int index)
{
    session.setFile(opened->at(index)->getFilePath());
    session.getApp()->enableFile();
}

void Edit::on_tabbedEditor_tabCloseRequested(int index)
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

void Edit::on_treeSelection_currentIndexChanged(int index)
{
    ui->tabbedTree->setCurrentIndex(index);
}

void Edit::on_projectsView_doubleClicked(const QModelIndex &index)
{
    QString path = projects->filePath(index);
    if (path != QString())
        loadFile(path);
}

void Edit::on_modulesView_doubleClicked(const QModelIndex &index)
{
    QString path = modules->filePath(index);
    if (path != QString())
        loadFile(path);
}
