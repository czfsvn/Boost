file(GLOB_RECURSE SRC_FILES *.cpp)

#set(EXCLUDE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/Main.cpp)
#list(REMOVE_FILES SRC_FILES ${EXCLUDE_FILES})


add_executable(server ${SRC_FILES})
target_link_libraries(server ${LIBS})
