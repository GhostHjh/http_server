cmake_minimum_required(VERSION 3.10)
project(http_server)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

add_executable(http_server ${CMAKE_SOURCE_DIR}/src/main.cpp)

target_link_libraries(http_server PUBLIC pthread sqlite3)

target_include_directories(http_server PUBLIC ${CMAKE_SOURCE_DIR})
