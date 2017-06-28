#ifndef PROJECT_H
#define PROJECT_H

#include "settings.h"
#include <QString>

class Project
{
public:
    Project();
    void createQflow(QString path);
private:
    Settings *settings;
};

#endif // PROJECT_H
