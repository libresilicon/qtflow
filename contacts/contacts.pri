# This was needed to work around "number of sections exceeded object file format limit" linker error
win32:QMAKE_CXXFLAGS += /bigobj

SOURCES += \
	$$PWD/contactplacement.cpp \
    $$PWD/contactsview.cpp \
    $$PWD/padinfo.cpp
 
HEADERS += \
	$$PWD/contactplacement.h \
    $$PWD/contactsview.h \
    $$PWD/padinfo.h

FORMS    += \
	$$PWD/contactplacement.ui
