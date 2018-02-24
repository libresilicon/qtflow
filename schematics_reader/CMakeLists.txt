set(TARGET "SchematicsReader")

set (CMAKE_AUTOMOC ON)
set (CMAKE_INCLUDE_CURRENT_DIR ON)

BISON_TARGET(SchematicsParser schematics_parser.yy ${CMAKE_CURRENT_BINARY_DIR}/schematics_parser.cpp)
FLEX_TARGET(SchematicsScanner schematics_scanner.ll ${CMAKE_CURRENT_BINARY_DIR}/schematics_scanner.cpp)
ADD_FLEX_BISON_DEPENDENCY(SchematicsScanner SchematicsParser)

include_directories(
	"${CMAKE_CURRENT_SOURCE_DIR}/.."
	${CMAKE_CURRENT_SOURCE_DIR}
	"${CMAKE_CURRENT_BINARY_DIR}/../qlayout"
	${CMAKE_CURRENT_BINARY_DIR}
)

set(SOURCES
	schematicsdata.cpp
	schematicsscanner.cpp
)

add_library(${TARGET} SHARED
	${BISON_SchematicsParser_OUTPUTS}
	${FLEX_SchematicsScanner_OUTPUTS}
	${SOURCES}
)

target_link_libraries(${TARGET} ${QT5_LIBRARIES} QEditor)

#if(BUILD_STAND_ALONE_BINARIES)
#	add_executable(schematics_editor schematics_editor.cpp schematics_editor_app.cpp)
#	target_link_libraries(schematics_editor ${QT5_LIBRARIES} QEditor Schematics QLayout TechReader)
#endif(BUILD_STAND_ALONE_BINARIES)

