#-------------------------------------------------
#
# Project created by QtCreator 2017-06-25T15:09:44
#
#-------------------------------------------------

QT += core
QT += gui
QT += script
QT += xml
QT += widgets

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

QMAKE_CXXFLAGS += -g
QMAKE_CXXFLAGS += -std=c++0x
QMAKE_CXXFLAGS += -lpython2.7
QMAKE_CXXFLAGS += -lPythonQt

FLEXSOURCES = \
	magic_scanner.ll \
	vcd_scanner.ll \
	schematics_scanner.ll \
	lef_scanner.ll \
	def_scanner.ll

BISONSOURCES = \
	magic_parser.yy \
	vcd_parser.yy \
	schematics_parser.yy \
	lef_parser.yy \
	def_parser.yy

OTHER_FILES +=  \
        $$FLEXSOURCES \
        $$BISONSOURCES

SOURCES += \
	main.cpp\
	maintoolbox.cpp\
	mainwindow.cpp \
	grid.cpp \
	templates.cpp \
	project.cpp \
	edit.cpp \
	editor.cpp \
	verilog.cpp \
	savechanges.cpp \
	welcome.cpp \
	iopads.cpp \
	dependencies.cpp \
	projectstreemodel.cpp \
	modulestreemodel.cpp \
	moduleslistmodel.cpp \
	new.cpp \
	wave.cpp \
	vcdtreemodel.cpp \
	vcdlistmodel.cpp \
	headlessmainapp.cpp \
	settings.cpp \
	fileselector.cpp \
	projectselector.cpp \
	moduleselector.cpp \
	editorwidget.cpp \
	editortabmanager.cpp \
	codeeditorwidget.cpp \
	schematicseditorwidget.cpp \
	schematicseditor.cpp \
        vcdscanner.cpp \
        magicscanner.cpp \
        magicdata.cpp \
        vcdata.cpp \
        schematicsdata.cpp \
        schematicsscanner.cpp \
	magiclayouteditorwidget.cpp \
	magiclayouteditor.cpp \
	deflayouteditorwidget.cpp \
	deflayouteditor.cpp \
	qtflowfilelist.cpp \
	testbenchtoolbox.cpp \
	synthesistoolbox.cpp \
	projectsettings.cpp \
        lefscanner.cpp \
        lefdata.cpp \
        defscanner.cpp \
        defdata.cpp \
	NicePyConsole.cpp \
	SimpleConsole.cpp \
	PythonCompleter.cpp \
	PythonCompleterPopup.cpp \
	PygmentsHighlighter.cpp \
    pythonconsolewidget.cpp \
    pythonconsoledockwidget.cpp

HEADERS  += \
	mainwindow.h \
	maintoolbox.h \
	grid.h \
	templates.h \
	project.h \
	edit.h \
	editor.h \
	verilog.h \
	savechanges.h \
	welcome.h \
	iopads.h \
	dependencies.h \
	projectstreemodel.h \
	modulestreemodel.h \
	moduleslistmodel.h \
	new.h \
	wave.h \
	vcdtreemodel.h \
	vcdlistmodel.h \
	headlessmainapp.h \
	settings.h \
	fileselector.h \
	projectselector.h \
	moduleselector.h \
	editorwidget.h \
	editortabmanager.h \
	codeeditorwidget.h \
	schematicseditorwidget.h \
	schematicseditor.h \
	ieditor.h \
        vcdscanner.h \
        magicscanner.h \
        magicdata.h \
        vcdata.h \
        schematicsdata.h \
        schematicsscanner.h \
        vcdcommon.h \
	magiclayouteditorwidget.h \
	magiclayouteditor.h \
	deflayouteditorwidget.h \
	deflayouteditor.h \
	qtflowfilelist.h \
	testbenchtoolbox.h \
	synthesistoolbox.h \
	projectsettings.h \
        lefscanner.h \
        lefdata.h \
	defdata.h \
	defscanner.h \
	NicePyConsole.h \
	SimpleConsole.h \
	PythonCompleter.h \
	PythonCompleterPopup.h \
	PygmentsHighlighter.h \
    pythonconsolewidget.h \
    pythonconsoledockwidget.h

FORMS    += \ 
	mainwindow.ui \
	grid.ui \
	templates.ui \
	environment.ui \
	edit.ui \
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
    console.ui

flexsource.input = FLEXSOURCES
flexsource.output = ${QMAKE_FILE_BASE}.cpp
flexsource.commands = flex++ --header-file=${QMAKE_FILE_BASE}.h -o ${QMAKE_FILE_BASE}.cpp ${QMAKE_FILE_IN}
flexsource.variable_out = SOURCES
flexsource.name = Flex Sources ${QMAKE_FILE_IN}
flexsource.CONFIG += target_predeps

QMAKE_EXTRA_COMPILERS += flexsource

flexheader.input = FLEXSOURCES
flexheader.output = ${QMAKE_FILE_BASE}.h
flexheader.commands = @true
flexheader.variable_out = HEADERS
flexheader.name = Flex Headers ${QMAKE_FILE_IN}
flexheader.CONFIG += target_predeps no_link

QMAKE_EXTRA_COMPILERS += flexheader

bisonsource.input = BISONSOURCES
bisonsource.output = ${QMAKE_FILE_BASE}.cpp
bisonsource.commands = bison -Lc++ -d --defines=${QMAKE_FILE_BASE}.h -o ${QMAKE_FILE_BASE}.cpp ${QMAKE_FILE_IN}
bisonsource.variable_out = SOURCES
bisonsource.name = Bison Sources ${QMAKE_FILE_IN}
bisonsource.CONFIG += target_predeps

QMAKE_EXTRA_COMPILERS += bisonsource

bisonheader.input = BISONSOURCES
bisonheader.output = ${QMAKE_FILE_BASE}.h
bisonheader.commands = @true
bisonheader.variable_out = HEADERS
bisonheader.name = Bison Headers ${QMAKE_FILE_IN}
bisonheader.CONFIG += target_predeps no_link

QMAKE_EXTRA_COMPILERS += bisonheader

RESOURCES = qtflow.qrc

include(PythonQt/src/src.pri)
include(PythonQt/generated_cpp_56/com_trolltech_qt_core_builtin/com_trolltech_qt_core_builtin.pri)
include(PythonQt/generated_cpp_56/com_trolltech_qt_gui_builtin/com_trolltech_qt_gui_builtin.pri)
