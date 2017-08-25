#include "editortabmanager.h"

EditorTabManager::EditorTabManager(QWidget *parent) :
	QWidget(parent),
	editArea(new QTabWidget(this))
{
	editArea->resize(this->maximumSize());
	editArea->setTabsClosable(true);

	connect(editArea, SIGNAL(tabCloseRequested(int)), this, SLOT(closeFile(int)));
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
		connect(editorWidget, SIGNAL(textChanged()), this, SLOT(onTextChanged()));
		connect(editorWidget, SIGNAL(textSaved()), this, SLOT(onTextSaved()));
	}
}

void EditorTabManager::onTextSaved()
{
	EditorWidget *ed;
	for(int idx=0;idx<editArea->count();idx++) {
		ed = (EditorWidget*)editArea->widget(idx);
		if(ed->getStatusChanged()) {
			editArea->setTabText(idx, QFileInfo(ed->getFilePath()).fileName()+"*");
		} else {
			editArea->setTabText(idx, QFileInfo(ed->getFilePath()).fileName());
		}
	}
}

void EditorTabManager::onTextChanged()
{
	EditorWidget *ed;
	for(int idx=0;idx<editArea->count();idx++) {
		ed = (EditorWidget*)editArea->widget(idx);
		if(ed->getStatusChanged()) {
			editArea->setTabText(idx, QFileInfo(ed->getFilePath()).fileName()+"*");
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
	if(suffix=="sch") return true;
	if(suffix=="sym") return true;
	return false;
}

void EditorTabManager::closeFile(int idx)
{
	editArea->removeTab(idx);
}
