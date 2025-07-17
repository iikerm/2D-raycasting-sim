# 2D Raycasting and Rendering Simulator
## What does the title mean?
The main idea behind this project is to make a program that can take a 'maze' composed of different objects and cast various rays of simulated light from a 'camera' that will collide in one way or another with these objects in the maze as the user moves the camera across the window. These collisions can then be interpreted according to their distance away from the camera in order to produce a dynamic image representing what the camera is seeing through its angle of vision.

## Why is this something useful to do as a programmer?
This project, as simple as it may be, helps you to focus on different approaches to find solutions for many kinds of problems, including some examples like:
- Working with point transformations in two-dimensional space
- Applying euclidean geometry to calculate distances
- Improving performance by not computing idle frames
- Choosing the best order in which things can be rendered, according to the performance of the different options available.

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
