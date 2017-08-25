#include "editortabmanager.h"

EditorTabManager::EditorTabManager(QWidget *parent) :
	QWidget(parent),
	editArea(new QTabWidget(this))
{
	editArea->resize(this->maximumSize());
	editArea->setTabsClosable(true);

	connect(editArea, SIGNAL(tabCloseRequested(int)), this, SLOT(closeFile(int)));
	//connect(editArea, SIGNAL(currentChanged(int)), this, SLOT(showEditDockerWidgets(int)));
}

void EditorTabManager::openFile(QString filepath)
{
	QFileInfo info(filepath);

	for(int idx=0; idx<editArea->count(); idx++) {
		Editor *ed = (Editor *)editArea->widget(idx);
		if(ed->getFilePath()==filepath) return; // already open
	}

	if(isCode(info.suffix())) {
		EditorWidget *editorWidget = new EditorWidget(editArea);
		editorWidget->loadFile(filepath);
		editArea->addTab(editorWidget,info.fileName());
		connect(editorWidget,SIGNAL(textChanged(QString)),this,SLOT(onTextChanged(QString)));
	}
}

void EditorTabManager::onTextChanged(QString f)
{
	EditorWidget *ed;
	for(int idx=0;idx<editArea->count();idx++) {
		ed = (EditorWidget*)editArea->widget(idx);
		if(ed->getStatusChanged()) {
			editArea->setTabText(idx,"Changed");
		}
	}
}

bool EditorTabManager::isCode(QString suffix)
{
	if(suffix=="v") return true;
	if(suffix=="vs") return true;
	return false;
}

bool EditorTabManager::isSchematic(QString suffix)
{
	return false;
}

void EditorTabManager::closeFile(int idx)
{
	EditorWidget *ed = (EditorWidget*)editArea->widget(idx);
	//ed->saveFile();
	editArea->removeTab(idx);
}
