# Hexeng2D

![Hexeng2D Logo](/Resources/hexeng2d_logo.png)

Hexeng2D is the project of an open source cross-plateform 2D Game Engine framework, written in c++.

For the moment however, Hexeng2D is in an early stage of developpement in wich cross-plateform is not guarented and some bugs might be there. Also the code is pretty messy and unsafe.

***

## Installation and Setup

<ins>**0. Prerequisites on linux :**</ins>

- Make sure you have X11 installed on your machine. If you don't, run the command specified [here](https://www.glfw.org/docs/3.3/compile.html#compile_deps_x11)

- Make sure you have alsa-dev installed on your machine. If you don't, run :
    - `sudo apt-get install libasound2-dev` on debian based distributions
    - `sudo dnf install alsa-lib-devel`on fedora and similar.
If you're neither on debian based nor on fedora and similar, try to search for how to install alsa-dev on your distribution.

- Make sure OpenGL dev package is installed, if it's not, install it (in most distributions it's called libgl-dev).

<ins>**1. Clone the repository :**</ins>

In a terminal (git bash if you're on windows, the classic terminal on linux), run the following commands :
`cd <destination>`
`git clone https://github.com/Ily3s/Hexeng2D`

<ins>**2. Precompile using CMake:**</ins>

In a terminal that includes cmake (for example, the vs terminal on windows), run the following commands :
`cd <destination>`
`cmake . -G <IDE> -A <platform>`
Note that the plateform can be left unspecified

<ins>**3. How to use :**</ins>

Cmake will output a project with at least 3 sub projects :

- Hexeng2D is the engine, which will compile into a dynamic link library.
- Sandbox is a demo of the engine that I use to test and debug the engine.
- EmptyProject is an empty project that you can use to build your game

You can rename EmptyProject and build your game in it or you can compile the engine binaries, take all its headers and link Hexeng2D to your project as you want. Note that if you go for the second option, you will also have to take Hexeng2D dependencies headers.

To get started, I recommand that you take a look at the Sandbox sub-broject in order for you to understand how the engine works. Also you can find the documentation in this repo at doc/html/index.html

<ins>**Note :**</ins> The recommanded configuration is Visual Studio 2022 on Windows

***

## Features

I could do a nice and little presentation of all the great features of my software at the top of the README file, but screw that, I'm a software engineer not a salesman, so here is a quick enumeration instead :

- Classic Rendering
- Batch Rendering
- Text Rendering
- Audio Player
- Language System
(notice how I make each first letter in upper case so it seems to be a big thing, I'm so good at selling my things, ain't I)
- Physics Engine and HitBoxes
- Saves System
- Quads Rendering
- Animations System
- Event Manager System
- Layers System
- Camera System
(the last two basically just means there is depth and a z coordinate going on)
- Scenes System

These were the big features of the engine, hope you had fun reading that.