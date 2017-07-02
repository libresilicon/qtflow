#ifndef SAVECHANGES_H
#define SAVECHANGES_H

#include <QDialog>
#include <QFile>

namespace Ui {
class SaveChanges;
}

class SaveChanges : public QDialog
{
    Q_OBJECT

public:
    explicit SaveChanges(QWidget *parent = 0, int i = 0, QString path = "");
    ~SaveChanges();

signals:
    void accepted(int);
    void rejected();

private slots:
    void on_buttonBox_rejected();

    void on_buttonBox_accepted();

private:
    Ui::SaveChanges *ui;

    int index;
};

#endif // SAVECHANGES_H
