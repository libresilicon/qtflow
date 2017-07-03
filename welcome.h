#ifndef MAINWELCOME_H
#define MAINWELCOME_H

#include <QWidget>

namespace Ui {
class Welcome;
}

class Welcome : public QWidget
{
    Q_OBJECT

public:
    explicit Welcome(QWidget *parent = 0);
    ~Welcome();

private:
    Ui::Welcome *ui;
};

#endif // MAINWELCOME_H
