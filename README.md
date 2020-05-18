# AVLIT-Engine
I guess you guys aren't ready for that yet, but your kids are gonna love it.

## How to build
The project uses CMake to build the engine and the application (minimum version 3.2).

First of all, you need to get the submodules used by the engine, to do so, type in a terminal:
```
git submodule init
git submodule update
```
This surely can be done via a git GUI program, but I'm not familiar with any of them.


 - **On Windows** (using Visual studio CMake integration):
 
At the root of the project, just create a directory build/[*Debug or Release*] (it is important for everything to work as intended to be two directories deep).
 
Then, just create a configuration a CMakeSettings.json file at the root of the project, containing at least one of the configuration below (please watch for the paths and the compiler choice):
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
            "cmakeCommandArgs": "-DCMAKE_PREFIX_PATH=C:/Path/To/Qt5/Directory/lib/cmake/Qt5",
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
            "cmakeCommandArgs": "-DCMAKE_PREFIX_PATH=C:/Path/To/Qt5/Directory/lib/cmake/Qt5",
            "buildCommandArgs": "-v",
            "ctestCommandArgs": "",
            "variables": [],
            "cmakeToolchain": ""
        }
    ]
}
```
And you're done. All you need to do is open the project folder in Visual Studio and CMake should run by itself, you just need to start the build when CMake is done.


 - **On Linux** (and maybe MacOS ?):
 
 All you need to do is:
 ```
mkdir build; mkdir build/WhateverYouWant
cd build/WhateverYouWant
cmake ../.. -DCMAKE_PREFIX_PATH=/path/to/Qt5/cmake/files/if/not/in/$path -DCMAKE_BUILD_TYPE=Debug/Release
make
cd bin
./SceneEditor&
echo "EZ"
```

## Dependencies
 The engine depends on three well known libraries: 

 - [glm](https://github.com/g-truc/glm) for pretty much all the linear algebra
 - [glad](https://github.com/Dav1dde/glad) to load the OpenGL functions
 - [assimp](https://github.com/assimp/assimp) to import assets (models/materials)
 - [stb](https://github.com/nothings/stb) to import textures

The demo uses Qt (feel free to modify the CMakeLists.txt file in the Application directory to avoid building it).


## What's coming
Here's a non exhaustive list of features I plan on implementing:

 - TAA
 - Area lights
 - DX12
 - SSGI
 - SSR

