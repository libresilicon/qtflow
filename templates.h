#ifndef TEMPLATES_H
#define TEMPLATES_H

#include <QDialog>

namespace Ui {
class Templates;
}

class Templates : public QDialog
{
    Q_OBJECT

public:
    explicit Templates(QWidget *parent = 0);
    ~Templates();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::Templates *ui;
};

#endif // TEMPLATES_H
