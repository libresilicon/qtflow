#ifndef QTFLOWFILELIST_H
#define QTFLOWFILELIST_H

#include <QWidget>
#include <QFileSystemModel>

class QtFlowFileList : public QFileSystemModel
{
	Q_OBJECT

public:
	QtFlowFileList();
};

#endif // QTFLOWFILELIST_H
