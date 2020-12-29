cmake_minimum_required(VERSION 2.8)

project(Tcp)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpermissive -g")

include_directories(/usr/local/include)
set(LIBS boost_system boost_thread pthread)

#SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb -pg -fpermissive ")

add_subdirectory("${PROJECT_SOURCE_DIR}/TcpServer")
