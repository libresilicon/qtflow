set(TARGET "VCDReader")

set (CMAKE_AUTOMOC ON)
set (CMAKE_INCLUDE_CURRENT_DIR ON)

BISON_TARGET(VCDParser vcd_parser.yy ${CMAKE_CURRENT_BINARY_DIR}/vcd_parser.cpp)
FLEX_TARGET(VCDScanner vcd_scanner.ll ${CMAKE_CURRENT_BINARY_DIR}/vcd_scanner.cpp)
ADD_FLEX_BISON_DEPENDENCY(VCDScanner VCDParser)

include_directories(
	"${CMAKE_CURRENT_SOURCE_DIR}/.."
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
)

set(SOURCES
	vcd_data.cpp
	vcd_loader.cpp
)

add_library(${TARGET} SHARED
	${BISON_VCDParser_OUTPUTS}
	${FLEX_VCDScanner_OUTPUTS}
	${SOURCES}
)

target_link_libraries(${TARGET} ${QT5_LIBRARIES})


