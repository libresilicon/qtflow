set(TARGET "QtFlow")

set (CMAKE_AUTOMOC ON)
set (CMAKE_INCLUDE_CURRENT_DIR ON)

BISON_TARGET(DEFParser def_parser.yy ${CMAKE_CURRENT_BINARY_DIR}/def_parser.cpp)
FLEX_TARGET(DEFScanner def_scanner.ll ${CMAKE_CURRENT_BINARY_DIR}/def_scanner.cpp)
ADD_FLEX_BISON_DEPENDENCY(DEFScanner DEFParser)

find_package(PythonLibs REQUIRED)

include_directories(
	"${CMAKE_CURRENT_BINARY_DIR}/../schematics"
	"${CMAKE_CURRENT_BINARY_DIR}/../qlayout"
	"${CMAKE_CURRENT_BINARY_DIR}/../contacts"
	"${CMAKE_CURRENT_SOURCE_DIR}/../PythonQt/src"
	"${CMAKE_CURRENT_BINARY_DIR}"
	"${CMAKE_CURRENT_SOURCE_DIR}/.."
	${PYTHON_INCLUDE_DIRS}
)

set(SOURCES
	buildflow.cpp
	editortabmanager.cpp
	fileselector.cpp
	grid.cpp
	mainapplication.cpp
	main.cpp
	mainwindow.cpp
	moduleselector.cpp
	moduleslistmodel.cpp
	modulestreemodel.cpp
	new.cpp project.cpp
	project_padframe.cpp
	projectselector.cpp
	projectsettings.cpp
	projectsitem.cpp
	projectstreemodel.cpp
	pyprojectsettings.cpp
	pysettings.cpp
	pythonconsoledockwidget.cpp
	qtflowfilelist.cpp
	qtreeviewdragdrop.cpp
	savechanges.cpp
	settings.cpp
	specialnets.cpp
	templates.cpp
	wave.cpp
	welcome.cpp
)

set(UI_SOURCES
	buildflow.ui
	console.ui
	files.ui
	grid.ui
	librarysettings.ui
	mainwindow.ui
	modules.ui
	new.ui
	projectsettings.ui
	projects.ui
	savechanges.ui
	settings.ui
	specialnets.ui
	templates.ui
	wave.ui
	welcome.ui
)

qt5_wrap_ui(UI_GENERATED_HEADERS ${UI_SOURCES})

QT5_ADD_RESOURCES(ICONS ${PROJECT_SOURCE_DIR}/icons/icons.qrc)

add_executable(${TARGET} ${SOURCES} ${UI_GENERATED_HEADERS} ${ICONS})

target_link_libraries(${TARGET}
	${QT5_LIBRARIES}
	QEditor
	QLayout
	Contact
	LEF
	DEF
	Qt5Python36_QtAll
	VCD
	VCDReader
	TechReader
	QSchematics
	Schematics
	SchematicsReader
	ColorMapReader
	SymbolReader
	Symbols
	GDS
	Magic
	Magic3D
)
