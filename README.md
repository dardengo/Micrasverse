
<div align="center" >

![micras_simulation_blue](https://github.com/Team-Micras/micras_simulation/assets/62271285/655d90d7-ae21-47df-b6ab-64d46ef4a559)

A micromouse simulator built for [Micras](https://github.com/Team-Micras/), the micromouse!

[![forthebadge](https://forthebadge.com/images/badges/made-with-c-plus-plus.svg)](https://forthebadge.com)
[![forthebadge](https://forthebadge.com/images/badges/built-with-grammas-recipe.svg)](https://forthebadge.com)
[![forthebadge](https://forthebadge.com/images/badges/60-percent-of-the-time-works-every-time.svg)](https://forthebadge.com)

</div align="center">

![Animated example of simulation with Micras going forward, then back, and then turning around its center. The GIF shows charts plotting motors' electrical current, motor's angular velocity, Micras' angular velocity, Micras' linear velocity, Micras' angular position, and distance sensors' readings](./docs/assets/Micrasverse%202025-01-14%2023-17-55.gif)


## ✨ Features
- Maze generated through easy-to-modify .txt files.
- Attachable sensors and actuators.
- Simple DC motor model with adjustable parameters.
- Renderable distance sensors.
- DIP switch.
- Addressable RGB.
- Fan control.
- Real-time interactive charts.
- Can run headless.


## 📁 Folder structure

- **.vscode/** - Visual Studio Code configuration files.
- **docs/** - Assets for this file.
- **external/**, **include/** and **lib/** - Libraries' files used in the project.
- **src/** - Micrasverse source code.


## 📚 Dependencies
Micrasverse depends on the following libraries:

| Name  | Description                     | License     |
| ------| --------------------------------| ------------|
| [Box2D](https://github.com/erincatto/box2d) | Used for 2D physics          | MIT         |
| [GLFW](https://www.glfw.org/)  | Used for window management   | zlib/libpng |
| [ImGui](https://github.com/ocornut/imgui) | Used for GUI overlay         | MIT         |
| [ImPlot](https://github.com/epezent/implot) | Used for generating graphs         | MIT         |
| [GLM](https://github.com/g-truc/glm)   | Used for vector math | MIT         |
| [Mazefiles](https://github.com/micromouseonline/mazefiles)   | Used for generating the maze |          |

⚡ If you're running it headless, only [Box2D](https://github.com/erincatto/box2d) and the maze file is needed.

## 🙏 Credits
This project was inspired by [Artful Bytes' Bots2D](https://github.com/artfulbytes/bots2d) and built with the help of [Michael Grieco's](https://www.youtube.com/@MichaelGrieco) awesome openGL tutoritals.
