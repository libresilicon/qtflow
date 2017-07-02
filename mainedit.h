#ifndef MAINEDIT_H
#define MAINEDIT_H

#include "session.h"
#include "editor.h"

#include <QWidget>
#include <QFileSystemModel>

namespace Ui {
class MainEdit;
}

class MainEdit : public QWidget
{
    Q_OBJECT

public:
    explicit MainEdit(QWidget *parent = 0);
    ~MainEdit();

    void loadProject(QString path);

    void loadFile(QString path);
    void saveFile(QString path);

private slots:
    void on_filesView_doubleClicked(const QModelIndex &index);

    void on_tabbedEditor_currentChanged(int index);

private:
    Ui::MainEdit *ui;

    Session &session;

    QFileSystemModel *filesystem;
    QList<IEditor*> *opened;
};

#endif // MAINEDIT_H
