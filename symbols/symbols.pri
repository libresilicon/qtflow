# This was needed to work around "number of sections exceeded object file format limit" linker error
win32:QMAKE_CXXFLAGS += /bigobj

SOURCES += \ 
    $$PWD/symboleditorwidget.cpp \
    $$PWD/symboleditor.cpp \
    $$PWD/symboleditorscene.cpp

HEADERS += \ 
    $$PWD/symboleditorwidget.h \
    $$PWD/symboleditor.h \
    $$PWD/symboleditorscene.h

