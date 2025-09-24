# Raster Image Editor

A simple **console‑based raster image editor** written in C++, supporting **PPM**, **PGM**, and **PBM** formats.

## Table of Contents

- [Overview](#overview)  
- [Features](#features)  
- [Project Structure](#project-structure)  
- [Building & Running](#building--running)  
- [Usage / Commands](#usage--commands)  
- [Examples](#examples)  
- [Design & Architecture](#design--architecture)  
- [License](#license)  

---

## Overview

Raster Image Editor is a lightweight console application that allows you to read, manipulate, and save raster images in basic formats (PPM, PGM, PBM).  
It is intended for educational purposes or as a small utility to demonstrate image processing concepts in C++.

---

## Features

- Read and write PPM (color), PGM (grayscale), and PBM (black & white) images  
- Support for basic image transformations (rotate, flip, etc.)  
- Command‑line interface (no GUI)  
- Modular design to allow adding new operations or formats  
- Example images included (e.g. `tiger.ppm`, `ship.pgm`, `camera.pbm`)  

---

## Project Structure

```
Raster-Image-Editor/
├── core/             # core logic and image processing classes
├── transforms/       # modules for image transforms (rotate, flip, etc.)
├── images/           # image design
├── main.cpp          # entry point and command interface
├── Diagram.png       # architecture or workflow diagram  
└── …                 # additional files, headers, etc.
```

Some important files:

- `main.cpp` — the application entry point  
- `core/` — contains core image data structures, I/O routines  
- `transforms/` — image transformation operations  
- `images/` — image containters
- `Diagram.png` — visual representation of modules or process  

---

## Building & Running

These instructions assume you have a C++ compiler (e.g., `g++`) and `make` (or you can compile manually).

### On Linux / macOS / WSL

1. Open terminal and go into project directory:
   ```bash
   cd path/to/Raster-Image-Editor
   ```

2. Compile (example):
   ```bash
   g++ -std=c++11 main.cpp core/*.cpp transforms/*.cpp -I core -I transforms -o raster_editor
   ```

   (You may also write a **Makefile** or use a build system like CMake for convenience.)

3. Run:
   ```bash
   ./raster_editor
   ```

### On Windows (with MinGW / Visual Studio)

- For MinGW: similar command using `g++` from mingw prompt.  
- Or create a Visual Studio project, add the `.cpp` and `.h` files, set include paths, compile and run from inside Visual Studio.

---

## Usage / Commands

Once running, the console app may prompt you for commands or arguments. Example command syntax:

```
load <filename>       # Load an image file (PPM / PGM / PBM)
save <filename>       # Save current image
rotate <angle>        # Rotate by given degrees (e.g. 90, 180, 270)
flip <horizontal|vertical>  # Flip image
crop x y width height # Crop a region
help                  # Show available commands
exit / quit           # Exit the program
```

You can chain commands or apply transformations in sequence.

---

## Examples

1. Load a PPM:
   ```
   > load tiger.ppm
   Loaded image: tiger.ppm
   ```

2. Flip horizontally:
   ```
   > flip horizontal
   ```

3. Rotate 90 degrees:
   ```
   > rotate left
   ```

4. Save result:
   ```
   > save tiger_rotated.ppm
   ```

You can try with the included images in `images/` directory (e.g. `ship.pgm`, `camera.pbm`).

---

## Design & Architecture

- **Core module** handles image data (pixel containers, header parsing, I/O for PPM, PGM, PBM)  
- **Transform modules** (rotate, flip, crop, etc.) apply operations on the core image data  
- **Main / interface** logic parses user commands, drives the workflow, calls core + transform modules  
- Easily extensible: to add a new transform or support another format, you can add a new module following existing patterns  

Diagram in `Diagram.png` helps illustrate module relationships and data flow.

---

## License

```
MIT License  
Copyright (c) 2025

Permission is hereby granted, free of charge, to any person obtaining a copy...
```

---
