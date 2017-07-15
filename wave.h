#ifndef WAVE_H
#define WAVE_H

#include <QWidget>

namespace Ui {
class Wave;
}

class Wave : public QWidget
{
    Q_OBJECT

public:
    explicit Wave(QWidget *parent = 0);
    ~Wave();

    void loadVcd(QString);

private:
    Ui::Wave *ui;
};

#endif // WAVE_H
