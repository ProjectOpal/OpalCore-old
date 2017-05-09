# At LEAST 3.0 but newer is better.
cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
# Set the name and the version of the library module.
project(Hecate VERSION 0.1 LANGUAGES CXX C)
# Set the root of the git directory. This is user dependant.
set(GIT_ROOT "/vagrant/OpalCore")

# Include the nanopb directory.
set(NANOPB_DIRS ${GIT_ROOT}/nanopb)
# Add the Nanopb directory to this build.
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${NANOPB_DIRS}/extra")
find_package(Nanopb REQUIRED)

# Include the FreeRTOS directory.
set(FREERTOS_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/freertos-addons)
# Add the FreeRTOS directory to this build.
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${FREERTOS_DIRS}")
find_package(Freertos REQUIRED)

# Set to the protos that we want to build
set(PROTOS "protos/imu.proto"
           "protos/core.proto")

# Build protos
nanopb_generate_cpp(PROTO_SRCS PROTO_HDRS ${PROTO_FILES})
set_source_files_properties(${PROTO_SRCS} ${PROTO_HDRS}
    PROPERTIES GENERATED TRUE)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -g -O0")
include(${GIT_ROOT}/extras/CMakeLists.txt)
generate_cpp_proto(PROTO_FILES)

add_library(${CMAKE_PROJECT_NAME} hecate.cpp)
