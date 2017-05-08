# At LEAST 3.0 but newer is better.
cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
# Set the name and the version of the library module.
project(stabalization VERSION 0.1 LANGUAGES CXX C)
# Set the root of the git directory. This is user dependant.
set(GIT_ROOT "/Users/sam/Documents/cuair/OpalCore")
# Include the nanopb directory.
set(NANOPB_DIRS ${GIT_ROOT}/nanopb)
# Add the Nanopb directory to this build.
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${NANOPB_DIRS}/extra")
find_package(Nanopb REQUIRED)

# FreeRTOS Include.
set(FREERTOS_SRC_DIR ${GIT_ROOT}/FreeRTOS/Source)
set(FREERTOS_INC_DIR ${GIT_ROOT}/FreeRTOS/Source/include)
set(PORTABLE_SRC_DIR ${GIT_ROOT}/freertos-addons/Linux/portable/GCC/Linux)
set(PORTABLE_SRC_MEM_MANG_DIR ${GIT_ROOT}/FreeRTOS/Source/portable/MemMang)
set(FREERTOS_CPP_SRC_DIR ${GIT_ROOT}/freertos-addons/c++/Source)
set(FREERTOS_CPP_INC_DIR ${GIT_ROOT}/freertos-addons/c++/Source/include)
# Find all the .c freertos files
file(GLOB FREERTOS_SRC
        "${FREERTOS_SRC_DIR}/*.c")
# Find all freertos CPP files from freertos-addons
file(GLOB FREERTOS_CPP_SRC
          "${FREERTOS_CPP_SRC_DIR}/*.cpp")

# Choose the memory managment alogorithm
set(FREERTOS_SRC ${FREERTOS_SRC}
                 "${PORTABLE_SRC_MEM_MANG_DIR}/heap_3.c")
# Choose port mapping
set(FREERTOS_SRC ${FREERTOS_SRC} "${PORTABLE_SRC_DIR}/port.c")

# Include .h and .hpp files to the build
include_directories(${FREERTOS_INC_DIR})
include_directories(${FREERTOS_CPP_INC_DIR})
include_directories(${PORTABLE_SRC_DIR})
# Include all FreeRTOS header files
file(GLOB FREERTOS_SRC_HDRS
          "${FREERTOS_INC_DIR}/*.h"
          "${FREERTOS_CPP_INC_DIR}/*.hpp")
# Include port mapping
set(FREERTOS_SRC_HDRS ${FREERTOS_SRC_HDRS}
                      "${PORTABLE_SRC_DIR}/portmacro.h")

# Include the FreeRTOSConfig file
set(FREERTOS_SRC_HDRS ${FREERTOS_SRC_HDRS}
                      "${CMAKE_CURRENT_SOURCE_DIR}/src/SHIL/FreeRTOSConfig.h")
# Include Linux Macros
set (FREERTOS_SRC_HDRS ${FREERTOS_SRC_HDRS}
                       "${CMAKE_CURRENT_SOURCE_DIR}/src/SHIL/FreeRTOSmacros.hpp")
message(STATUS "Loaded FreeRTOS and FreeRTOSCPP files")




