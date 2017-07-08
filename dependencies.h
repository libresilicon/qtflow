#ifndef DEPENDENCIES_H
#define DEPENDENCIES_H

#include "settings.h"

class IDependencies
{
public:
    IDependencies() {}
    virtual ~IDependencies() {}

    virtual bool tcsh() { return false; }
    virtual bool qflow() { return false; }
};

class Dependencies : public IDependencies
{
public:
    Dependencies();
    ~Dependencies();

    bool tcsh();
    bool qflow();

private:
    QtFlowSettings *qtflow;
};

#endif // DEPENDENCIES_H
