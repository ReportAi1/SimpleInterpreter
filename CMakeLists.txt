cmake_minimum_required(VERSION 3.10)
project(SimpleInterpreter)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include src)

file(GLOB SOURCES "src/*.cpp")

add_executable(SimpleInterpreter ${SOURCES})