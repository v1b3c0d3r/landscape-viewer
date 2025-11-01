# 3D Landscape Renderer

This is a simple 3D landscape rendering application created using C, OpenGL, and SDL. It loads a heightmap from a PNG image and renders it as a 3D mesh. The landscape can be rendered as a wireframe or with a texture.

## Prerequisites

To build and run this project, you will need the following dependencies:

```bash
sudo apt-get update
sudo apt-get install -y libcglm-dev libsdl2-dev libgl-dev libglew-dev
```

## Building

To build the project, run the following command:

```bash
make
```

This will compile the source code and create an executable file at `build/landscape`.

## Usage

To run the application, use the following command:

```bash
./build/landscape <heightmap.png> [texture.png]
```

- `<heightmap.png>`: The path to the heightmap image.
- `[texture.png]` (optional): The path to the texture image.

If a texture is not provided, the landscape will be rendered as a wireframe.

**Example:**

```bash
# Render as a wireframe
./build/landscape assets/heightmap.png

# Render with a texture
./build/landscape assets/heightmap.png assets/texture.png
```

## Controls

- **Mouse**: Look around
- **W, A, S, D**: Move the camera
- **ESC**: Exit the application
