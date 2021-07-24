cmake_minimum_required(VERSION 3.15)

# set the project name
project(presentation)

include(./common.cmake)

find_package(Protobuf REQUIRED)

include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

get_filename_component(person_proto "person.proto" ABSOLUTE)
get_filename_component(person_proto_path "${person_proto}" PATH)

# Generated sources
set(person_proto_src "${PROJECT_SOURCE_DIR}/src-gen/person.pb.cc")
set(person_proto_hdrs "${PROJECT_SOURCE_DIR}/src-gen/person.pb.h")

add_custom_command(
    OUTPUT "${person_proto_src}" "${person_proto_hdrs}"
    COMMAND ${_PROTOBUF_PROTOC}
    ARGS --cpp_out "${PROJECT_SOURCE_DIR}/src-gen"
    -I "${person_proto_path}"
    "${person_proto}"
    DEPENDS "${person_proto}")

add_executable(serializador serializador.cc ${person_proto_src} ${person_proto_hdrs})
add_executable(desserializador desserializador.cc ${person_proto_src} ${person_proto_hdrs})

target_link_libraries(serializador ${_PROTOBUF_LIBPROTOBUF})
target_link_libraries(desserializador ${_PROTOBUF_LIBPROTOBUF})
