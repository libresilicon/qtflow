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
	qlayoutlabel.cpp
	genericlayouteditorwidget.cpp
	genericlayouteditor.cpp
	tech_data_wrapper.cpp
)

set(UI_SOURCES
	cellmanager.ui
	drcsettings.ui
	layoutvisibles.ui
)

qt5_wrap_ui(UI_GENERATED_HEADERS ${UI_SOURCES})

QT5_ADD_RESOURCES(TECH_FILES ${PROJECT_SOURCE_DIR}/tech/tech.qrc)

add_library(${TARGET} SHARED
	${SOURCES}
	${UI_GENERATED_HEADERS}
	${TECH_FILES}
)

target_link_libraries(${TARGET} ${QT5_LIBRARIES} GDS)
install(TARGETS ${TARGET} LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
