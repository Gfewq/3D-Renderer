# 3D Ray Tracer

A fully functional 3D ray tracer written in C that renders colored spheres with realistic lighting, shading, and anti-aliasing. The program takes a scene description as input and outputs a `.ppm` image generated entirely from scratch using vector math and ray tracing techniques.

## 🖼️ Overview
The final program simulates light rays being cast from a camera into a 3D world of spheres. Each pixel’s color is determined by intersections between rays and objects, light intensity, and shadow casting. Anti-aliasing smooths out jagged edges by averaging multiple samples per pixel.  
The result is a crisp, shaded 3D image with smooth color blending and soft shadows.

Example output:

<img src="assets/FS12.png" width="600"/>

---

## ⚙️ Build Instructions

Make sure you have `gcc` and `make` installed.

The Makefile automatically compiles with all required source files and links the math library (-lm).

To compile the project:

``` bash

make
``` 

##  ▶️ How to Run

Run the final renderer with:

``` bash
./FS_assg <input_file> <output_file.ppm>
```


Example:

```bash
./FS_assg scenes/scene1.txt output.ppm
```

You can then open the generated .ppm file using:

- The provided viewppm viewer (for quick terminal previews), or

- Any image viewer that supports PPM files (e.g. VS Code PPM plugin, or an online PPM viewer).

## 🧩 Input Format

The input text file describes the scene setup, including camera, light, colors, and sphere data. 

Example:

```bash
640 480
2.0
1.0
20.0 20.0 10.0 1000.0
4
0x1188EE 0xDD2266 0xDD7700 0x11CC66
0
3
2.0 0.0 -5.0 2.0 2
-2.0 0.0 -5.0 2.0 3
0.0 -102.0 -5.0 100.0 1
```


Where:

- First two lines specify image and viewport size

- Light position and brightness follow

- Next is the color palette (in HEX)

- Then the background color index

- Finally, the list of spheres (position, radius, and color index)

## 🌈 Features

- Realistic Ray Tracing — Simulates rays cast from a virtual camera to compute pixel colors.

- Accurate Lighting & Shadows — Calculates shading based on light intensity and occlusion.

- Anti-Aliasing — 9-sample grid smoothing for each pixel.

- Full Color Rendering — Converts HEX colors to RGB and applies per-object coloring.

- Modular Design — Separate files for vectors, spheres, and color operations for maintainability.

## 📦 Output

The final image is written to a .ppm file in ASCII format (P3).
Each pixel contains RGB values between 0–255, representing the final rendered color.

Example output image:

<img src="assets/FS11.png" width="600"/>

End Result:
A compact, modular, and mathematically grounded 3D rendering engine written entirely in C — capable of producing shaded, anti-aliased sphere scenes from simple text input.
