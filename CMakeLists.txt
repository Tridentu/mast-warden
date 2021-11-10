cmake_minimum_required(VERSION 3.10)
project(mast-warden)



find_package(Curses REQUIRED)
find_package(LibNotify REQUIRED)
pkg_search_module(GTK2 REQUIRED gtk+-3.0)
find_library(MENU_LIBRARY menu)
find_library(MAST_TK_LIB mast_tk)
find_package(ICU 69.0 COMPONENTS uc i18n REQUIRED)

include_directories(${CURSES_INCLUDE_DIR})
include_directories(${LIBNOTIFY_INCLUDE_DIRS})
include_directories(${GTK2_INCLUDE_DIRS})
include_directories(${CMAKE_INSTALL_PREFIX}/include/mast_tk)

link_directories(${GTK2_LIBRARY_DIRS})

include_directories("./src")
file (GLOB mw_SOURCES src/*.cpp)

add_definitions(${GTK2_CFLAGS_OTHER})

add_executable(mast-warden ${mw_SOURCES})
target_link_libraries(mast-warden ${CURSES_LIBRARIES} ${MENU_LIBRARY} ${GTK2_LIBRARIES} mast_tk  ICU::uc ICU::i18n gobject-2.0 xkbregistry)

install(TARGETS mast-warden RUNTIME DESTINATION ${BIN_DIR})
