#include "pysettings.h"

PySettings::PySettings(QObject *parent) : QObject(parent)
{

}

void PySettings::test()
{

}

PyObject* PySettings::getMainModule() {
        return PythonQt::self()->getMainModule();
}
