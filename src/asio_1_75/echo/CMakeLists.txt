cmake_minimum_required(VERSION 2.8)

add_executable(async_tcp_echo_server async_tcp_echo_server.cpp )
target_link_libraries(async_tcp_echo_server ${LIBS})

add_executable(async_udp_echo_server async_udp_echo_server.cpp)
target_link_libraries(async_udp_echo_server ${LIBS})

add_executable(blocking_tcp_echo_client blocking_tcp_echo_client.cpp)
target_link_libraries(blocking_tcp_echo_client ${LIBS})

add_executable(blocking_tcp_echo_server blocking_tcp_echo_server.cpp)
target_link_libraries(blocking_tcp_echo_server ${LIBS})

add_executable(blocking_udp_echo_client blocking_udp_echo_client.cpp)
target_link_libraries(blocking_udp_echo_client ${LIBS})

add_executable(blocking_udp_echo_server blocking_udp_echo_server.cpp)
target_link_libraries(blocking_udp_echo_server ${LIBS})