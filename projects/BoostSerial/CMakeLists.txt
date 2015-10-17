cmake_minimum_required(VERSION 2.8)
project( BoostSerial )
find_package(Boost 1.55 COMPONENTS system REQUIRED)
include_directories(${Boost_INCLUDE_DIRS}) 
add_executable( BoostSerial BoostSerial.cpp )
target_link_libraries(BoostSerial
  ${Boost_SYSTEM_LIBRARY}
)
