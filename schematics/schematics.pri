
PARSE_OUT_PWD = $$OUT_PWD/schematics

INCLUDEPATH += $${PARSE_OUT_PWD}

# This was needed to work around "number of sections exceeded object file format limit" linker error
win32:QMAKE_CXXFLAGS += /bigobj

SOURCES += \
	$$PWD/schematicseditor.cpp \
	$$PWD/schematicseditorwidget.cpp \
	$$PWD/schematicswire.cpp \
	$$PWD/schematicspart.cpp \
	$$PWD/schematicspartselection.cpp \
	$$PWD/schematicslibraryeditor.cpp \
    $$PWD/schematicsparametricpartdialog.cpp
 
HEADERS += \
	$$PWD/schematicseditor.h \
	$$PWD/schematicseditorwidget.h \
	$$PWD/schematicswire.h \
	$$PWD/schematicspart.h \
	$$PWD/schematicspartselection.h \
	$$PWD/schematicslibraryeditor.h \
    $$PWD/schematicsparametricpartdialog.h
