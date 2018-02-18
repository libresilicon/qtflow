#-------------------------------------------------
#
# Project created by QtCreator 2018-02-19T00:26:41
#
#-------------------------------------------------

QT += core
QT += gui
QT += xml
QT += widgets
QT += opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtDEF
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

FLEXSOURCES += def_scanner.ll
BISONSOURCES += def_parser.yy

SOURCES += \
	defdata.cpp \
	deflayouteditor.cpp \
	deflayouteditorwidget.cpp \
	defscanner.cpp \
    	defmoduleinfo.cpp \
    	defdatapin.cpp \
    	main.cpp \
       	qtdef.cpp \
	defrouteinfo.cpp
 
HEADERS += \
	defdata.h \
	deflayouteditor.h \
	deflayouteditorwidget.h \
	defscanner.h \
	defmoduleinfo.h \
	defdatapin.h \
	defrouteinfo.h \
        qtdef.h

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

