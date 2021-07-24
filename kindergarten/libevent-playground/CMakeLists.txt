cmake_minimum_required (VERSION 3.11)
project(libevent-playground)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

find_package(LIBEVENT REQUIRED)

include_directories(${LIBEVENT_INCLUDE_DIRS})

add_executable(main main.c)
target_link_libraries(main ${LIBEVENT_LIBRARIES})
