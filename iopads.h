#ifndef IOPADS_H
#define IOPADS_H

#include <QDockWidget>

namespace Ui {
class IOPads;
}

class IOPads : public QDockWidget
{
    Q_OBJECT

public:
    explicit IOPads(QWidget *parent = 0);
    ~IOPads();

private:
    Ui::IOPads *ui;
};

#endif // IOPADS_H
