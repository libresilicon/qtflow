
PARSE_OUT_PWD = $$OUT_PWD/lef

# This was needed to work around "number of sections exceeded object file format limit" linker error
win32:QMAKE_CXXFLAGS += /bigobj

LEXSOURCES += $$PWD/lef_scanner.ll
BISONSOURCES += $$PWD/lef_parser.yy

SOURCES += \
	$$PWD/lefdata.cpp \
	$$PWD/lefscanner.cpp
 
HEADERS += \
	$$PWD/lefdata.h \
	$$PWD/lefscanner.h
