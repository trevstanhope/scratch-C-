cmake_minimum_required(VERSION 2.8)
project( TaskScheduler )
find_package(Boost 1.55 COMPONENTS system REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++1y")
include_directories(${Boost_INCLUDE_DIRS}) 
add_executable( TaskScheduler TaskScheduler.cpp )
target_link_libraries(TaskScheduler
  ${Boost_SYSTEM_LIBRARY}
)
