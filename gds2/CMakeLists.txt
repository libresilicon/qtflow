set(TARGET "GDS")

set (CMAKE_AUTOMOC ON)
set (CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
)

set(SOURCES
	gdsboundary.cpp
	gdscell.cpp
	gdsfile.cpp
	gdspoint.cpp
	gdtdata.cpp
)

add_library(${TARGET} SHARED
	${SOURCES}
)

target_link_libraries(${TARGET} ${QT5_LIBRARIES})

install(TARGETS ${TARGET} LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

