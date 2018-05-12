# FuseMuse

Repository for FuseMuse, a Qt-based application for highly modular fusional procedural composition.

### Build steps

#### Dependencies

1. `quazip`: available via most package managers
2. `qt5`: available via most package managers

#### Setup

1. Clone the FuseMuse repository
2. `cd` into it; Run `git submodule init; git submodule update`

At this point, if you want to use Qt Creator, you are ready

#### Build

1. Create build directory, `cd` into it
2. run `qmake -makefile <<Path to Fusemuse.pro file in repository root>>`
3. `make`

A FuseMuse executable should now be ready to use in the current directory.  The build process also copies the default packets and modules to the build directory, so FuseMuse will be usable immediately.

### Adding Modules to FuseMuse

Additional Packets, Driver Modules, and Control Modules for FuseMuse can be found at https://github.com/Riounis/FuseMuseModules. To add these to your build of FuseMuse, simply download the zip or jar file for the module you want to use and add it to your default packets, default driver modules, or default control modules folder in your FuseMuse build.

### Music Library

If you want to use the FuseMuse music library for your project that does not need to incorporate the FuseMuse procedural generation engine, it can be found at https://github.com/Riounis/libfm.
