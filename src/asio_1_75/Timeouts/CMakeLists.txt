cmake_minimum_required(VERSION 2.8)

add_executable(timeouts_async_tcp_client async_tcp_client.cpp)
target_link_libraries(timeouts_async_tcp_client ${LIBS})

add_executable(timeouts_blocking_tcp_client blocking_tcp_client.cpp)
target_link_libraries(timeouts_blocking_tcp_client ${LIBS})

add_executable(timeouts_blocking_token_tcp_client blocking_token_tcp_client.cpp)
target_link_libraries(timeouts_blocking_token_tcp_client ${LIBS})

add_executable(timeouts_blocking_udp_client blocking_udp_client.cpp)
target_link_libraries(timeouts_blocking_udp_client ${LIBS})

add_executable(timeouts_server server.cpp)
target_link_libraries(timeouts_server ${LIBS})