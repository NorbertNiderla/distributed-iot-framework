cmake_minimum_required(VERSION 3.10)

project(iot-communication-app)

add_executable(iot-communication-app src/logger.cpp src/main.cpp src/message.cpp src/udp_comm.cpp)

# pthread flag is required
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(iot-communication-app PRIVATE Threads::Threads)