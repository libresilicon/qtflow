# This was needed to work around "number of sections exceeded object file format limit" linker error
win32:QMAKE_CXXFLAGS += /bigobj

SOURCES += \
    $$PWD/qschematicsscene.cpp \
    $$PWD/qschematicsparametricpart.cpp \
    $$PWD/qschematicswire.cpp \
    $$PWD/qschematicspart.cpp \
    $$PWD/qschematicspin.cpp

HEADERS += \
    $$PWD/qschematicsscene.h \
    $$PWD/qschematicsparametricpart.h \
    $$PWD/qschematicswire.h \
    $$PWD/qschematicspart.h \
    $$PWD/qschematicspin.h

