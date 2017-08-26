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
	EditorWidget *ed;
	QFileInfo info(filepath);

	for(int idx=0; idx < editArea->count(); idx++) {
		ed = (EditorWidget *)editArea->widget(idx);
		if(ed->getFilePath()==filepath) return; // already open
	}

	if(isCode(info.suffix())) {
		QTextStream(stdout) << filepath << '\n';
		CodeEditorWidget *editorWidget = new CodeEditorWidget(editArea);
		editorWidget->loadFile(filepath);
		editArea->addTab(editorWidget,info.fileName());
		connect(editorWidget, SIGNAL(contentChanged()), this, SLOT(onContentChanged()));
		connect(editorWidget, SIGNAL(contentSaved()), this, SLOT(onContentSaved()));
	}
	if(isSchematic(info.suffix())) {
		SchematicsEditorWidget *editorWidget = new SchematicsEditorWidget(editArea);
		editorWidget->loadFile(filepath);
		editArea->addTab(editorWidget,info.fileName());
		connect(editorWidget, SIGNAL(contentChanged()), this, SLOT(onContentChanged()));
		connect(editorWidget, SIGNAL(contentSaved()), this, SLOT(onContentSaved()));
	}
	if(isLayout(info.suffix())) {
	}
}

void EditorTabManager::onContentSaved()
{
	EditorWidget *ed;
	for(int idx=0; idx<editArea->count(); idx++) {
		ed = (EditorWidget*)editArea->widget(idx);
		if(ed->getStatusChanged()) {
			editArea->setTabText(idx, QFileInfo(ed->getFilePath()).fileName()+"*");
		} else {
			editArea->setTabText(idx, QFileInfo(ed->getFilePath()).fileName());
		}
	}
	emit(fileSaved());
}

void EditorTabManager::onContentChanged()
{
	EditorWidget *ed;
	for(int idx=0; idx<editArea->count(); idx++) {
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

bool EditorTabManager::isLayout(QString suffix)
{
	if(suffix=="mag") return true;
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
