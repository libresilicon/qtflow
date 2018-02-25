set(TARGET "QLayout")

set (CMAKE_AUTOMOC ON)
set (CMAKE_INCLUDE_CURRENT_DIR ON)

BISON_TARGET(DEFParser def_parser.yy ${CMAKE_CURRENT_BINARY_DIR}/def_parser.cpp)
FLEX_TARGET(DEFScanner def_scanner.ll ${CMAKE_CURRENT_BINARY_DIR}/def_scanner.cpp)
ADD_FLEX_BISON_DEPENDENCY(DEFScanner DEFParser)

include_directories(
	"${CMAKE_CURRENT_SOURCE_DIR}/.."
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
)

set(SOURCES
	drcsettings.cpp
	layoutvisibles.cpp
	qlayoutcellmanager.cpp
	qlayoutdistancemeasure.cpp
	qlayoutmacroitem.cpp
	qlayoutmacropinitem.cpp
	qlayoutrectitem.cpp
	qlayoutscene.cpp
	qlayoutviaitem.cpp
	qlayoutwireitem.cpp
)

set(UI_SOURCES
	cellmanager.ui
	drcsettings.ui
	layoutvisibles.ui
)

qt5_wrap_ui(UI_GENERATED_HEADERS ${UI_SOURCES})

add_library(${TARGET} SHARED
	${SOURCES}
	${UI_GENERATED_HEADERS}
)

target_link_libraries(${TARGET} ${QT5_LIBRARIES} GDS)


