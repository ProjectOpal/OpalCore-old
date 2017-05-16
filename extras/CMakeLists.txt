# Helpful functions for compiling Project Opal.
# Author: Sam Rosenstein (samr37l@gmail.com)
# Date: 4/16/17

# At LEAST 3.0 but newer is better.
cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

# Generate CPP proto headers from nanopb generated headers. Allows for
#   useful functions and namespacing.
function(generate_cpp_proto HDRS)
  if (NOT ARGN)
    return()
  endif(NOT ARGN)

  if(NOT DEFINED GIT_ROOT)
    set(GIT_ROOT "${CMAKE_BINARY_DIR}/..")
  endif(NOT DEFINED GIT_ROOT)
  set(TOOLS_DIR ${GIT_ROOT}/tools)

  list(REMOVE_DUPLICATES ARGN)
  foreach(item ${ARGN})
    string(REGEX MATCH ".*\\.pb\\.h" item ${item})
    if(item)
      string(CONCAT hpp_proto_hdr "${item}" "pp")
      set(output_hdr "${hpp_proto_hdr}")
      add_custom_command(
        OUTPUT ${output_hdr}
        COMMAND python3 ${TOOLS_DIR}/nanopb_cpp.py ${item}
        DEPENDS ${item}
        COMMENT "Building HPP Headers for ${hpp_proto_hdr}"
      )
      list(APPEND ${HDRS} "${output_hdr}")
    endif(item)
  endforeach(item)
  set_source_files_properties(${${HDRS}} PROPERTIES GENERATED TRUE)
  set(${HDRS} ${${HDRS}} PARENT_SCOPE)
endfunction(generate_cpp_proto)

# Setup a module.
# opal_module creates a library from the provided arguments.
macro(opal_module MODULE_NAME MODULE_VERSION MODULE_REQUIREMENTS SRCS
         HAS_PROTOS)
  project(${MODULE_NAME} VERSION ${MODULE_VERSION} LANGUAGES CXX C)
  if(HAS_PROTOS)
    add_subdirectory("protos")
    set_source_files_properties(${PROTO_HDRS} ${PROTO_SRCS}
                                PROPERTIES GENERATED TRUE)
  endif(HAS_PROTOS)

  add_library(${PROJECT_NAME} STATIC ${SRCS} ${PROTO_SRCS} ${PROTO_HDRS})

  if(HAS_PROTOS)
    add_dependencies(${PROJECT_NAME} "${PROJECT_NAME}-protos")
  endif(HAS_PROTOS)

  target_include_directories(${PROJECT_NAME} PUBLIC
                             "${CMAKE_CURRENT_SOURCE_DIR}/../.."
                             "${CMAKE_CURRENT_BINARY_DIR}/../..")

  target_link_libraries(${PROJECT_NAME} ${MODULE_REQUIREMENTS})
endmacro(opal_module)

# Build the protos from a folder within a module.
# Creates vars PROTOS_PATH PROTO_HDRS and PROTO_SRCS
macro(opal_module_protos)
  set(ARGN ${ARGN})
  if (NOT ARGN)
    return()
  endif(NOT ARGN)

  include(${CMAKE_BINARY_DIR}/../extras/CMakeLists.txt)
  nanopb_generate_cpp(PROTO_SRCS PROTO_HDRS ${ARGN})
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
endmacro(opal_module_protos)
