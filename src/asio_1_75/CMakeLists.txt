cmake_minimum_required(VERSION 2.8)

project(asio_1_75)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpermissive -g -O0")
set(LIBS boost_system boost_filesystem crypto boost_iostreams boost_thread pthread)

include_directories(/usr/local/include /usr/local/boost_1_75/include)
link_directories(/usr/local/lib /usr/local/boost_1_75/lib)

SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb -pg -fpermissive -DBOOST_ASIO_HAS_LOCAL_SOCKETS")

SET(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS_DEBUG})

set(DIRS
    allocation
    buffers
    chat
    echo
    fork
    Futures
    HandlerTracking
    Multicast
    Nonblocking
    #Operations
    socks4
    #Spawn
    #SSL
    Timeouts
    Timer
    UnixDomain
    )

foreach(DIR ${DIRS})
    add_subdirectory("${PROJECT_SOURCE_DIR}/${DIR}")
endforeach()
