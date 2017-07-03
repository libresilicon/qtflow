#ifndef MAINEDIT_H
#define MAINEDIT_H

#include "session.h"
#include "editor.h"

#include <QWidget>
#include <QFileSystemModel>

namespace Ui {
class Edit;
}

class Edit : public QWidget
{
    Q_OBJECT

public:
    explicit Edit(QWidget *parent = 0);
    ~Edit();

    void loadProject(QString path);

    void loadFile(QString path);
    void saveFile(QString path);

public slots:
    void saveAndExit(int);

private slots:
    void on_filesView_doubleClicked(const QModelIndex &index);

    void on_tabbedEditor_currentChanged(int index);

    void on_tabbedEditor_tabCloseRequested(int index);

private:
    Ui::Edit *ui;

    Session &session;

    QFileSystemModel *filesystem;
    QList<IEditor*> *opened;
};

#endif // MAINEDIT_H
