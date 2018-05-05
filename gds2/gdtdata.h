#ifndef GDTDATA_H
#define GDTDATA_H

#include <QString>
#include <QDebug>

#include "gdscell.h"

class GDSFile;
class GDTData
{
public:
	GDTData(QString fileName); // constructor to open (create if WRITE) stream file (calls opstrm())
	bool containsCell(QString name);
	GDSCell* getCell(QString name);
	QVector<GDSCell*> getCells();

private:
	void goThroughFile();
	void buildDataStructure();

	GDSFile *m_file;
	QString m_fileName;

	QVector<GDSCell*> m_cells;
};

#endif // GDTDATA_H
