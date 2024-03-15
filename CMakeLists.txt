cmake_minimum_required(VERSION 3.20)
project(kaizen-server CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED 1)

add_executable(kaizen-server main.cpp arena.cpp)
target_include_directories(kaizen-server PRIVATE
	.
	external/enet/include
)