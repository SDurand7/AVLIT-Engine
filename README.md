# AVLIT-Engine
Not maintained anymore. Brand new engine incoming.

## How to build
The project uses CMake to build the engine and the application (minimum version 3.2).

First of all, you need to get the submodules used by the engine, to do so, type in a terminal:
```
git submodule init
git submodule update
```
This surely can be done via a git GUI program, but I'm not familiar with any of them.


 - **On Windows** (using Visual studio CMake integration):
 
To build, you just need to create a CMakeSettings.json configuration file at the root of the project, containing at least one of the configurations below (please watch for the paths and the compiler choice):
```
{
    "configurations": [
        {
            "name": "Debug",
            "generator": "Ninja",
            "configurationType": "Debug",
            "inheritEnvironments": [ "clang_cl_x64" ],
            "buildRoot": "C:/Path/To/Root/Directory/ALIT-Engine/build/Debug",
            "installRoot": "",
            "cmakeCommandArgs": "-DCMAKE_PREFIX_PATH=C:/Path/To/Qt5/Directory/lib/cmake/Qt5 -DRENDERING_API:STRING=OpenGL",
            "buildCommandArgs": "-v",
            "ctestCommandArgs": "",
            "variables": []
        },
        {
            "name": "Release",
            "generator": "Ninja",
            "configurationType": "Release",
            "inheritEnvironments": [ "clang_cl_x64" ],
            "buildRoot": "C:/Path/To/Root/Directory/ALIT-Engine/build/Release",
            "installRoot": "",
            "cmakeCommandArgs": "-DCMAKE_PREFIX_PATH=C:/Path/To/Qt5/Directory/lib/cmake/Qt5 -DRENDERING_API:STRING=OpenGL",
            "buildCommandArgs": "-v",
            "ctestCommandArgs": "",
            "variables": [],
            "cmakeToolchain": ""
        }
    ]
}
```
And you're done. All you need to do is open the project folder in Visual Studio and CMake should run by itself, you just need to start the build when CMake is done.


 - **On Linux** (and MacOS ?):
 
 All you need to do is:
 ```
mkdir build
cd build
cmake .. -DCMAKE_PREFIX_PATH=/path/to/Qt5/cmake/files/if/not/in/$path -DCMAKE_BUILD_TYPE=Debug/Release -DRENDERING_API:STRING=OpenGL
make -jX
bin/Demo
```

## Dependencies
 The engine depends on these well known libraries: 

 - [glm](https://github.com/g-truc/glm) for pretty much all the linear algebra
 - [glad](https://github.com/Dav1dde/glad) to load the OpenGL functions
 - [assimp](https://github.com/assimp/assimp) to import assets (models/materials)
 - [stb](https://github.com/nothings/stb) to import textures

The demo uses Qt (feel free to modify the CMakeLists.txt file in the Application directory to avoid building it).


## What's coming
Here's a non exhaustive list of things I'll be working on in the near future:

 - A DirectX 11 renderer (to be integrated soon)
 - A particle system
 - Voxel GI
 - SSR

