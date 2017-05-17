# Template CMake file for compiling a module.
#
# Author: Sam Rosenstein
# Date: 4/16/17
# This CMake file can be put into the root directory of a module to compile
#   the module with CMake. The module can then be included in other modules
#   by #include "moduleName/module.hpp". Any folder struture within the module
#   SHOULD be kept by adding CMake files.

# At LEAST 3.0 but newer is better.
cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

# Set the module name
set(MODULE_NAME "stabalization")
# Set the files needed to compile the module
set(SRCS "stabalization.cpp")
# Set if you have a protos folder that is being compiled. Note: This assumes
#   the protos are located under a protos folder.
set(HAS_PROTOS True)
# Set the modules this module requires. All modules require freertos as a base
set(MODULE_REQUIREMENTS "freertos" "hecate")
# Set the version of the module.
set(MODULE_VERSION "0.1")

opal_module("${MODULE_NAME}" "${MODULE_VERSION}" "${MODULE_REQUIREMENTS}"
            "${SRCS}" "${HAS_PROTOS}")


