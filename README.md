# 2D Raycasting and Rendering Simulator
## What does the title mean?
The main idea behind this project is to make a program that can take a 'maze' composed of different objects and cast various rays of simulated light from a 'camera' that will collide in one way or another with these objects in the maze as the user moves the camera across the window. These collisions can then be interpreted according to their distance away from the camera in order to produce a dynamic image (a render) that represents what the camera is seeing through its angle of vision.

## How does this program work?
It is made up of 3 main components:
### Rays *'of light'*
A ray (of light in this case) represents a line going from point A to point B. This line is made up of individual points where we can check if the ray of light has collided with a solid object (i.e. the point is inside the object), and if it has, we know that the ray must only travel up to this specific point.

Rays can be rotated (by applying matrix multiplication with a rotation matrix), and moved in any direction (by adding/subtracting a 2-Dimensional offset to all the points in the ray).

### The Camera
This is an object that groups rays together so that they all start from a common point. It is represented by a circle and the user of the program can control its movements by using the `WASD` keys, and its angle of rotation can be increased using the `↑ Up` arrow, and decreased using the `↓ Down` arrow.

> Some optimizations, like ordering the vector of solid objects according to their distance away from the camera to improve ray collision probability, or drawing all the rays as an array of triangles instead of hundreds of lines, have been done inside some methods of this class.

### The Renderer class
This class is a simpler class, meant to be associated with a Camera object and to **render** (as its name implies) what this object is *'seeing'*. It does this by taking the distances away from the camera where each of its rays has found a collision. Using these distances, and comparing them with a reference distance is how different shades of a color (in the case of the image below: red) are produced, where darker tones indicate an object that is further away from the camera.

All this information is represented in a view (which in the image below can be found in the top right corner), which is effectively a rendered simulation of what the camera object is *'seeing'* through its rays.

## Program images:
<img width="2733" height="1697" alt="preview image" src="https://github.com/user-attachments/assets/1a672370-077a-4af8-82d2-c3c7baaa0bfd" />
&nbsp;&nbsp;

> The maze that generated the solid objects in the image above is currently hardcoded inside the `main()` function in [src/main.cpp](src/main.cpp):
> <img width="500" height="110" alt="hardcoded maze of 1s and 0s" src="https://github.com/user-attachments/assets/a8db5f47-c6d3-4663-ace8-70f927a7f085" />

## Program dependencies
This program was made and tested using the SFML library in version 2.6.1.
>[!NOTE]
>When writing this, the latest SFML version available is v3.0.0, but there should be no problems when compiling with this version.

To install it, in Ubuntu and other linux distros with the `apt` packet manager, one can use:  
```bash
$ sudo apt install libsfml-dev
```

## How can you try this program?
This was developed using Ubuntu 24.04, so the binaries available in the `bin` folder can be downloaded and executed in similar operating systems using:
```bash
2d-raycasting-sim/bin$ chmod +x main
2d-raycasting-sim/bin$ ./main
```

In case you want to tweak the program's parameters, or change it for any other reason, I have included a [makefile](./makefile), which can be executed using:
```bash
2d-raycasting-sim$ make
```
After this, the binary called `main` in the `bin` folder will reflect the changes you made to the code.

> [!NOTE]
> For compilation and testing in windows, see [Compile_Windows.md](Compile_Windows.md) (not done yet).
