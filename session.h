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

    void setProject(QString);
    QString getProject();
    bool isProjectActive();

    void setFile(QString);
    QString getFile();
    bool isFileActive();

    void setApp(App*);
    App* getApp();

private:
    Session()
    {
        projectPath = "";
        projectActive = false;
        filePath = "";
        fileActive = false;
        app = NULL;
    }

    Session(Session const&);
    void operator=(Session const&);

    bool projectActive;
    QString projectPath;

    bool fileActive;
    QString filePath;

    App *app;
};

#endif // SESSION_H
