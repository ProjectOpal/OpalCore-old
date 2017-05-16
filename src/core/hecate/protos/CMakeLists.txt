# Template CMake file for compiling protos within modules.
#
# Author: Sam Rosenstein
# Date: 4/16/17
# This CMake file can be put into the protos folder within a module
#   and allows you to compile your protos within that folder. It also
#   sets up the include paths correctly.

# Set the protos you would like to compile here
set(PROTOS "core.proto" "imu.proto")

########### Don't touch anything below this line ###############
include(${CMAKE_BINARY_DIR}/../extras/CMakeLists.txt)
nanopb_generate_cpp(PROTO_SRCS PROTO_HDRS ${PROTOS})
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -g -O0")
generate_cpp_proto(PROTO_CPP_HDRS ${PROTO_HDRS})
list(APPEND PROTO_HDRS ${PROTO_CPP_HDRS})
set(PROTOS_PATH)
foreach(item ${PROTO_HDRS})
  get_filename_component(item_path ${item} DIRECTORY)
  list(APPEND PROTOS_PATH ${item_path})
endforeach(item)
list(REMOVE_DUPLICATES PROTOS_PATH)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
add_custom_target("${PROJECT_NAME}-protos" DEPENDS ${PROTO_HDRS} ${PROTO_SRCS})

set(PROTOS_PATH ${PROTOS_PATH} PARENT_SCOPE)
set(PROTO_HDRS ${PROTO_HDRS} PARENT_SCOPE)
set(PROTO_SRCS ${PROTO_SRCS} PARENT_SCOPE)
