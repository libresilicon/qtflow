set(TARGET "LEF")

set (CMAKE_AUTOMOC ON)
set (CMAKE_INCLUDE_CURRENT_DIR ON)

BISON_TARGET(LEFParser lef_parser.yy ${CMAKE_CURRENT_BINARY_DIR}/lef_parser.cpp)
FLEX_TARGET(LEFScanner lef_scanner.ll ${CMAKE_CURRENT_BINARY_DIR}/lef_scanner.cpp)
ADD_FLEX_BISON_DEPENDENCY(LEFScanner LEFParser)

include_directories(
	"${CMAKE_CURRENT_SOURCE_DIR}/.."
	${CMAKE_CURRENT_SOURCE_DIR}
	"${CMAKE_CURRENT_BINARY_DIR}/../qlayout"
	${CMAKE_CURRENT_BINARY_DIR}
)

set(SOURCES
	lefdata.cpp
	leflayer.cpp
	leflayerinfo.cpp
	lefmacro.cpp
	lefobstruction.cpp
	lefpin.cpp
	lefport.cpp
	lefscanner.cpp
	lefvia.cpp
)

add_library(${TARGET} SHARED
	${BISON_LEFParser_OUTPUTS}
	${FLEX_LEFScanner_OUTPUTS}
	${SOURCES}
)

target_link_libraries(${TARGET} ${QT5_LIBRARIES})

install(TARGETS ${TARGET} LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
