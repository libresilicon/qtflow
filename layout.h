#ifndef LAYOUT_H
#define LAYOUT_H

#include "common.h"

#include <QMainWindow>

namespace Ui {
class Layout;
}

class Layout : public QMainWindow
{
    Q_OBJECT

public:
    explicit Layout(QWidget *parent = 0);
    ~Layout();

private slots:
    void on_Layout_destroyed();

    void on_newProject_triggered();
    void on_openMagicFile_triggered();

private:
    Ui::Layout *ui;

    rects_t loadMagicFile(QString name);
};

#endif // LAYOUT_H
