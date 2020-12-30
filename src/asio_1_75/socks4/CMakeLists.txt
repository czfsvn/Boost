cmake_minimum_required(VERSION 2.8)

add_executable(sync_client sync_client.cpp)
target_link_libraries(sync_client ${LIBS})
