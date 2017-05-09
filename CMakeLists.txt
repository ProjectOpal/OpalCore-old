cmake_minimum_required(VERSION 2.8)
project(PROJECT_OPAL C CXX)
# set(CMAKE_VERBOSE_MAKEFILE "1")

################# PROTO COMPILATION #####################

# location of nanopb top level directory
set(NANOPB_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/nanopb)
# set(TOOLS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tools)

# add nanopb cmake to modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${NANOPB_DIRS}/extra")
find_package(Nanopb REQUIRED)
include_directories(${NANOPB_INCLUDE_DIRS})
include(${CMAKE_CURRENT_SOURCE_DIR}/extras/CMakeLists.txt)

################# FreeRTOS C++ COMPILATION #####################
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/freertos-addons)
if (NOT ${CMAKE_CROSSCOMPILING})
  set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS}
    "-std=c++11 -Wall -Werror -Wextra -Wpedantic -pthread -O0 -g")
endif(NOT ${CMAKE_CROSSCOMPILING})

################# Main COMPILATION #####################

# Add the directories with the

# add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/src/core/hecate")

add_executable(main src/main.cpp)
target_link_libraries(main freertos)
