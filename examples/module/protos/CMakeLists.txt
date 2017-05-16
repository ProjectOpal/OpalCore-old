# Template CMake file for compiling protos within modules.
#
# Author: Sam Rosenstein
# Date: 4/16/17
# This CMake file can be put into the protos folder within a module
#   and allows you to compile your protos within that folder. It also
#   sets up the include paths correctly.

# Set the protos you would like to compile here
opal_module_protos("thing1.proto" "thing2.proto")
