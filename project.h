#ifndef PROJECT_H
#define PROJECT_H

#include "settings.h"
#include <QString>
#include <QProcess>

class IProject
{
public:
    IProject() {}
    virtual ~IProject() {}
    virtual void createQflow(QString) {}
    virtual void executeQflow(QProcess*) {}
};

class Project : public IProject
{
public:
    Project();
    ~Project();
    void createQflow(QString);
    void executeQflow(QProcess*);
private:
    ISettings *settings;
};

#endif // PROJECT_H
