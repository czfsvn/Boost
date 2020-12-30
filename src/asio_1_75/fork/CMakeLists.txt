cmake_minimum_required(VERSION 2.8)

add_executable(daemon daemon.cpp)
target_link_libraries(daemon ${LIBS})

add_executable(process_per_connection process_per_connection.cpp)
target_link_libraries(process_per_connection ${LIBS})