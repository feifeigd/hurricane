
cmake_minimum_required(VERSION 3.0)

project(hurricane)

include_directories(
	${PROJECT_SOURCE_DIR}/deps/meshy/include/utils
)
include_directories(include/main)
add_executable(hurricane main.cpp)
