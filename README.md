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
