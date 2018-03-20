#ifndef DEFLAYOUTEDITORWIDGET_H
#define DEFLAYOUTEDITORWIDGET_H

#include "deflayouteditor.h"
#include "qlayout/genericlayouteditorwidget.h"

class DEFLayoutEditorWidget : public GenericLayoutEditorWidget
{
	Q_OBJECT
public:
	DEFLayoutEditorWidget(QWidget *parent = nullptr);
};

#endif // DEFLAYOUTEDITORWIDGET_H
