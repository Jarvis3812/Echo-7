cmake_minimum_required(VERSION 3.22)
project(RileyCorpBrain)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(../core core_build)
add_executable(RileyCorpBrain main.cpp)
target_link_libraries(RileyCorpBrain PRIVATE core_build)
# Add other source files and modules as needed
