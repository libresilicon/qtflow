#ifndef PYPROJECTSETTINGS_H
#define PYPROJECTSETTINGS_H

#include <QObject>

class PyProjectSettings : public QObject
{
	Q_OBJECT
public:
	explicit PyProjectSettings(QObject *parent = nullptr);

signals:

public slots:
};

#endif // PYPROJECTSETTINGS_H