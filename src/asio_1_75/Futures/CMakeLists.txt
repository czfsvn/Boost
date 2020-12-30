cmake_minimum_required(VERSION 2.8)

add_executable(daytime_client daytime_client.cpp)
target_link_libraries(daytime_client ${LIBS})
