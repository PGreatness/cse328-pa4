# PA4

Ahnaf Hasan

112776839

ahnaf.hasan@stonybrook.edu

## Overview

A template for PA4.

NOTE: 
Don't forget TODOs in shader sources. 
A sample implementation of the Phong shading model in OpenGL fragment shaders
is available in the sample code `OpenGLTessellationSphere`
for the Lecture on PA3. 

## Features Implemented

Check all features implemented with "x" in "[ ]"s. 
Only features or parts checked here would be graded! 

- [X] **P0: Global Functionalities** (See each object for display modes)
  - [X] Transformations
    - [X] Translation
    - [X] Rotation
    - [X] Scaling
    - [X] Shear
    - [X] Reflection
  - [X] Camera Functionalities
    - [X] Show/hide x, y, z Axes
    - [X] `W`/`S`/`A`/`D`/`UP`/`DOWN` Functionalities
- [X] **P1: Simple Polyhedral Objects**
  - [X] Tetrahedron
    - [X] Wireframe
    - [X] Flat
    - [X] Smooth
  - [X] Cube
    - [X] Wireframe
    - [X] Flat
    - [X] Smooth
  - [X] Octahedron
    - [X] Wireframe
    - [X] Flat
    - [X] Smooth
- [ ] **P2: Subdivision**
  - [X] Icosahedron
    - [X] Wireframe
    - [X] Flat
    - [X] Smooth
    - [X] Subdivision
  - [X] Ellipsoid
    - [X] Wireframe
    - [X] Flat
    - [X] Smooth
    - [X] Subdivision
  - [X] Dodecahedron
    - [X] Wireframe
    - [X] Flat
    - [X] Smooth
    - [X] Subdivision
  - [X] Torus
    - [X] Wireframe
    - [X] Flat
    - [X] Smooth
    - [X] Subdivision
- [ ] **P3: Tessellation**
  - [X] Simple Quadric Primitives
    - [X] Sphere
      - [X] Wireframe
      - [X] Flat/Smooth
    - [X] Cylinder
      - [X] Wireframe
      - [X] Flat/Smooth
    - [X] Cone
      - [X] Wireframe
      - [X] Flat/Smooth
  - [ ] Super-quadrics
    - [ ] Wireframe
    - [ ] Flat/Smooth
- [ ] **P4: Flight Simulation**
  - [ ] City Scene Assembly (Has 8-12 urban structures)
  - [ ] Display
    - [ ] Wireframe
    - [ ] Flat
    - [ ] Smooth
  - [ ] Loops
    - [ ] Horizontal Loop
    - [ ] Vertical Loop
- [ ] **P5: Bonus**
  - [ ] Normal Display Mode
  - [ ] Other (Please Specify)

## Usage

- Display modes
  - Please refer to `var/out/*.bmp` for the expected visual effect of different display modes. 
  - `var/out/normal.bmp` refers to `NORMAL_MAPPING` display mode, which is one of the BONUS extensions.
- Camera
  - Press `X` to show/hide the x, y, z axis. 
  - Press `W`/`S`/`A`/`D`/`UP`/`DOWN`, or drag/scroll the mouse to adjust the camera. 

## Appendix

Please note that the following some bugs that are known to exist in the code.

- P2: Torus
  - The torus's subdivisions cause the torus to snap back to its original orientation.
- P3: Simple Quadric Primitives
  - The interactivity is limited. The user can only translate the simple quadric primitives.
  - The caps of the shapes are not done.
