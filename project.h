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
    virtual bool create(QString) { return false; }
    virtual bool valuedump(QString = DEFAULT_VERILOG, QProcess* = new QProcess) { return false; }
    virtual bool synthesis(QString = DEFAULT_VERILOG, QProcess* = new QProcess) { return false; }
    virtual bool timing(QString = DEFAULT_VERILOG, QProcess* = new QProcess) { return false; }
    virtual bool placement(QString = DEFAULT_VERILOG, QProcess* = new QProcess) { return false; }
    virtual bool routing(QString = DEFAULT_VERILOG, QProcess* = new QProcess) { return false; }
    virtual bool buildAll(QString = DEFAULT_VERILOG, QProcess* = new QProcess) { return false; }
};

class Qflow : public IProject
{
public:
    Qflow();
    ~Qflow();
    bool create(QString);
    bool valuedump(QString ident = DEFAULT_VERILOG, QProcess* = new QProcess);
    bool synthesis(QString ident = DEFAULT_VERILOG, QProcess* = new QProcess);
    bool timing(QString ident = DEFAULT_VERILOG, QProcess* = new QProcess);
    bool placement(QString ident = DEFAULT_VERILOG, QProcess* = new QProcess);
    bool routing(QString ident = DEFAULT_VERILOG, QProcess* = new QProcess);
    bool buildAll(QString ident = DEFAULT_VERILOG, QProcess* = new QProcess);
private:
    ISettings *settings;

    QFileDevice::Permissions executable;
};

#endif // PROJECT_H
