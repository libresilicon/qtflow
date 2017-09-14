
PARSE_OUT_PWD = $$OUT_PWD/schematics

INCLUDEPATH += $${PARSE_OUT_PWD}

# This was needed to work around "number of sections exceeded object file format limit" linker error
win32:QMAKE_CXXFLAGS += /bigobj

FLEXSOURCES += $$PWD/schematics_scanner.ll
BISONSOURCES += $$PWD/schematics_parser.yy

SOURCES += \
	$$PWD/schematicsdata.cpp \
	$$PWD/schematicseditor.cpp \
	$$PWD/schematicseditorwidget.cpp \
	$$PWD/schematicsscanner.cpp
 
HEADERS += \
	$$PWD/schematicsdata.h \
	$$PWD/schematicseditor.h \
	$$PWD/schematicseditorwidget.h \
	$$PWD/schematicsscanner.h
