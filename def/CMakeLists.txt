set(TARGET "DEF")

set (CMAKE_AUTOMOC ON)
set (CMAKE_INCLUDE_CURRENT_DIR ON)

BISON_TARGET(DEFParser def_parser.yy ${CMAKE_CURRENT_BINARY_DIR}/def_parser.cpp)
FLEX_TARGET(DEFScanner def_scanner.ll ${CMAKE_CURRENT_BINARY_DIR}/def_scanner.cpp)
ADD_FLEX_BISON_DEPENDENCY(DEFScanner DEFParser)

include_directories(
	"${CMAKE_CURRENT_SOURCE_DIR}/.."
	${CMAKE_CURRENT_SOURCE_DIR}
	"${CMAKE_CURRENT_BINARY_DIR}/../qlayout"
	${CMAKE_CURRENT_BINARY_DIR}
)

set(SOURCES
	defdata.cpp
	defdatapin.cpp
	deflayouteditor.cpp
	deflayouteditorwidget.cpp
	defmoduleinfo.cpp
	defrouteinfo.cpp
	defscanner.cpp
)

add_library(${TARGET} SHARED
	${BISON_DEFParser_OUTPUTS}
	${FLEX_DEFScanner_OUTPUTS}
	${SOURCES}
)

target_link_libraries(${TARGET}
	${QT5_LIBRARIES}
	QEditor
	QLayout
)

if(BUILD_STAND_ALONE_BINARIES)
	QT5_ADD_RESOURCES(ICONS ${PROJECT_SOURCE_DIR}/icons/icons.qrc)
	add_executable(def_editor def_editor.cpp def_editor_app.cpp ${ICONS})
	target_link_libraries(def_editor ${QT5_LIBRARIES} QEditor DEF QLayout TechReader)
endif(BUILD_STAND_ALONE_BINARIES)

install(TARGETS ${TARGET} LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

