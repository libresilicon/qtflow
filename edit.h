#ifndef MAINEDIT_H
#define MAINEDIT_H

#include "session.h"
#include "editor.h"
#include "projectstreemodel.h"
#include "modulestreemodel.h"

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
    void onCustomContextMenu(const QPoint&);
    void onOpenTcsh(bool);
    void onTcshError(QProcess::ProcessError);

private slots:
    void on_filesView_doubleClicked(const QModelIndex &index);

    void on_tabbedEditor_currentChanged(int index);

    void on_tabbedEditor_tabCloseRequested(int index);

    void on_treeSelection_currentIndexChanged(int index);

    void on_projectsView_doubleClicked(const QModelIndex &index);

    void on_modulesView_doubleClicked(const QModelIndex &index);

private:
    Ui::Edit *ui;

    Session &session;

    QList<IEditor*> *opened;
    QFileSystemModel *filesystem;
    ProjectsTreeModel *projects;
    ModulesTreeModel *modules;
    QMenu *filesContext;
    QAction *openTcsh;
};

#endif // MAINEDIT_H
