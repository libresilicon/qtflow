#include "qtflowfilelist.h"
#include <QTextStream>

QtFlowFileList::QtFlowFileList(QObject *parent, Project *p) :
	QFileSystemModel(parent),
	project(p)
{
	if(project) setRootPath(project->getRootDir());
}

void QtFlowFileList::refresh()
{
	if(project) setRootPath(project->getSourceDir());

	QRegExp rx("^\\s*module ([a-zA-Z0-9_]+)");
	QDirIterator it(rootPath(), QStringList() << "*.v", QDir::Files, QDirIterator::Subdirectories);
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
					mapFileModule[info.filePath()] = rx.cap(1);
				}
			}
			file.close();
		}
	}
}

QVariant QtFlowFileList::data(const QModelIndex &index, int role) const
{
	QString path = filePath(index);

	if(mapFileModule.contains(path)) {
		if (role == Qt::FontRole)
		{
			QFont font;
			font.setBold(mapFileModule[path]==project->getTopLevel());
			font.setUnderline(mapFileModule[path]==project->getTestBench());

			return font;
		}
	}

	return QFileSystemModel::data(index,role);
}

void QtFlowFileList::setProject(Project *p)
{
	project = p;
	refresh();
}
