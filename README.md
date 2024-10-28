# Hexeng2D

![Hexeng2D Logo](/Resources/hexeng2d_logo.png)

Hexeng2D is an open source cross-plateform 2D Game Engine framework, written in c++.

For the moment, Hexeng2D is supported only on linux and windows.

## Installation and Setup

<ins>**0. Prerequisites on linux :**</ins>

- X11 or Wayland and xkbcommon development packages are required. To install them, see the command specified [here](https://www.glfw.org/docs/3.4/compile.html#compile_deps_wayland)

- alsa developement package is required. To install it, run :
    - `sudo apt-get install libasound2-dev` on debian based distributions
    - `sudo dnf install alsa-lib-devel` on redhat based distributions.

- OpenGL dev package is required, in most distributions it's called libgl-dev.

<ins>**1. Clone the repository :**</ins>

If you are on windows, install git bash if it's not arleady done, open it and type the following command :
`git clone https://github.com/Ily3s/Hexeng2D`.
On linux, any terminal will do the trick.

<ins>**2. Compile using CMake:**</ins>

Install cmake if it's not already done.  
To integrate the repo in an IDE :
`cmake . -G <IDE>`  
To build the repo :
`cmake --build .`  

If you don't have an IDE to do it for you, you can chose to compile in Debug or in Release with the command
`cmake -DCMAKE_BUILD_TYPE=Target` (where Target is Debug or Release). Do it before building the repo

Everything should be built in the "bin" directory that can be located either in the repository root or sometimes in a Debug/Release folder (depending on your cmake options and/or your IDE settings)

<ins>**3. How to use :**</ins>

This repo comes with at least 3 sub projects :

- Hexeng2D is the engine, which will compile into a dynamic link library.
- Sandbox is a demo of the engine that I use to test and debug the engine.
- EmptyProject is an empty project that you can use to build your game.

There are also some sample games of the engine in the Samples/ directory.

To make a game, you can rename EmptyProject and build your game in it or you can take the engine binary file (the dynamic link library), along with all its headers and link Hexeng2D to your project as you want. Note that if you go for the second option, you will also have to take Hexeng2D dependencies headers.

To get started, I recommand that you take a look at the samples in order for you to understand how the engine works. Also you can find the documentation in this repo at doc/html/index.html.

To run a sample (or even the Sandbox project) you have to copy the res/ folder (of the sample you wish to run) in the bin/ directory (note that this isn't necessary if you run the sample with an IDE).

Any game you ship have to come with the Hexeng2D dynamic link library in the same folder as your game's executable.

## Features

Here is a useless enumeration of the key features of the engine :

- Quad and polygons rendering
- Batch rendering
- Text rendering
- Audio player
- Language system
- Physics engine (really just a bad hitbox system)
- Saves system
- Animations system
- Event manager
- Layers and camera system (depth)
- Scenes system
