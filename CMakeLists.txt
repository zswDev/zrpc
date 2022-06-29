cmake_minimum_required(VERSION 3.22)


set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O -Wall")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O -Wall")

find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache) # Less useful to do it for linking, see edit2
endif(CCACHE_FOUND)

#set(CMAKE_CXX_COMPILE "/usr/bin/clang++")
#set(CMAKE_C_COMPILE "/usr/bin/clang")
#set(CMAKE_MAKE_PROGRAM "/usr/bin/make")

set(CMAKE_TOOLCHAIN_FILE "/home/code/download/vcpkg/scripts/buildsystems/vcpkg.cmake")

#set(CMAKE_CXX_IMPLICIT_LINK_DIRECTORIES /usr/local/lib ${CMAKE_CXX_IMPLICIT_LINK_DIRECTORIES})

project(zrpc)

# find_package(gRPC CONFIG REQUIRED)

# find_package(Protobuf REQUIRED)

set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(Threads REQUIRED)

# find_package(Boost REQUIRED COMPONENTS)
# include_directories(${Boost_INCLUDE_DIRS})

# find_library(HI_REDIS hiredis)
# find_package(redis++ CONFIG REQUIRED)

# find_package(mongoc-1.0 CONFIG REQUIRED)
# find_package(mongocxx CONFIG REQUIRED)
# find_package(bsoncxx CONFIG REQUIRED)

# find_package(nlohmann_json CONFIG REQUIRED)

# find_package(OpenSSL REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

# add_executable(server src/server.cpp include/srpc.grpc.pb.cc include/srpc.pb.cc)
# target_link_libraries(server PRIVATE Threads::Threads gRPC::gpr gRPC::upb gRPC::grpc gRPC::grpc++ protobuf::libprotoc protobuf::libprotobuf protobuf::libprotobuf-lite)

# add_executable(client src/client.cpp include/srpc.grpc.pb.cc include/srpc.pb.cc)
# target_link_libraries(client PRIVATE Threads::Threads gRPC::gpr gRPC::upb gRPC::grpc gRPC::grpc++ protobuf::libprotoc protobuf::libprotobuf protobuf::libprotobuf-lite)

# add_executable(async_client src/async_client.cpp include/srpc.grpc.pb.cc include/srpc.pb.cc)
# target_link_libraries(async_client PRIVATE Threads::Threads gRPC::gpr gRPC::upb gRPC::grpc gRPC::grpc++ protobuf::libprotoc protobuf::libprotobuf protobuf::libprotobuf-lite)

# add_executable(async_server src/async_server.cpp include/srpc.grpc.pb.cc include/srpc.pb.cc)
# target_link_libraries(async_server PRIVATE Threads::Threads gRPC::gpr gRPC::upb gRPC::grpc gRPC::grpc++ protobuf::libprotoc protobuf::libprotobuf protobuf::libprotobuf-lite)

# add_executable(http src/http.cpp)
# target_link_libraries(http PRIVATE Threads::Threads OpenSSL::SSL OpenSSL::Crypto)

# add_executable(websocket src/websocket.cpp)
# target_link_libraries(websocket PRIVATE Threads::Threads)

# add_executable(async_websocket src/async_websocket.cpp)
# target_link_libraries(async_websocket PRIVATE Threads::Threads)

# add_executable(chan src/chan.cpp)
# target_link_libraries(chan PRIVATE Threads::Threads)

# add_executable(gate src/gate.cpp)
# target_link_libraries(gate PRIVATE Threads::Threads)

# add_executable(redis src/redis.cpp)
# target_link_libraries(redis PRIVATE ${HI_REDIS} redis++::redis++_static Threads::Threads)

# add_executable(mongo src/mongo.cpp)
# target_link_libraries(mongo PRIVATE mongo::mongoc_static mongo::mongocxx_static mongo::bsoncxx_static)

# add_executable(json src/json.cpp)
# set(nlohmann-json_IMPLICIT_CONVERSIONS OFF)
# target_link_libraries(json PRIVATE nlohmann_json::nlohmann_json)

# find_package(SQLiteCpp CONFIG REQUIRED)
# add_executable(sqlite src/sqlite.cpp)
# target_link_libraries(sqlite PRIVATE SQLiteCpp)

find_package(libmysql CONFIG REQUIRED)
find_path(MySQL_INCLUDE_DIR
    NAMES mysql_version.h
    PATH_SUFFIXES mysql
)
include_directories(${MySQL_INCLUDE_DIR})
add_executable(mysql src/mysql.cpp)
target_link_libraries(mysql PRIVATE ${MYSQL_LIBRARIES})