#ifndef MAINPROJECTS_H
#define MAINPROJECTS_H

#include <QWidget>

namespace Ui {
class MainProjects;
}

class MainProjects : public QWidget
{
    Q_OBJECT

public:
    explicit MainProjects(QWidget *parent = 0);
    ~MainProjects();

private:
    Ui::MainProjects *ui;
};

#endif // MAINPROJECTS_H
