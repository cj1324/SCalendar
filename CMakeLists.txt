
project(SCalendar C)

cmake_minimum_required(VERSION 2.6)

set(CMAKE_BUILD_TYPE Release)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
pkg_check_modules(CURL REQUIRED libcurl)

include_directories(${GTK3_INCLUDE_DIRS})
include_directories(${CURL_INCLUDE_DIRS})
include_directories(src)

add_executable(scal src/scal.c src/notifications.c src/datasource.c)

target_link_libraries(scal ${GTK3_LIBRARIES})
target_link_libraries(scal ${CURL_LIBRARIES})
