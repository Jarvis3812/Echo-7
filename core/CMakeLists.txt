cmake_minimum_required(VERSION 3.22)
project(RileyCorpBrainCore)

set(CMAKE_CXX_STANDARD 17)

add_library(core_build STATIC riley_corpbrain.cpp schema_model.cpp database.cpp python_embed.cpp)
# Add other core source files as needed
