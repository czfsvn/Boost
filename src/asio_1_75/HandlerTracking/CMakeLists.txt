cmake_minimum_required(VERSION 2.8)

add_executable(async_tcp_echo_server2 async_tcp_echo_server.cpp)
target_link_libraries(async_tcp_echo_server2 ${LIBS})
