#ifndef MODULES_H
#define MODULES_H

#include "moduleslistmodel.h"
#include "settings.h"

#include <QDialog>
#include <QAbstractListModel>


namespace Ui {
class Modules;
}

class Modules : public QDialog
{
    Q_OBJECT

public:
    explicit Modules(QWidget *parent = 0);
    ~Modules();

    void refresh();
    void refresh(QString);

signals:
    void topModuleChanged();

private slots:
    void on_setTopModule_clicked();

    void on_closeButton_clicked();

private:
    Ui::Modules *ui;

    QString path;
    ModulesListModel *modules;
};

#endif // MODULES_H
