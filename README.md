# COMP4102Group

Generates a page for a colouring book from an input image. The page is black and white and is numbered to an associating colour legend.

## CMake

CMake is a multi-platform tool for being able to use whatever toolchain on whatever operating system to build a project.

Below are instructions on how to compile.
The executable should be built in 'build' in the main directory.

## Windows Compilation

**NOTE**: You must have the .dll files of opencv in your `PATH` environment variable, e.g. `C:\opencv\build\x64\vc15\bin`

#### Visual Studio Open Folder Method
- Open the main directory of the git repository in Visual Studio by > Open a local folder
- Open source/CMakeLists.txt 
- Build all or Debug

#### CMake GUI Method
- Open CMake-GUI
- Set the source code to `source`, e.g. `C:/Users/DJ/Documents/GitHub/ColouringBook/source`
- Set the build directory to a clean directory, e.g. `C:/Users/DJ/Documents/GitHub/ColouringBook/source/build`
- Click Configure > Visual Studio 2019, everything else default
- Click Generate
- Click Open Project
- Build ColouringBook (can be done by right clicking in the solution explorer)

#### To assign commands arguments in Visual Studio
- Right click CMakeLists.txt > Add Debug Configuration > Default
- Add `"args": ["../1.jpg"]` under `"name": "CMakeLists.txt",` (make sure there's a comma)

## Linux Compilation
In Linux to use CMake:
- mkdir build && cd build
- cmake ..
- make