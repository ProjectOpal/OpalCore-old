cmake_minimum_required(VERSION 2.8)
project(PROJECT_OPAL C CXX)

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
set(FREERTOS_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/freertos-addons)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${FREERTOS_DIRS}")
find_package(Freertos REQUIRED)

################# Main COMPILATION #####################
set(HDR_FILES)
set(CPP_FILES)

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/src/core/hectate")
# find_package(hectate REQUIRED)
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/src/SHIL")

# PROTO_SRCS and PROTO_HDRS are generated from NanoPB.
# PROTO_CPP_HDRS is generated from secondary cpp proto generation.
# HDR_FILES and CPP_FILES are generated from adding add_subdirectory.
# message(STATUS "Hdr files: ${HDR_FILES}")
message(STATUS "HPP file: ${HDR_FILES}")

# TODO (@smr277): Figure out how to get rid of this
include_directories("${CMAKE_CURRENT_SOcURCE_DIR}/src/SHIL")

add_executable(main src/main.cpp ${PROTO_SRCS} ${PROTO_HDRS} ${PROTO_CPP_HDRS}
               ${HDR_FILES} ${CPP_FILES})
