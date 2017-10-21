#ifndef GDTDATA_H
#define GDTDATA_H

#include <QString>
#include <QDebug>

#include "gdsfile.h"

class GDTData
{
public:
	GDTData(QString fileName); // constructor to open (create if WRITE) stream file (calls opstrm())

private:
	void goThroughFile();

	GDSFile *m_file;
	QString m_name;
};

#endif // GDTDATA_H
