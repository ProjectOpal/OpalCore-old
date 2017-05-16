# Source for Opal

This contains all the current core and supporting source code for Project Opal. **However** there are external modules that may not be included in this repo. This only contains the core module and modules needed for basic UAV operation for fixed wing (quad support will come shortly). 

### Working with Opal
#### Creating a new module
A new module needs a few thing. First, create a new directory for your module. Within that directory you will need:

* A `parameter.proto` file. This file contains all the things that a user can configure for your application. Certain things should be noted: (TBD TODO @smr277)
* The CPP and HPP files for your system. Your module should implement the Timer thread class, and mark at which interval your module should run. (Example should be here TODO @smr277). 
* The internal protos used by your system 
* CMake file to compile your module, and any supporting files for compilation (TODO @smr277: figure out best way to do this). 