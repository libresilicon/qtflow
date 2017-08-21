#ifndef DEPENDENCIES_H
#define DEPENDENCIES_H

#include <QSettings>

class IDependencies
{
public:
	IDependencies() {}
	virtual ~IDependencies() {}

	virtual bool tcsh() { return false; }
	virtual bool qflow() { return false; }
};

class Dependencies : public IDependencies
{
public:
	Dependencies(QSettings*);
	~Dependencies();

	bool tcsh();
	bool qflow();

private:
	QSettings *qtflow;
};

#endif // DEPENDENCIES_H
