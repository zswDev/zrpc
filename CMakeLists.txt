cmake_minimum_required(VERSION 3.22)


set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Os -Wall -s")

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Os -Wall -s")

find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache) # Less useful to do it for linking, see edit2
endif(CCACHE_FOUND)

#set(CMAKE_CXX_COMPILE "/usr/bin/clang++")
#set(CMAKE_C_COMPILE "/usr/bin/clang")
#set(CMAKE_MAKE_PROGRAM "/usr/bin/make")

set(CMAKE_TOOLCHAIN_FILE "/home/code/download/vcpkg-master/scripts/buildsystems/vcpkg.cmake")

#set(CMAKE_CXX_IMPLICIT_LINK_DIRECTORIES /usr/local/lib ${CMAKE_CXX_IMPLICIT_LINK_DIRECTORIES})

project(zrpc)

find_package(gRPC CONFIG REQUIRED)

find_package(Protobuf REQUIRED)

set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(Threads REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable(server src/server.cpp include/srpc.grpc.pb.cc include/srpc.pb.cc)
target_compile_features(server PRIVATE cxx_std_17)
target_link_libraries(server PRIVATE Threads::Threads gRPC::gpr gRPC::upb gRPC::grpc gRPC::grpc++ protobuf::libprotoc protobuf::libprotobuf protobuf::libprotobuf-lite)

add_executable(client src/client.cpp include/srpc.grpc.pb.cc include/srpc.pb.cc)
target_compile_features(client PRIVATE cxx_std_17)
target_link_libraries(client PRIVATE Threads::Threads gRPC::gpr gRPC::upb gRPC::grpc gRPC::grpc++ protobuf::libprotoc protobuf::libprotobuf protobuf::libprotobuf-lite)

add_executable(async_client src/async_client.cpp include/srpc.grpc.pb.cc include/srpc.pb.cc)
target_compile_features(async_client PRIVATE cxx_std_17)
target_link_libraries(async_client PRIVATE Threads::Threads gRPC::gpr gRPC::upb gRPC::grpc gRPC::grpc++ protobuf::libprotoc protobuf::libprotobuf protobuf::libprotobuf-lite)

add_executable(async_server src/async_server.cpp include/srpc.grpc.pb.cc include/srpc.pb.cc)
target_compile_features(async_server PRIVATE cxx_std_17)
target_link_libraries(async_server PRIVATE Threads::Threads gRPC::gpr gRPC::upb gRPC::grpc gRPC::grpc++ protobuf::libprotoc protobuf::libprotobuf protobuf::libprotobuf-lite)

