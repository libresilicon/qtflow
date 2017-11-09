# This was needed to work around "number of sections exceeded object file format limit" linker error
win32:QMAKE_CXXFLAGS += /bigobj

SOURCES += \
	$$PWD/qlayoutwireitem.cpp \
	$$PWD/qlayoutmacroitem.cpp \
	$$PWD/qlayoutrectitem.cpp \
	$$PWD/qlayoutscene.cpp \
	$$PWD/layoutvisibles.cpp \
    $$PWD/drcsettings.cpp \
    $$PWD/qlayoutdistancemeasure.cpp \
    $$PWD/qlayoutmacropinitem.cpp \
    $$PWD/qlayoutviaitem.cpp

HEADERS += \
	$$PWD/qlayoutwireitem.h \
	$$PWD/qlayoutmacroitem.h \
	$$PWD/qlayoutrectitem.h \
	$$PWD/qlayoutscene.h \
	$$PWD/layoutvisibles.h \
    $$PWD/drcsettings.h \
    $$PWD/qlayoutdistancemeasure.h \
    $$PWD/qlayoutmacropinitem.h \
    $$PWD/qlayoutviaitem.h

FORMS    += \
	$$PWD/layoutvisibles.ui \
    $$PWD/drcsettings.ui

