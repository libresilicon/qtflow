#include "editortabmanager.h"

#include <QLabel>

EditorTabManager::EditorTabManager(QWidget *parent) :
	QTabWidget(parent),
	project(NULL),
	visibles(NULL)
{
	setTabsClosable(true);
	parent->layout()->addWidget(this);
	connect(this, SIGNAL(tabCloseRequested(int)), this, SLOT(closeFile(int)));
}

void EditorTabManager::openFile(QString filepath)
{
	EditorWidget *ed;
	QFileInfo info(filepath);

	for(int idx=0; idx < count(); idx++) {
		ed = (EditorWidget *)widget(idx);
		if(ed->getFilePath()==filepath) return; // already open
	}

	if(isCode(info.suffix())) {
		CodeEditorWidget *editorWidget = new CodeEditorWidget(this);
		editorWidget->loadFile(filepath);
		addTab(editorWidget,info.fileName());
		connect(editorWidget, SIGNAL(contentChanged()), this, SLOT(onContentChanged()));
		connect(editorWidget, SIGNAL(contentSaved()), this, SLOT(onContentSaved()));
	} else if(isSchematic(info.suffix())) {
		SchematicsEditorWidget *editorWidget = new SchematicsEditorWidget(this);
		editorWidget->loadFile(filepath);
		addTab(editorWidget,info.fileName());
		connect(editorWidget, SIGNAL(contentChanged()), this, SLOT(onContentChanged()));
		connect(editorWidget, SIGNAL(contentSaved()), this, SLOT(onContentSaved()));
	} else if(isLayout(info.suffix())) {
		if(info.suffix()=="mag") {
			MagicLayoutEditorWidget *editorWidget = new MagicLayoutEditorWidget(this);
			editorWidget->setProject(project);
			editorWidget->loadFile(filepath);
			addTab(editorWidget,info.fileName());
			connect(editorWidget, SIGNAL(contentChanged()), this, SLOT(onContentChanged()));
			connect(editorWidget, SIGNAL(contentSaved()), this, SLOT(onContentSaved()));
		} else if(info.suffix()=="def") {
			DEFLayoutEditorWidget *editorWidget = new DEFLayoutEditorWidget(this);
			editorWidget->loadFile(filepath);
			addTab(editorWidget,info.fileName());
			connect(editorWidget, SIGNAL(contentChanged()), this, SLOT(onContentChanged()));
			connect(editorWidget, SIGNAL(contentSaved()), this, SLOT(onContentSaved()));
		}
	}
}

void EditorTabManager::onContentSaved()
{
	EditorWidget *ed;
	for(int idx=0; idx < count(); idx++) {
		ed = (EditorWidget*)widget(idx);
		if(ed->getStatusChanged()) {
			setTabText(idx, QFileInfo(ed->getFilePath()).fileName()+"*");
		} else {
			setTabText(idx, QFileInfo(ed->getFilePath()).fileName());
		}
	}
	emit(fileSaved());
}

void EditorTabManager::onContentChanged()
{
	qDebug() << __FUNCTION__;
	EditorWidget *ed;
	for(int idx=0; idx < count(); idx++) {
		ed = (EditorWidget*)widget(idx);
		if(ed->getStatusChanged()) {
			setTabText(idx, QFileInfo(ed->getFilePath()).fileName()+"*");
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
	if(suffix=="def") return true;
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
	EditorWidget *ed = (EditorWidget*)widget(idx);
	ed->closeFile();
	removeTab(idx);
}

void EditorTabManager::setProject(Project *p)
{
	project = p;
}

void EditorTabManager::setVisibles(LayoutVisibles *v)
{
	visibles = v;
}
