# This was needed to work around "number of sections exceeded object file format limit" linker error
win32:QMAKE_CXXFLAGS += /bigobj

FLEXSOURCES += $$PWD/vcd_scanner.ll
BISONSOURCES += $$PWD/vcd_parser.yy

SOURCES += \
	$$PWD/vcd_data.cpp \
	$$PWD/vcdlistmodel.cpp \
	$$PWD/vcd_loader.cpp \
	$$PWD/vcdtreemodel.cpp

HEADERS += \
	$$PWD/vcdlistmodel.h \
	$$PWD/vcd_loader.h \
	$$PWD/vcdscanner.h \
	$$PWD/vcdtreemodel.h \
	$$PWD/vcd_data_fwd.hpp \
	$$PWD/vcd_data.hpp \
	$$PWD/vcd_error.hpp \
	$$PWD/vcd_lexer.hpp \
	$$PWD/vcd_loader.hpp \
	$$PWD/vcdparse.hpp
