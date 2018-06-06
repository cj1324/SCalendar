
project(SCalendar C)

cmake_minimum_required(VERSION 2.6)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

include_directories(${GTK3_INCLUDE_DIRS})

add_executable(scal src/scal.c)

target_link_libraries(scal ${GTK3_LIBRARIES})
