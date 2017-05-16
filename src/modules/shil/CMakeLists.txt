# At LEAST 3.0 but newer is better.
cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
# Set the name and the version of the library module.
project(shil VERSION 0.1 LANGUAGES CXX C)

set_source_files_properties(${PROTO_HDRS} ${PROTO_SRCS}
                            PROPERTIES GENERATED TRUE)

add_library(shil STATIC imu.cpp PracticalSocket.cpp)
target_include_directories(shil PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/.."
                           "${CMAKE_CURRENT_BINARY_DIR})/..")
target_link_libraries(shil freertos hecate)


