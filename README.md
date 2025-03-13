# Monotile Visualizer

![](assets/readme/Tile_Morphing.gif)

This project is based on the paper [An aperiodic monotile](https://cs.uwaterloo.ca/~csk/hat/). As shown in the paper, the monotile is in fact a continuous spectrum of shapes capable of tiling an infinite plane without ever becoming periodic. In this spectrum, edge proportions and angles between them are constant, so it's possible to generate these tiles procedurally.

## Rendering with OpenGL

Although edges and vertices are easy to calculate, there is still a need to fill the shape using triangles. By morphing through the entire spectrum it is possible to determine what vertices can always be connected, this way we can determine the vertices that make up the triangles inside the shape.

![](assets/readme/Tile_Connections.gif)

## Running the project

#### First, make sure you have `GLFW` in your system includes.

```cmd
sudo apt-get install libglfw3-dev
```

#### The project is built with CMake. In the main directory, run:

```cmd
mkdir build
cd build
```

#### Inside the build directory, run:

```cmd
cmake ../
cmake --build .
```

#### And finally, run this to execute:

```cmd
./MonotileVisualizer
```

## Keybindings
- Q --- Increase [side length](https://arxiv.org/pdf/2303.10798#page=12)
- W --- Decrease side length
- E --- Toggle wireframe and vertex connections
- D --- Output side length and pairs of connected vertices