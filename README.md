# Hexeng2D

![Hexeng2D Logo](/Resources/hexeng2d_logo.png)

Hexeng2D is (meant to be) an open source cross-plateform 2d game engine framework for c++. It is currently in an early state of developpement.

***

## How to get started

<ins>**1. Clone the repository:**</ins>

Open git bash and execute the command `cd *dest_folder*` with \*dest_folder\* being the path to the folder you want Hexeng2D in.
Then, execute the command `git clone https://github.com/Ily3s/Hexeng2D`

<ins>**2. Use CMake:**</ins>

Open a terminal that has the cmake command (for example, the vs terminal has cmake).
Go the the *dest_folder* you have Hexeng2D in with `cd *dest_folder*`.
Then use the command `cmake . -G "*IDE*" -A *platform*` with \*IDE\* being the IDE you are using and \*platform\* being the platform/architecture you want to build your game on.

<ins>**3. How to use :**</ins>

Cmake will output a project with at least 3 sub projects :

- Hexeng2D is the engine, which will compile into a dynamic link library.
- Sandbox is a demo of the engine that I use to test and debug the engine.
- EmptyProject is an empty project that you can use to build your game

You can rename EmptyProject and build your game in it or you can compile the engine binaries, take all its headers and link Hexeng2D to your project as you want.

<ins>**Notes :**</ins>

- For the moment, Hexeng2D (officially) supports only Windows
- The recommanded configuration is Visual Studio on Windows
- The only tested configuration is :
    - OS : Windows 10
    - Architecture : Win32, x64
    - IDE : Visual Studio 16 2019
    - Plateforms : Debug, Release
    - GPU : Nvidia GTX 1050, latest drivers
    - Screen : 1920px * 1080px, 60hz

***

## Current Developpement State

 **In short : Unfinished**

For the moment, I don't recommand Hexeng2D to make a game, but keep an eye on it, it will get good.