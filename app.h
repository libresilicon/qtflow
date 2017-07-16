#ifndef APP_H
#define APP_H

#include <QString>

class App {

public:
    App() {}
    virtual ~App() {}

    virtual void enableProject() {}
    virtual void disableProject() {}

    virtual void enableTopModule() {}

    virtual void enableFile() {}
    virtual void disableFile() {}

    virtual void error(QString) {}
};

#endif // APP_H
