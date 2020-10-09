set(CMAKE_CXX_STANDARD 17)

set(THREADS_PREFER_PTHREAD_FLAG ON)

project(Tlang)

add_subdirectory(core)

aux_source_directory(. DIR_SRCS)

include_directories(./ ./core)

add_executable(${PROJECT_NAME} main.cpp)

find_package(Threads REQUIRED)

target_link_libraries(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT} core)

