cmake_minimum_required(VERSION 2.8)

add_executable(reference_counted reference_counted.cpp)
target_link_libraries(reference_counted ${LIBS})
