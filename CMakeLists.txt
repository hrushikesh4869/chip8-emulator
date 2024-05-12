cmake_minimum_required(VERSION 3.22.1)
project(chip-8)
add_executable(Main main.cpp)

add_subdirectory(lib)

target_include_directories(Main PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_link_libraries(Main PUBLIC cpu)
target_link_libraries(Main PUBLIC ram)
target_link_libraries(Main PUBLIC display)
target_link_libraries(Main PUBLIC io)
target_link_libraries(Main PUBLIC -pthread)
