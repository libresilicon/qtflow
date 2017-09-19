# This was needed to work around "number of sections exceeded object file format limit" linker error
win32:QMAKE_CXXFLAGS += /bigobj

#FLEXSOURCES += $$PWD/magic_scanner.ll
#BISONSOURCES += $$PWD/magic_parser.yy

#SOURCES += \
#	$$PWD/magicscanner.cpp
 
#HEADERS += \
#	$$PWD/magicscanner.h


FORMS    += \
	$$PWD/magic3d.ui

HEADERS += \
    $$PWD/magic3d.h

SOURCES += \
    $$PWD/magic3d.cpp
