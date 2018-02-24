set(TARGET "Magic")
set (CMAKE_AUTOMOC ON)
set (CMAKE_INCLUDE_CURRENT_DIR ON)

BISON_TARGET(MagicParser magic_parser.yy ${CMAKE_CURRENT_BINARY_DIR}/magic_parser.cpp)
FLEX_TARGET(MagicScanner magic_scanner.ll ${CMAKE_CURRENT_BINARY_DIR}/magic_scanner.cpp)
ADD_FLEX_BISON_DEPENDENCY(MagicScanner MagicParser)

include_directories(
	"${CMAKE_CURRENT_SOURCE_DIR}/.."
	${CMAKE_CURRENT_SOURCE_DIR}
	"${CMAKE_CURRENT_BINARY_DIR}/../qlayout"
	${CMAKE_CURRENT_BINARY_DIR}
)

set(SOURCES
	magicdata.cpp
	magiclayouteditor.cpp
	magiclayouteditorwidget.cpp
	magicscanner.cpp
)

add_library(${TARGET} SHARED
	${BISON_MagicParser_OUTPUTS}
	${FLEX_MagicScanner_OUTPUTS}
	${SOURCES}
)

target_link_libraries(${TARGET} ${QT5_LIBRARIES} QEditor)

if(BUILD_STAND_ALONE_BINARIES)
	add_executable(magic_editor magic_editor.cpp magic_editor_app.cpp)
	target_link_libraries(magic_editor ${QT5_LIBRARIES} QEditor Magic QLayout TechReader Magic3D)
endif(BUILD_STAND_ALONE_BINARIES)

