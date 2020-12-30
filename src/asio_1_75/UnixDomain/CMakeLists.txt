cmake_minimum_required(VERSION 2.8)

add_executable(connect_pair connect_pair.cpp)
target_link_libraries(connect_pair ${LIBS})

add_executable(iostream_client iostream_client.cpp)
target_link_libraries(iostream_client ${LIBS})

add_executable(stream_client stream_client.cpp)
target_link_libraries(stream_client ${LIBS})

add_executable(stream_server stream_server.cpp)
target_link_libraries(stream_server ${LIBS})