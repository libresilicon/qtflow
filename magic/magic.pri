# This was needed to work around "number of sections exceeded object file format limit" linker error
win32:QMAKE_CXXFLAGS += /bigobj

FLEXSOURCES += $$PWD/magic_scanner.ll
BISONSOURCES += $$PWD/magic_parser.yy

SOURCES += \
	$$PWD/magicdata.cpp \
	$$PWD/magiclayouteditor.cpp \
	$$PWD/magiclayouteditorwidget.cpp \
	$$PWD/magicscanner.cpp \
    $$PWD/qgraphicswireitem.cpp
 
HEADERS += \
	$$PWD/magicdata.h \
	$$PWD/magiclayouteditor.h \
	$$PWD/magiclayouteditorwidget.h \
	$$PWD/magicscanner.h \
    $$PWD/qgraphicswireitem.h

