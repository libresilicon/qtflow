#include "session.h"

void Session::setProject(QString path)
{
    projectPath = path;
    projectActive = true;
}

QString Session::getProject()
{
    return projectPath;
}

bool Session::isProjectActive()
{
    return projectActive;
}

void Session::setApp(App *window)
{
    app = window;
}

App* Session::getApp()
{
    return app;
}
