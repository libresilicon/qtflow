# This was needed to work around "number of sections exceeded object file format limit" linker error
win32:QMAKE_CXXFLAGS += /bigobj

FLEXSOURCES += $$PWD/tech_scanner.ll
BISONSOURCES += $$PWD/tech_parser.yy

SOURCES += \
	$$PWD/techdata.cpp \
	$$PWD/techscanner.cpp \
    $$PWD/techdesignrule.cpp \
    $$PWD/techdesignrulespacing.cpp
 
HEADERS += \
	$$PWD/techdata.h \
	$$PWD/techscanner.h \
    $$PWD/techdesignrule.h \
    $$PWD/techdesignrulespacing.h

