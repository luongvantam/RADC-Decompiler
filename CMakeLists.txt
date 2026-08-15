cmake_minimum_required(VERSION 3.10)
project(radc_decompiler)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build." FORCE)
endif()

file(GLOB SOURCES 
  "src/*.cpp"
  "src/casioemu/*.cpp"
  "src/casioemu/Chipset/*.cpp"
  "src/casioemu/Peripheral/*.cpp"
  "src/casioemu/Data/*.cpp"
)

add_executable(radc ${SOURCES})
