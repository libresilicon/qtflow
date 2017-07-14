#ifndef MAINEDIT_H
#define MAINEDIT_H

#include "session.h"
#include "editor.h"
#include "projectstreemodel.h"
#include "modulestreemodel.h"
#include "new.h"

#include <QWidget>
#include <QFileSystemModel>
#include <QProcess>

namespace Ui {
class Edit;
}

class Edit : public QWidget
{
    Q_OBJECT

public:
    explicit Edit(QWidget *parent = 0);
    ~Edit();

    void loadProject(const QString &path);

    void loadFile(QString path);
    void saveFile(QString path);

public slots:
    void saveAndExit(int);

    void onFilesContextMenu(const QPoint&);
    void onOpenTcsh(bool);
    void onTcshError(QProcess::ProcessError);

    void onProjectsContextMenu(const QPoint&);
    void onCreateTestbench(bool);

    void onLoadFile(QFileInfo&);

private slots:
    void on_filesView_doubleClicked(const QModelIndex &index);

    void on_tabbedEditor_currentChanged(int index);

    void on_tabbedEditor_tabCloseRequested(int index);

    void on_treeSelection_currentIndexChanged(int index);

    void on_projectsView_doubleClicked(const QModelIndex &index);

    void on_modulesView_doubleClicked(const QModelIndex &index);

private:
    Ui::Edit *ui;
    New *createWidget;

    Session &session;

    QList<IEditor*> *opened;
    QFileSystemModel *filesystem;
    ProjectsTreeModel *projects;
    ModulesTreeModel *modules;

    QMenu *filesContext;
    QAction *openTcsh;

    QMenu *projectsContext;
    QAction *createTestbench;
};

#endif // MAINEDIT_H
