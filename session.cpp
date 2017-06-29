#include "session.h"

void Session::setProject(QString path)
{
    projectPath = path;
}

void Session::setApp(App *window)
{
    app = window;
}

App* Session::getApp()
{
    return app;
}
