# Project Opal Modules
Modules are user build plugins that perform actions within Project Opal. This directory provides an example module and build system, with explainations on how to setup and build your own module. 

### Setup
A module consists of two main parts:

- The source code and protos that run the module
- A `parameter.proto` file that lets the use configure the module. This isn't required, but is helpful. 

For explaination on source code requirements, check `example.cpp`.

For explaination on the `parameter.proto` file, check the `parameter.proto` in the `protos` directory. 

### Building
Notice the root `CMakeLists.txt` file. Look at this file, and edit it according to your module. For protos, look at the `protos` file and the `CMakeLists.txt` file within that folder. 

Once you put your module in the `module` directory, the build system will auto recognize the module and add it to the build!

### Testing
TBD. LOLZ. 