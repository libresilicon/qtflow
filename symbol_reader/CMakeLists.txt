set(TARGET "SymbolReader")

set (CMAKE_AUTOMOC ON)
set (CMAKE_INCLUDE_CURRENT_DIR ON)

BISON_TARGET(SymbolParser symbol_parser.yy ${CMAKE_CURRENT_BINARY_DIR}/symbol_parser.cpp)
FLEX_TARGET(SymbolScanner symbol_scanner.ll ${CMAKE_CURRENT_BINARY_DIR}/symbol_scanner.cpp)
ADD_FLEX_BISON_DEPENDENCY(SymbolScanner SymbolParser)

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/..
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
)

set(SOURCES
	schematicssymbol.cpp
	symboldata.cpp
	symbolpin.cpp
	symbolscanner.cpp
)

add_library(${TARGET} SHARED
	${BISON_SymbolParser_OUTPUTS}
	${FLEX_SymbolScanner_OUTPUTS}
	${SOURCES}
)

target_link_libraries(${TARGET} ${QT5_LIBRARIES})
install(TARGETS ${TARGET} LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})


