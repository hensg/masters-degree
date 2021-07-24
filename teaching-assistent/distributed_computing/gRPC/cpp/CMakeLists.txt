cmake_minimum_required(VERSION 3.5.1)

project(Calculadora C CXX)

include(./common.cmake)
include(FetchContent)

FetchContent_Declare(
    gRPC
    GIT_REPOSITORY https://github.com/grpc/grpc
    GIT_TAG        v1.28.0
    )
set(FETCHCONTENT_QUIET OFF)
FetchContent_MakeAvailable(gRPC)

# Proto file
get_filename_component(calculadora_proto "../calculadora.proto" ABSOLUTE)
get_filename_component(calculadora_proto_path "${calculadora_proto}" PATH)

# Generated sources
set(calculadora_proto_srcs "${PROJECT_SOURCE_DIR}/src-gen/calculadora.pb.cc")
set(calculadora_proto_hdrs "${PROJECT_SOURCE_DIR}/src-gen/calculadora.pb.h")
set(calculadora_grpc_srcs "${PROJECT_SOURCE_DIR}/src-gen/calculadora.grpc.pb.cc")
set(calculadora_grpc_hdrs "${PROJECT_SOURCE_DIR}/src-gen/calculadora.grpc.pb.h")
add_custom_command(
    OUTPUT "${calculadora_proto_srcs}" "${calculadora_proto_hdrs}" "${calculadora_grpc_srcs}" "${calculadora_grpc_hdrs}"
    COMMAND ${_PROTOBUF_PROTOC}
    ARGS --grpc_out "${PROJECT_SOURCE_DIR}/src-gen"
    --cpp_out "${PROJECT_SOURCE_DIR}/src-gen"
    -I "${calculadora_proto_path}"
    --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
    "${calculadora_proto}"
    DEPENDS "${calculadora_proto}")

# Include generated *.pb.h files
include_directories("${CMAKE_CURRENT_BINARY_DIR}")

# calculadora_grpc_proto
add_library(calculadora_grpc_proto
    ${calculadora_grpc_srcs}
    ${calculadora_grpc_hdrs}
    ${calculadora_proto_srcs}
    ${calculadora_proto_hdrs})
target_link_libraries(calculadora_grpc_proto
    ${_REFLECTION}
    ${_GRPC_GRPCPP}
    ${_PROTOBUF_LIBPROTOBUF}
    )

# Targets calculadora_[async_](client|server)
foreach(_target
        calculadora_server calculadora_client)
    add_executable(${_target} "${_target}.cc")
    target_link_libraries(${_target}
        calculadora_grpc_proto 
        ${_REFLECTION}
        ${_GRPC_GRPCPP}
        ${_PROTOBUF_LIBPROTOBUF})
endforeach()
