# This was needed to work around "number of sections exceeded object file format limit" linker error
win32:QMAKE_CXXFLAGS += /bigobj

SOURCES += \
    $$PWD/qschematicsscene.cpp

HEADERS += \
    $$PWD/qschematicsscene.h

