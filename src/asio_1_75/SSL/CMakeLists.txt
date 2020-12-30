cmake_minimum_required(VERSION 2.8)

add_executable(ssl_client client.cpp)
target_link_libraries(ssl_client ${LIBS})

add_executable(ssl_server server.cpp)
target_link_libraries(ssl_server ${LIBS})