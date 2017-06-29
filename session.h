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

    void setApp(App* window);
    App* getApp();

private:
    Session()
    {
        projectPath = "";
        app = NULL;
    }

    Session(Session const&);
    void operator=(Session const&);

    QString projectPath;

    App *app;
};

#endif // SESSION_H
