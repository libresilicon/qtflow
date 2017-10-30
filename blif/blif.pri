PARSE_OUT_PWD = $$OUT_PWD/blif

# This was needed to work around "number of sections exceeded object file format limit" linker error
win32:QMAKE_CXXFLAGS += /bigobj

SOURCES += \
	$$PWD/blifscanner.cpp \
	$$PWD/blifdata.cpp

HEADERS += \
	$$PWD/blifscanner.h \
	$$PWD/blifdata.h
