set(TARGET "Schematics")

set (CMAKE_AUTOMOC ON)
set (CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories(
	"${CMAKE_CURRENT_SOURCE_DIR}/.."
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
)

set(SOURCES
	schematicseditor.cpp
	schematicseditorwidget.cpp
	schematicslibraryeditor.cpp
	schematicsparametricpartdialog.cpp
	schematicspart.cpp
	schematicspartselection.cpp
	schematicswire.cpp
)

set(UI_SOURCES
	partselection.ui
	parametricpart.ui
)

qt5_wrap_ui(UI_GENERATED_HEADERS ${UI_SOURCES})

add_library(${TARGET} SHARED
	${SOURCES}
	${UI_GENERATED_HEADERS}
)

target_link_libraries(${TARGET} ${QT5_LIBRARIES})

