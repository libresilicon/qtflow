#ifndef PROJECT_H
#define PROJECT_H

#include "constants.h"
#include "settings.h"

#include <QString>
#include <QProcess>
#include <QFileDevice>

class IProject
{
public:
    IProject() {}
    virtual ~IProject() {}
    virtual void create(QString) {}
    virtual void synthesis(QProcess* = new QProcess, QString = DEFAULT_VERILOG) {}
    virtual void timing(QProcess* = new QProcess, QString = DEFAULT_VERILOG) {}
    virtual void placement(QProcess* = new QProcess, QString = DEFAULT_VERILOG) {}
    virtual void routing(QProcess* = new QProcess, QString = DEFAULT_VERILOG) {}
    virtual void buildAll(QProcess* = new QProcess, QString = DEFAULT_VERILOG) {}
};

class Qflow : public IProject
{
public:
    Qflow();
    ~Qflow();
    void create(QString);
    void synthesis(QProcess* = new QProcess, QString ident = DEFAULT_VERILOG);
    void timing(QProcess* = new QProcess, QString = DEFAULT_VERILOG);
    void placement(QProcess* = new QProcess, QString = DEFAULT_VERILOG);
    void routing(QProcess* = new QProcess, QString = DEFAULT_VERILOG);
    void buildAll(QProcess* = new QProcess, QString = DEFAULT_VERILOG);
private:
    ISettings *settings;

    QFileDevice::Permissions executable;
};

#endif // PROJECT_H
