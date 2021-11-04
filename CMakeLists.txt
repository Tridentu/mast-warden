cmake_minimum_required(VERSION 3.10)
project(mast-warden)



find_package(Curses REQUIRED)
find_library(MENU_LIBRARY menu)
find_package(ICU 69.0 COMPONENTS uc i18n REQUIRED)

include_directories(${CURSES_INCLUDE_DIR})

include_directories("./src")
file (GLOB mw_SOURCES src/*.cpp)

add_executable(mast-warden ${mw_SOURCES})
target_link_libraries(mast-warden ${CURSES_LIBRARIES} ${MENU_LIBRARY} ICU::uc ICU::i18n)

install(TARGETS mast-warden RUNTIME DESTINATION ${BIN_DIR})
