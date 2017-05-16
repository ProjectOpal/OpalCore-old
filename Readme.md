# Project Opal
### What is it?!
Project Opal is an open source C++ UAV platform, that is hardware agnostic and modular. It runs ontop of [FreeRTOS](http://www.freertos.org/) which can be ported to many different hardware layers (even Linux!). The repository is the base of Project Opal, providing a centralized location where all core and use supplied modules exist.
### Cool, so how do I use it?
Head over to our website (TODO) to download a build for your flight controller!

## Developer information
### Getting started
- Download the git repo: `git clone https://github.com/projectopal/OpalCore`
- Setup the git submodules: `git submodule init --recursive`
- Update the git submodules: `git submodule update --recursive`

### Building Native
Currently the Native build only runs on Linux (tested on Ubuntu 16.04). If developing on Mac or Windows (why) download [Vagrant](https://www.vagrantup.com/) and the [Ubuntu 16.04 box](https://atlas.hashicorp.com/bento/boxes/ubuntu-16.04). For more information, head over to [src/shil](https://github.com/projectopal/OpalCore/tree/master/src/modules/shil).

- Start in the top level of the git directory
- Create build directory `mkdir build`
- `cd build`
- Build Project Opal! `cmake .. && make project_opal -j4`
