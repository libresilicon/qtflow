set(TARGET "TechReader")

BISON_TARGET(TechParser tech_parser.yy ${CMAKE_CURRENT_BINARY_DIR}/tech_parser.cpp)
FLEX_TARGET(TechScanner tech_scanner.ll ${CMAKE_CURRENT_BINARY_DIR}/tech_scanner.cpp)
ADD_FLEX_BISON_DEPENDENCY(TechScanner TechParser)

include_directories(
	"${CMAKE_CURRENT_SOURCE_DIR}/.."
	"${CMAKE_CURRENT_SOURCE_DIR}"
	"${CMAKE_CURRENT_BINARY_DIR}"
)

set(SOURCES
	techdata.cpp
	techdesignrule.cpp
	techdesignrulespacing.cpp
	techscanner.cpp
)

add_library(${TARGET} SHARED
	${SOURCES}
	${BISON_TechParser_OUTPUTS}
	${FLEX_TechScanner_OUTPUTS}
)

target_link_libraries(${TARGET} ${QT5_LIBRARIES})
