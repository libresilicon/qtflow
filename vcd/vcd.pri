# This was needed to work around "number of sections exceeded object file format limit" linker error
win32:QMAKE_CXXFLAGS += /bigobj

SOURCES += \
	$$PWD/vcdtreemodel.cpp \
	$$PWD/vcdtreeitem.cpp \
	$$PWD/vcdsignaltreemodel.cpp \
	$$PWD/vcdsignaltreeitem.cpp \
	$$PWD/vcdsignalviewtreemodel.cpp \
	$$PWD/vcdsignalviewtreeitem.cpp \
	$$PWD/vcdsignalview.cpp \
    $$PWD/vcdviewgraphicsitemsignal.cpp

HEADERS += \
	$$PWD/vcdtreemodel.h \
	$$PWD/vcdtreeitem.h \
	$$PWD/vcdsignaltreemodel.h \
	$$PWD/vcdsignaltreeitem.h \
	$$PWD/vcdsignalviewtreemodel.h \
	$$PWD/vcdsignalviewtreeitem.h \
	$$PWD/vcdsignalview.h \
    $$PWD/vcdviewgraphicsitemsignal.h
