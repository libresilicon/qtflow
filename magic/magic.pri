
PARSE_OUT_PWD = $$OUT_PWD/magic

# This was needed to work around "number of sections exceeded object file format limit" linker error
win32:QMAKE_CXXFLAGS += /bigobj

FLEXSOURCES = $$PWD/magic_scanner.ll
BISONSOURCES = $$PWD/magic_parser.yy

SOURCES += \
	$$PWD/magicdata.cpp \
	$$PWD/magiclayouteditor.cpp \
	$$PWD/magiclayouteditorwidget.cpp \
	$$PWD/magicscanner.cpp
 
HEADERS += \
	$$PWD/magicdata.h \
	$$PWD/magiclayouteditor.h \
	$$PWD/magiclayouteditorwidget.h \
	$$PWD/magicscanner.h

flexsource.input = FLEXSOURCES
flexsource.output = $${PARSE_OUT_PWD}/${QMAKE_FILE_BASE}.cpp
flexsource.commands = flex++ --header-file=$${PARSE_OUT_PWD}/${QMAKE_FILE_BASE}.h -o $${PARSE_OUT_PWD}/${QMAKE_FILE_BASE}.cpp ${QMAKE_FILE_IN}
flexsource.variable_out = SOURCES
flexsource.name = Flex Sources ${QMAKE_FILE_IN}
flexsource.CONFIG += target_predeps

QMAKE_EXTRA_COMPILERS += flexsource

flexheader.input = FLEXSOURCES
flexheader.output = $${PARSE_OUT_PWD}/${QMAKE_FILE_BASE}.h
flexheader.commands = @true
flexheader.variable_out = HEADERS
flexheader.name = Flex Headers ${QMAKE_FILE_IN}
flexheader.CONFIG += target_predeps no_link

QMAKE_EXTRA_COMPILERS += flexheader

bisonsource.input = BISONSOURCES
bisonsource.output = $${PARSE_OUT_PWD}/${QMAKE_FILE_BASE}.cpp
bisonsource.commands = bison -Lc++ -d --defines=$${PARSE_OUT_PWD}/${QMAKE_FILE_BASE}.h -o $${PARSE_OUT_PWD}/${QMAKE_FILE_BASE}.cpp ${QMAKE_FILE_IN}
bisonsource.variable_out = SOURCES
bisonsource.name = Bison Sources ${QMAKE_FILE_IN}
bisonsource.CONFIG += target_predeps

QMAKE_EXTRA_COMPILERS += bisonsource

bisonheader.input = BISONSOURCES
bisonheader.output = $${PARSE_OUT_PWD}/${QMAKE_FILE_BASE}.h
bisonheader.commands = @true
bisonheader.variable_out = HEADERS
bisonheader.name = Bison Headers ${QMAKE_FILE_IN}
bisonheader.CONFIG += target_predeps no_link

QMAKE_EXTRA_COMPILERS += bisonheader

