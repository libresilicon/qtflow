#ifndef MAINEDIT_H
#define MAINEDIT_H

#include "session.h"

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

private:
    Ui::MainEdit *ui;

    QFileSystemModel *filesystem;
};

#endif // MAINEDIT_H
