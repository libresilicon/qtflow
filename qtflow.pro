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

INCLUDEPATH += /usr/include/python3.6m
#INCLUDEPATH += PYTHON_WIN32 (add here windows path when building)

include(PythonQt.prf)
include(PythonQt_QtAll.prf)

QMAKE_CXXFLAGS += -g
QMAKE_CXXFLAGS += -std=c++0x
QMAKE_CXXFLAGS += -lpython3
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
	projectsettings.cpp \
	pythonconsoledockwidget.cpp \
	pysettings.cpp \
	pyprojectsettings.cpp \
	qtreeviewdragdrop.cpp \
	layoutvisibles.cpp \
	projectsitem.cpp \
    generictexteditorwidget.cpp \
    buildflow.cpp

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
	projectsettings.h \
	pythonconsoledockwidget.h \
	pysettings.h \
	pyprojectsettings.h \
	qtreeviewdragdrop.h \
	layoutvisibles.h \
	projectsitem.h \
    generictexteditorwidget.h \
    buildflow.h

FORMS    += \
	mainwindow.ui \
	grid.ui \
	templates.ui \
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
	projectsettings.ui \
	console.ui \
	layoutvisibles.ui \
	partselection.ui \
	librarysettings.ui \
    buildflow.ui

RESOURCES += icons/icons.qrc

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
include(schematics_reader/schematics_reader.pri)
include(magic/magic.pri)
include(magic3d/magic3d.pri)
include(vcd/vcd.pri)
include(vcd_reader/vcd_reader.pri)
include(lef/lef.pri)
include(def/def.pri)
include(tech_reader/tech_reader.pri)
include(cmap_reader/cmap_reader.pri)
include(qlayout/qlayout.pri)
include(qschematics/qschematics.pri)
include(symbol_reader/symbol_reader.pri)

DISTFILES += \
	scripts/blif2cel.py \
	scripts/synthesis.py \
	scripts/simulation.py \
	tech/osu018/osu018.prm \
	tech/osu018/osu018_stdcells.lef \
	tech/osu018/osu018_stdcells.lib \
	tech/osu018/osu018_stdcells.v \
	tech/osu018/osu018.par \
	tech/osu018/osu018_stdcells.sp \
	tech/osu018/SCN6M_SUBM.10.tech \
	tech/osu018/osu018.sh \
	tech/osu018/osu018.magicrc \
	tech/process.xml \
	tech/osu050/SCN3ME_SUBM.30.tech \
	tech/osu050/osu050.prm \
	tech/osu050/osu05_stdcells.lib \
	tech/osu050/osu05_stdcells.v \
	tech/osu050/osu050_stdcells.lef \
	tech/osu050/osu050.par \
	tech/osu050/osu050_stdcells.sp \
	tech/osu050/osu050.magicrc \
	tech/osu050/osu050.sh \
	tech/scmos.tech \
	tech/sxlib/sxlib.lib \
	tech/sxlib/sxlib.alliance.lib \
	tech/sxlib/sxlib.slib \
	tech/sxlib/sxlib.lef \
	tech/generate_schematic_symbols.py \
	tech/mos.dstyle \
	tech/osu035/osu035_stdcells.slib \
	tech/osu035/osu035_stdcells.v \
	tech/osu035/osu035_stdcells.lef \
	tech/osu035/osu035_stdcells.lib \
	tech/osu035/osu035.magicrc \
	tech/osu035/osu035.sh \
	tech/osu035/osu035.prm \
	tech/osu035/osu035.par \
	tech/osu035/SCN4M_SUBM.20.tech \
	tech/osu035/osu035_stdcells.sp \
	tech/tech.qrc \
	tech/gencmap.py \
	tech/mos.cmap

