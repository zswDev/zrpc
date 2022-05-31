cmake_minimum_required(VERSION 3.22)

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
