set(TARGET "Symbols")

set (CMAKE_AUTOMOC ON)
set (CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/..
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
)

set(SOURCES
	symboleditor.cpp
	symboleditorscene.cpp
	symboleditorwidget.cpp
)

add_library(${TARGET} SHARED
	${SOURCES}
)

target_link_libraries(${TARGET} ${QT5_LIBRARIES})
install(TARGETS ${TARGET} LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

