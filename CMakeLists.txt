cmake_minimum_required(VERSION 3.20)
project(kaizen-server CXX)

add_executable(kaizen-server main.cpp)
target_include_directories(kaizen-server PRIVATE
	.
	external/enet/include
)