cmake_minimum_required(VERSION 2.8)

add_executable(spawn_echo_server echo_server.cpp)
target_link_libraries(spawn_echo_server ${LIBS})
