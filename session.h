#ifndef SESSION_H
#define SESSION_H

#include "app.h"

#include <QObject>
#include <QString>

class Session
{
public:
    static Session& Instance()
    {
        static Session instance;
        return instance;
    }

    void setProject(QString path);
    QString getProject();
    bool isProjectActive();

    void setApp(App* window);
    App* getApp();

private:
    Session()
    {
        projectPath = "";
        projectActive = false;
        app = NULL;
    }

    Session(Session const&);
    void operator=(Session const&);

    bool projectActive;
    QString projectPath;

    App *app;
};

#endif // SESSION_H
