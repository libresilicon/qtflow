# This was needed to work around "number of sections exceeded object file format limit" linker error
win32:QMAKE_CXXFLAGS += /bigobj

FLEXSOURCES += $$PWD/vcd_scanner.ll
BISONSOURCES += $$PWD/vcd_parser.yy

SOURCES += \
	$$PWD/vcdata.cpp \
	$$PWD/vcdlistmodel.cpp \
	$$PWD/vcdscanner.cpp \
	$$PWD/vcdtreemodel.cpp
 
HEADERS += \
	$$PWD/vcdata.h \
	$$PWD/vcdcommon.h \
	$$PWD/vcderror.h \
	$$PWD/vcdlistmodel.h \
	$$PWD/vcdscanner.h \
	$$PWD/vcdtreemodel.h

