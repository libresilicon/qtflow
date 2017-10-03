#-------------------------------------------------
#
# Project created by QtCreator 2017-06-25T15:09:44
#
#-------------------------------------------------

QT += core
QT += gui
QT += xml
QT += widgets
QT += opengl
QT += KTextEditor

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qtflow
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += /usr/include/python2.7
#INCLUDEPATH += PYTHON_WIN32 (add here windows path when building)

include(PythonQt.prf)
include(PythonQt_QtAll.prf)

QMAKE_CXXFLAGS += -g
QMAKE_CXXFLAGS += -std=c++0x
QMAKE_CXXFLAGS += -lpython2.7
QMAKE_CXXFLAGS += -lPythonQt
QMAKE_CXXFLAGS += -lPythonQt_QtAll

OTHER_FILES +=  \
$$FLEXSOURCES \
$$BISONSOURCES

SOURCES += \
	main.cpp\
	mainwindow.cpp \
	grid.cpp \
	templates.cpp \
	project.cpp \
	savechanges.cpp \
	welcome.cpp \
	iopads.cpp \
	projectstreemodel.cpp \
	modulestreemodel.cpp \
	moduleslistmodel.cpp \
	new.cpp \
	wave.cpp \
	headlessmainapp.cpp \
	settings.cpp \
	fileselector.cpp \
	projectselector.cpp \
	moduleselector.cpp \
	editorwidget.cpp \
	editortabmanager.cpp \
	codeeditorwidget.cpp \
	qtflowfilelist.cpp \
	testbenchtoolbox.cpp \
	synthesistoolbox.cpp \
	projectsettings.cpp \
	pythonconsoledockwidget.cpp \
	pysettings.cpp \
	pyprojectsettings.cpp \
	qtreeviewdragdrop.cpp \
	layoutvisibles.cpp

HEADERS  += \
	mainwindow.h \
	grid.h \
	templates.h \
	project.h \
	savechanges.h \
	welcome.h \
	iopads.h \
	projectstreemodel.h \
	modulestreemodel.h \
	moduleslistmodel.h \
	new.h \
	wave.h \
	headlessmainapp.h \
	settings.h \
	fileselector.h \
	projectselector.h \
	moduleselector.h \
	editorwidget.h \
	editortabmanager.h \
	codeeditorwidget.h \
	ieditor.h \
	qtflowfilelist.h \
	testbenchtoolbox.h \
	synthesistoolbox.h \
	projectsettings.h \
	pythonconsoledockwidget.h \
	pysettings.h \
	pyprojectsettings.h \
	qtreeviewdragdrop.h \
	layoutvisibles.h

FORMS    += \
	mainwindow.ui \
	grid.ui \
	templates.ui \
	environment.ui \
	savechanges.ui \
	welcome.ui \
	iopads.ui \
	modules.ui \
	new.ui \
	mainwindow.ui \
	settings.ui \
	modules.ui \
	projects.ui \
	files.ui \
	wave.ui \
	testbenchtoolbox.ui \
	synthesistoolbox.ui \
	projectsettings.ui \
	console.ui \
	schematicstoolbox.ui \
	layoutvisibles.ui

RESOURCES += scripts/scripts.qrc
RESOURCES += icons/icons.qrc
RESOURCES += tech/tech.qrc

include(PythonQt/src/src.pri)
include(PythonQt/generated_cpp_56/com_trolltech_qt_core_builtin/com_trolltech_qt_core_builtin.pri)
include(PythonQt/generated_cpp_56/com_trolltech_qt_gui_builtin/com_trolltech_qt_gui_builtin.pri)

#flex definition
flex.name = Flex
flex.input = FLEXSOURCES
flex.output = $${OUT_PWD}/${QMAKE_FILE_BASE}/${QMAKE_FILE_BASE}.cpp
flex.commands = flex++ --header-file=$${OUT_PWD}/${QMAKE_FILE_BASE}/${QMAKE_FILE_BASE}.h -o ${QMAKE_FILE_OUT} ${QMAKE_FILE_IN}
flex.variable_out = SOURCES
flexsource.CONFIG += target_predeps
QMAKE_EXTRA_COMPILERS += flex

#bison definition
bison.name = Bison
bison.input = BISONSOURCES
bison.output = $${OUT_PWD}/${QMAKE_FILE_BASE}/${QMAKE_FILE_BASE}.cpp
bison.commands = bison -Lc++ --defines=$${OUT_PWD}/${QMAKE_FILE_BASE}/${QMAKE_FILE_BASE}.h -o ${QMAKE_FILE_OUT} ${QMAKE_FILE_IN}
bison.variable_out = SOURCES
bison.CONFIG += target_predeps
QMAKE_EXTRA_COMPILERS += bison

include(schematics/schematics.pri)
include(magic/magic.pri)
include(magic3d/magic3d.pri)
include(vcd/vcd.pri)
include(lef/lef.pri)
include(def/def.pri)
include(tech_reader/tech_reader.pri)
include(cmap_reader/cmap_reader.pri)
include(qlayout/qlayout.pri)
