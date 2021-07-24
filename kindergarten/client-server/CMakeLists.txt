cmake_minimum_required(VERSION 3.0.0)
project(shared-memory C)

include(CTest)
enable_testing()

add_executable(server server.c tpool.h tpool.c)
add_executable(client client.c)

set(CMAKE_C_FLAGS -pthread)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
include(CPack)
