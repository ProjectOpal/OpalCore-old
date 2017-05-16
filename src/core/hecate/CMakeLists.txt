# # At LEAST 3.0 but newer is better.
# cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
# # Set the name and the version of the library module.
# project(hecate VERSION 0.1 LANGUAGES CXX C)

# add_subdirectory("protos")
# set_source_files_properties(${PROTO_HDRS} ${PROTO_SRCS}
#                             PROPERTIES GENERATED TRUE)

# add_library(hecate STATIC hecate.cpp ${PROTO_SRCS} ${PROTO_HDRS})
# add_dependencies(hecate protos)
# target_include_directories(hecate PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/.."
#                            "${CMAKE_CURRENT_BINARY_DIR}/..")
# target_link_libraries(hecate freertos)

# CMake file for compiling a shil.
#
# Author: Sam Rosenstein
# Date: 4/16/17
#
# This CMake file follow the example CMake file provided in the examples/module
#   folder.

# At LEAST 3.0 but newer is better.
cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

# Set the module name
set(MODULE_NAME "hecate")
# Set the files needed to compile the module
set(SRCS "hecate.cpp")
# Set if you have a protos folder that is being compiled. Note: This assumes
#   the protos are located under a protos folder.
set(HAS_PROTOS TRUE)
# Set the modules this module requires. All modules require freertos as a base
set(MODULE_REQUIREMENTS "freertos")
# Set the version of the module.
set(MODULE_VERSION "0.1")

################# Don't need to touch anything below ######################
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
                           "${CMAKE_CURRENT_SOURCE_DIR}/.."
                           "${CMAKE_CURRENT_BINARY_DIR}/..")

target_link_libraries(${PROJECT_NAME} ${MODULE_REQUIREMENTS})



