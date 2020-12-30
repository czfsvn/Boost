cmake_minimum_required(VERSION 2.8)

add_executable(allocation_server server.cpp)
target_link_libraries(allocation_server ${LIBS})
