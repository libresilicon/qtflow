#include "dependencies.h"

#include <stdlib.h>
#include <QDir>

Dependencies::Dependencies() :
    IDependencies(),
    qtflow(new QtFlowSettings)
{

}

Dependencies::~Dependencies()
{

}

bool Dependencies::tcsh()
{
    int code = system("tcsh -c");
    return code == 0;
}

bool Dependencies::qflow()
{
    int code = system("qflow -v");
    if (code != 0)
        return false;

    QString path;
    QDir qflowprefix(qtflow->value("qflowprefix"));
    if (!qflowprefix.exists())
        return false;

    return true;
}
