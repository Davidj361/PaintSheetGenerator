# Paint Sheet Generator

Generates a paint sheet from an input image. The page is black and white and is numbered to an associating colour legend.

#### To Run & Use
Run the program in command line and provide the pathname to an image as an argument. Program will then open up a GUI and display where the rest of the interaction is done through keyboard key press.

#### Controls

- `e` is calculate with no dilated edges
- `d` is calculate with dilated edges
- `s` is save the result/product image in the same directory as the executable
- `1` shows the result/product
- `2` shows the quantized image with edges
- `3` shows the quantized image without edges
- `4` shows the original image

## CMake

CMake is a multi-platform tool for being able to use whatever toolchain on whatever operating system to build a project.

Below are instructions on how to compile the project with CMake.
The executable should be built in 'build' in the main directory.

## Windows Compilation

### Important Steps
- It is recommended to utilize System over User for environment variables
- `PATH` environment variable must have the the location of the OpenCV .dll files, e.g. `C:\opencv\build\x64\vc15\bin`
- `OPENCV_DIR` environment variable have the main directory of OpenCV which contains the `x64` , e.g. `C:\opencv\build`
- Applications must be restarted for reading new/updated environment variables
- The project directory should be clean of any Visual Studio project/solution files, etc

#### Visual Studio Open Folder Method
- Open the main directory of the git repository in Visual Studio by > Open a local folder
- Open source/CMakeLists.txt 
- Build all or Debug

#### CMake GUI Method
- Open CMake-GUI
- Set the source code to `source`, e.g. `C:/Users/DJ/Documents/GitHub/Project/source`
- Set the build directory to a clean directory, e.g. `C:/Users/DJ/Documents/GitHub/Project/source/build`
- Click Configure > Visual Studio 2019, everything else default
- Click Generate
- Click Open Project
- Build the code (can be done by right clicking in the solution explorer)

#### To assign commands arguments in Visual Studio
- Right click CMakeLists.txt > Add Debug Configuration > Default
- Add `"args": ["../1.jpg"]` under `"name": "CMakeLists.txt",` (make sure there's a comma)

## Linux Compilation
In Linux to use CMake:
- while in the main directory: mkdir build && cd build
- cmake ../source
- make
