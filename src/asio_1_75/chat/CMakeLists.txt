cmake_minimum_required(VERSION 2.8)

add_executable(chat_client chat_client.cpp chat_message.hpp)
target_link_libraries(chat_client ${LIBS})

add_executable(chat_server chat_server.cpp chat_message.hpp)
target_link_libraries(chat_server ${LIBS})