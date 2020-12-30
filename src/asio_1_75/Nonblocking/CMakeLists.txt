cmake_minimum_required(VERSION 2.8)

add_executable(third_party_lib third_party_lib.cpp)
target_link_libraries(third_party_lib ${LIBS})