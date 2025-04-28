

<div align="center">
  <img src="https://github.com/Team-Micras/micras_simulation/assets/62271285/655d90d7-ae21-47df-b6ab-64d46ef4a559" alt="Micrasverse Logo"/>
  <p>A modular 2D micromouse simulator built for <a href="https://github.com/Team-Micras/Team-Micras">Micras</a></p>


[![forthebadge](https://forthebadge.com/images/badges/made-with-c-plus-plus.svg)](https://forthebadge.com)
[![forthebadge](https://forthebadge.com/images/badges/0-percent-optimized.svg)](https://forthebadge.com)
[![forthebadge](https://forthebadge.com/images/badges/powered-by-black-magic.svg)](https://forthebadge.com)
</div>

## Table of Contents
- [Overview](#overview)
- [Demo](#demo)
- [Features](#features)
- [Installation](#installation)
- [Quick Start](#quick-start)
- [Project Structure](#project-structure)
- [Dependencies](#dependencies)
- [Building & Running](#building--running)
- [License](#license)
- [Acknowledgements](#acknowledgements)
- [Contact](#contact)

## Overview
Micrasverse is an extensible, high-fidelity 2D micromouse simulation platform designed to prototype and test navigation algorithms on the Micras hardware before deployment. Powered by Box2D physics and OpenGL rendering, Micrasverse provides real-time visualization, modular sensors & actuators, and interactive data charts.

## Demo
![Micrasverse Demo](./docs/assets/Micrasverse%202025-04-27.gif)

## Features
- **Custom Mazes**: Generate and edit maze layouts through simple `.txt` files.
- **Modular Components**: Attach/detach sensors (distance sensors, DIP switches, addressable RGB LEDs) and actuators (motors, fans).
- **Physics Simulation**: Realistic DC motor model and collision handling via Box2D.
- **Interactive Charts**: Live plotting of motor current, velocity, position, sensor readings, and more with ImPlot.
- **User-Friendly GUI**: Control simulation parameters, start/pause/reset, and toggle components with ImGui.
- **Cross-Platform**: Build and run on Linux and Windows.

## Installation
### Prerequisites
- CMake ≥ 3.14
- C++17 compatible compiler (GCC, Clang, MSVC)
- OpenGL development libraries
- [GLFW3](https://www.glfw.org/) (window/input)
- [Box2D](https://github.com/erincatto/box2d) (2D physics)
- [ImGui](https://github.com/ocornut/imgui) & [ImPlot](https://github.com/epezent/implot) (GUI & plotting)
- [GLM](https://github.com/g-truc/glm) (math)

Clone the repository:
```bash
git clone https://github.com/Team-Micras/micras_simulation.git
cd micras_simulation
```

## Quick Start
Build and run in Release mode:
```bash
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
./bin/micrasverse --maze ../docs/mazes/demo_maze.txt
```

## Project Structure
```
micras_simulation/
├── CMakeLists.txt        # Project build configuration
├── include/              # Public headers
├── src/                  # Source code modules
├── external/             # Third-party libraries
├── docs/                 # Assets & documentation
└── README.md             # This file
```

## Dependencies
| Library           | Purpose                        | License      |
| ----------------- | ------------------------------ | ------------ |
| Box2D             | Physics simulation             | MIT          |
| GLFW              | Windowing & input              | zlib/libpng  |
| ImGui             | GUI overlay                    | MIT          |
| ImPlot            | Data plotting                  | MIT          |
| GLM               | Mathematics                    | MIT          |
| Mazefiles         | Maze file parsing              | MIT          |

## Building & Running
Refer to [BUILD.md](BUILD.md) for detailed build configurations, platform-specific instructions, and advanced options.



## License
MIT License

Copyright (c) 2025 Team Micras

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

## Acknowledgements
- Inspired by [Artful Bytes' Bots2D](https://github.com/artfulbytes/bots2d)
- OpenGL tutorials from [Michael Grieco](https://www.youtube.com/@MichaelGrieco)

## Contact
- Maintained by [Team Micras](https://github.com/Team-Micras)
