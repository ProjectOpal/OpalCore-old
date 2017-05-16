# At LEAST 3.0 but newer is better.
cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
# Set the name and the version of the library module.
project(hecate VERSION 0.1 LANGUAGES CXX C)

add_subdirectory("protos")
set_source_files_properties(${PROTO_HDRS} ${PROTO_SRCS}
                            PROPERTIES GENERATED TRUE)

add_library(hecate STATIC hecate.cpp ${PROTO_SRCS} ${PROTO_HDRS})
add_dependencies(hecate protos)
target_include_directories(hecate PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/.."
                           "${CMAKE_CURRENT_BINARY_DIR}/..")
target_link_libraries(hecate freertos)


