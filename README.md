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

Additional Packets, Driver Modules, and Control Modules for FuseMuse can be found at https://github.com/Riounis/FuseMuseModules. To add these to your build of FuseMuse, simply download the zip or jar file for the module you want to use and add it to your default packets, default driver modules, or default control modules folder in your FuseMuse build. If you want to develop your own modules for use with FuseMuse, please see the readme file at the repository listed above.

### Music Library

If you want to use the FuseMuse music library for your project that does not need to incorporate the FuseMuse procedural generation engine, it can be found at https://github.com/Riounis/libfm.

### For Developers Working on FuseMuse

FuseMuse is a Qt application. Before working on the GUI, please familiarize yourself with the Qt signals and slots mechanism. {{link}} This is used for all user interaction.

#### FM Zip Info

FMZipInfo is a class which stores the metadata of a Packet, Driver Module, or Control Module (henceforth referred to as modules) as well as the absolute path of the module. They are initialized with the path of the module and automatically gather the metadata from it when the program starts or when a module is loaded. These are stored in the composition tree and used to execute modules. FMZipInfo.cpp

#### Shell

The method which executes the FuseMuse composition tree is executeShell in shell.cpp (soon). It operates by executing the Driver Module, populating the Composition object returned with the Packet Tree. It then traverses the Packet Tree, executing Packets in a Leftmost Depth-First Search order. After each Packet execution, the shell executes the control module. Finally, the Shell executes the final control module and returns the fully populated Composition JSON after playing it. 

Execution of modules is handled by the execute method in the same file. This method extracts the archive if necessary depending on the execution type and mode of the module in question, and it handles input and output.

#### Architecture and Location of Files

A JSON specification for the libfm data structure is provided in the programmer documentation in the repository at https://github.com/Riounis/FuseMuseModules and in java doc comments in source code at https://github.com/Riounis/libfm. On a high level, libfm provides tools for composers by having notes and chords, which store their pitches and duration, along with any accents that can be added to notes, dynamics, which express the beginning and ending of dynamic changes including crescendos and decrescendos, and composition metrics which store the key (and scale), time signature, and tempo of the composition, along with the position in the composition at which these metrics apply. Additionally, libfm contains Parts, which are lists of notes, chords, and dynamics, and Compositions, which are collections of Parts and Composition Metrics. Libfm also offers pattern-based music structuring as an additional feature for composition. The Pattern Segment holds a chord progression. Compositions have patterns made of names of Pattern Segments. These names reference the chord progression within the pattern segment. This allows FuseMuse module developers to add more structure to the music they create. The code for the default modules for FuseMuse can be found in the Driver Modules, Packets, or Control Modules directory of the repository at https://github.com/sigmaJ/FuseMuse.
