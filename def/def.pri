
PARSE_OUT_PWD = $$OUT_PWD/def

# This was needed to work around "number of sections exceeded object file format limit" linker error
win32:QMAKE_CXXFLAGS += /bigobj

LEXSOURCES += $$PWD/def_scanner.ll
YACCSOURCES += $$PWD/def_parser.yy

SOURCES += \
	$$PWD/defdata.cpp \
	$$PWD/deflayouteditor.cpp \
	$$PWD/deflayouteditorwidget.cpp \
	$$PWD/defscanner.cpp
 
HEADERS += \
	$$PWD/defdata.h \
	$$PWD/deflayouteditor.h \
	$$PWD/deflayouteditorwidget.h \
	$$PWD/defscanner.h


