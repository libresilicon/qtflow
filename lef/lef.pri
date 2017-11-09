# This was needed to work around "number of sections exceeded object file format limit" linker error
win32:QMAKE_CXXFLAGS += /bigobj

FLEXSOURCES += $$PWD/lef_scanner.ll
BISONSOURCES += $$PWD/lef_parser.yy

SOURCES += \
	$$PWD/lefdata.cpp \
	$$PWD/lefscanner.cpp \
    $$PWD/lefpin.cpp \
    $$PWD/lefport.cpp \
    $$PWD/leflayer.cpp \
    $$PWD/lefmacro.cpp \
    $$PWD/lefobstruction.cpp \
    $$PWD/leflayerinfo.cpp \
    $$PWD/lefvia.cpp
 
HEADERS += \
	$$PWD/lefdata.h \
	$$PWD/lefscanner.h \
    $$PWD/lefpin.h \
    $$PWD/lefport.h \
    $$PWD/leflayer.h \
    $$PWD/lefmacro.h \
    $$PWD/lefobstruction.h \
    $$PWD/leflayerinfo.h \
    $$PWD/lefvia.h
