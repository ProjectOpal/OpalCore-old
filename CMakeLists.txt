cmake_minimum_required(VERSION 2.8)
project(PROJECT_OPAL C CXX)

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

# set_source_files_properties(${PROTO_CPP_HDRS}
#     PROPERTIES GENERATED TRUE)


add_executable(main src/main.cpp ${PROTO_SRCS} ${PROTO_HDRS} ${PROTO_CPP_HDRS})
