cmake_minimum_required(VERSION 2.8)
project(PROJECT_OPAL C)

# location of nanopb top level directory
set(NANOPB_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/nanopb)

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

add_executable(main src/main.cpp ${PROTO_SRCS} ${PROTO_HDRS})
