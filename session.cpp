#include "session.h"

void Session::setProject(QString path)
{
    projectPath = path;
    projectActive = true;
}

QString Session::project()
{
    return projectPath;
}

bool Session::isProjectActive()
{
    return projectActive;
}

void Session::setFile(QString path)
{
    filePath = path;
    fileActive = true;
}

QString Session::file()
{
    return filePath;
}

bool Session::isFileActive()
{
    return fileActive;
}

void Session::setApp(App *window)
{
    app = window;
}

App* Session::getApp()
{
    return app;
}
