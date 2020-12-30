cmake_minimum_required(VERSION 2.8)

add_executable(receiver receiver.cpp)
target_link_libraries(receiver ${LIBS})

add_executable(sender sender.cpp)
target_link_libraries(sender ${LIBS})
