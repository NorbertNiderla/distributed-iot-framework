cmake_minimum_required(VERSION 3.10)

project(comm_app)

include_directories(./../../src/include/)
link_directories(./../../src/include/)

set (SRC_PATH ./../../src/)
set (SOURCES
    ${SRC_PATH}logger.cpp
    ${SRC_PATH}protocol.cpp
    ${SRC_PATH}communication.cpp
    main.cpp 
    )


add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})

# pthread flag is required
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE Threads::Threads)