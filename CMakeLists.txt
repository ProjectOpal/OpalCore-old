cmake_minimum_required(VERSION 2.8)
project(PROJECT_OPAL C CXX)

################# PROTO COMPILATION #####################

# location of nanopb top level directory
set(NANOPB_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/nanopb)
set(TOOLS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tools)

# add nanopb cmake to modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${NANOPB_DIRS}/extra")
find_package(Nanopb REQUIRED)
include_directories(${NANOPB_INCLUDE_DIRS})

# generate core protos
file(GLOB_RECURSE PROTO_FILES
  "${CMAKE_CURRENT_SOURCE_DIR}/src/core/protos/*.proto"
)
nanopb_generate_cpp(PROTO_SRCS PROTO_HDRS ${PROTO_FILES})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
#add_custom_target(generate_proto_sources DEPENDS ${PROTO_SRCS} ${PROTO_HDRS})
set_source_files_properties(${PROTO_SRCS} ${PROTO_HDRS}
    PROPERTIES GENERATED TRUE)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -g -O0")

# Find all of the compiled proto.pb.h files
set(ONLY_PROTO_HDRS)
foreach(item ${PROTO_HDRS})
  string(REGEX MATCH ".*\\.pb\\.h" item ${item})
  if(item)
    list(APPEND ONLY_PROTO_HDRS ${item})
  endif(item)
endforeach(item ${PROTO_HDRS})

# Process each proto.pb.h file into proto.pb.hpp files
foreach(proto_hdr ${ONLY_PROTO_HDRS})
  string(CONCAT hpp_proto_hdr "${proto_hdr}" "pp")
  add_custom_command(
    OUTPUT ${hpp_proto_hdr}
    COMMAND python3 ${TOOLS_DIR}/nanopb_cpp.py ${proto_hdr}
    DEPENDS ${proto_hdr}
    COMMENT "Building HPP Headers for ${proto_hdr}"
  )
  set(PROTO_CPP_HDRS ${PROTO_CPP_HDRS} ${hpp_proto_hdr})
endforeach()

################# FreeRTOS C++ COMPILATION #####################

set(FREERTOS_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/FreeRTOS/Source)
set(FREERTOS_INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/FreeRTOS/Source/include)
set(PORTABLE_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/freertos-addons/Linux/portable/GCC/Linux)
set(PORTABLE_SRC_MEM_MANG_DIR ${CMAKE_CURRENT_SOURCE_DIR}/FreeRTOS/Source/portable/MemMang)
set(FREERTOS_CPP_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/freertos-addons/c++/Source)
set(FREERTOS_CPP_INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/freertos-addons/c++/Source/include)

# Find all the .c freertos files
file(GLOB FREERTOS_SRC
        "${FREERTOS_SRC_DIR}/*.c")
# Choose the memory managment alogorithm
set(FREERTOS_SRC ${FREERTOS_SRC}
                 "${PORTABLE_SRC_MEM_MANG_DIR}/heap_3.c")
# Choose port mapping
set(FREERTOS_SRC ${FREERTOS_SRC} "${PORTABLE_SRC_DIR}/port.c")

# Find all freertos CPP files from freertos-addons
file(GLOB FREERTOS_CPP_SRC
          "${FREERTOS_CPP_SRC_DIR}/*.cpp")

# Include .h and .hpp files to the build
include_directories(${FREERTOS_INC_DIR})
include_directories(${FREERTOS_CPP_INC_DIR})
include_directories(${PORTABLE_SRC_DIR})

# Include all FreeRTOS header files
file(GLOB FREERTOS_SRC_HDRS
          "${FREERTOS_INC_DIR}/*.h"
          "${FREERTOS_CPP_INC_DIR}.hpp")

# Include port mapping
set(FREERTOS_SRC_HDRS ${FREERTOS_SRC_HDRS}
                      "${PORTABLE_SRC_DIR}/portmacro.h")

# Include the FreeRTOSConfig file
set(FREERTOS_SRC_HDRS ${FREERTOS_SRC_HDRS}
                      "${CMAKE_CURRENT_SOURCE_DIR}/src/HIL/FreeRTOSConfig.h")

# Include Linux Macros
set (FREERTOS_SRC_HDRS ${FREERTOS_SRC_HDRS}
                       "${CMAKE_CURRENT_SOURCE_DIR}/src/HIL/FreeRTOSmacros.hpp")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/src/HIL")

message(STATUS "Loaded FreeRTOS and FreeRTOSCPP files")

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-Wall -Werror -Wextra -Wpedantic -pthread -O0 -g")

################# Main COMPILATION #####################

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/src/include")

add_executable(main src/main.cpp ${PROTO_SRCS} ${PROTO_HDRS} ${PROTO_CPP_HDRS}
${FREERTOS_SRC} ${FREERTOS_CPP_SRC} ${FREERTOS_SRC_HDRS})
