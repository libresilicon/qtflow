#ifndef APP_H
#define APP_H

class App {

public:
    App() {}
    virtual ~App() {}

    virtual void enableProject() {}
    virtual void disableProject() {}

    virtual void enableFile() {}
    virtual void disableFile() {}
};

#endif // APP_H
