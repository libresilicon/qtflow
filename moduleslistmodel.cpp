#include "moduleslistmodel.h"
#include "constants.h"

#include <QDirIterator>
#include <QTextStream>
#include <QFont>

ModulesListModel::ModulesListModel(QObject *parent, QString s) :
	QAbstractListModel(parent),
	modules(QList<QString>()),
	files(QList<QString>()),
	sourcedir(s),
	topmodule(-1)
{
	if (sourcedir == QString())
		return;

	QRegExp rx("^\\s*module ([a-zA-Z0-9_]+)");
	QDirIterator it(sourcedir, QStringList() << "*.v", QDir::Files, QDirIterator::Subdirectories);
	while (it.hasNext())
	{
		QFile file(it.next());
		QFileInfo info(file);
		if (info.completeSuffix() == info.suffix() && file.open(QIODevice::ReadOnly))
		{
			QTextStream in(&file);
			while (!in.atEnd())
			{
				QString line = in.readLine();
				if (rx.indexIn(line, 0) != -1)
				{
					modules << rx.cap(1);
					files << info.baseName();
				}
			}
			file.close();
		}
	}

	QString index;
	QList<QString>::iterator is;
	int at;
	for (is = modules.begin(), at = 0; is != modules.end(); ++is, ++at)
		if (*is == index)
			topmodule = at;
}

ModulesListModel::~ModulesListModel()
{

}

int ModulesListModel::rowCount(const QModelIndex &) const
{
	return modules.count();
}

QVariant ModulesListModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid() || index.row() >= modules.count())
		return QVariant();

	if (role == Qt::FontRole)
	{
		QFont font;
		if (index.row() != topmodule)
			return font;
		font.setBold(true);
		return font;
	}

	if (role != Qt::DisplayRole)
		return QVariant();

	return modules.at(index.row());
}

QString ModulesListModel::file(const QModelIndex& index)
{
	if (!index.isValid() || index.row() >= files.count())
		return QString();

	return files.at(index.row());
}
