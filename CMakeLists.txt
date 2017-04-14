
cmake_minimum_required(VERSION 3.0)

project(hurricane)

include_directories(
	${PROJECT_SOURCE_DIR}/deps/meshy/include
	${PROJECT_SOURCE_DIR}/deps/meshy/include/utils
)

add_subdirectory(
	deps/meshy
)
include_directories(include/main)
add_executable(hurricane main.cpp)
target_link_libraries(hurricane meshy)
