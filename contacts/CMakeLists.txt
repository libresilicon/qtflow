set(TARGET "Contact")

set (CMAKE_AUTOMOC ON)
set (CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories(
	"${CMAKE_CURRENT_SOURCE_DIR}/.."
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
)

set(SOURCES
	contactplacement.cpp
	contactsview.cpp
	padinfo.cpp
)

set(UI_SOURCES
	contactplacement.ui
)

qt5_wrap_ui(UI_GENERATED_HEADERS ${UI_SOURCES})

add_library(${TARGET} SHARED
	${SOURCES}
	${UI_GENERATED_HEADERS}
)

target_link_libraries(${TARGET} ${QT5_LIBRARIES})

install(TARGETS ${TARGET} LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

