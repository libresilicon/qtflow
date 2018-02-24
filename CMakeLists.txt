project(QtFlow)
cmake_minimum_required(VERSION 2.8.10)
option(BUILD_FULL_EDA "Build the full EDA" ON)
option(BUILD_STAND_ALONE_BINARIES "Build standalone binaries for testint EDA modules" ON)

find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5OpenGL REQUIRED)
find_package(KF5TextEditor REQUIRED)

if(BUILD_FULL_EDA)
	add_subdirectory(QtFlow)
	add_subdirectory(PythonQt)
	find_package(PythonLibs REQUIRED)

	#-----------------------------------------------------------------------------
	# Find Python
	find_package(PythonLibs REQUIRED)
	include_directories(${PYTHON_INCLUDE_DIRS})
	add_definitions(-DPYTHONQT_USE_RELEASE_PYTHON_FALLBACK)

	#-----------------------------------------------------------------------------
	# Library Name
	# To maintain ABI compatibility, the Qt major version as well as the Python major and minor version are needed
	string(SUBSTRING ${PYTHONLIBS_VERSION_STRING} 0 1 python_major)
	string(SUBSTRING ${PYTHONLIBS_VERSION_STRING} 2 1 python_minor)
	set(PythonQt Qt${QT_VERSION_MAJOR}Python${python_major}${python_minor} CACHE INTERNAL "")
endif(BUILD_FULL_EDA)

set(QT5_LIBRARIES
	Qt5::Widgets
	Qt5::Core
	Qt5::OpenGL
	Qt5::Gui
	KF5::TextEditor
)

add_subdirectory(qeditor)
add_subdirectory(qlayout)
add_subdirectory(tech_reader)
add_subdirectory(def)
add_subdirectory(lef)
add_subdirectory(schematics)
add_subdirectory(schematics_reader)
add_subdirectory(qschematics)
add_subdirectory(magic)
add_subdirectory(magic3d)
add_subdirectory(contacts)
add_subdirectory(vcd)
add_subdirectory(vcd_reader)
#add_subdirectory(tech)
#add_subdirectory(blif)
add_subdirectory(cmap_reader)
add_subdirectory(gds2)
#add_subdirectory(helpers)
add_subdirectory(symbol_reader)
add_subdirectory(symbols)

#add_subdirectory(coriolis)

