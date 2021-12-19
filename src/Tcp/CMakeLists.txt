cmake_minimum_required(VERSION 3.5)

project(Tcp)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)



#set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpermissive -g")

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

if(APPLE)
    message("UNIX1")
    SET(INC_DIR /usr/local/include/ /usr/local/boost/boost_1_75_0)
    SET(LIB_DIR /usr/local/lib/ /usr/local/boost/boost_1_75_0/stage/lib)
elseif(WIN32)
    message("UNIX2")
elseif(UNIX)
    message("UNIX3")
    SET(INC_DIR /usr/local/include /usr/local/boost_1_75/include ${PROJECT_SOURCE_DIR}/Protos)
    SET(LIB_DIR /usr/local/lib /usr/local/boost_1_75/lib)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
endif()

message("${INC_DIR}")
message("${LIB_DIR}")

include_directories(${INC_DIR})
include_directories(${PROJECT_SOURCE_DIR}/Protos)

link_directories(${LIB_DIR})
link_directories(${LIBRARY_OUTPUT_PATH})

message("UNIX")

ADD_DEFINITIONS(-DBOOST_LOG_DYN_LINK -D_GLIBCXX_USE_CXX11_ABI=0)

SET(CMAKE_BUILD_TYPE "Debug")
#SET(CMAKE_BUILD_TYPE "Release")

set(EXTRA_LIBS 
    pthread
    protobuf
    boost_system 
    boost_thread 
    boost_log_setup 
    boost_log  
    boost_filesystem 
    boost_program_options 
    boost_serialization 
    )

set(LOCAL_LIB cnpb)
set(LIBS ${EXTRA_LIBS} ${LOCAL_LIB})


message("EXECUTABLE_OUTPUT_PATH1: ${EXECUTABLE_OUTPUT_PATH}")
message("LIBRARY_OUTPUT_PATH1: ${LIBRARY_OUTPUT_PATH}")

#SET(OPTS "-O1 -Wall -g -ggdb -pg -fpermissive -Wdeprecated-copy")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} ${OPTS}")

add_subdirectory("${PROJECT_SOURCE_DIR}/Protos")
add_subdirectory("${PROJECT_SOURCE_DIR}/TcpServer" bin)

