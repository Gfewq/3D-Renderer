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

```make```

##  ▶️ How to Run

Run the final renderer with:

./FS_assg <input_file> <output_file.ppm>


Example:

./FS_assg scenes/scene1.txt output.ppm

You can then open the generated .ppm file using:

The provided viewppm viewer (for quick terminal previews), or

Any image viewer that supports PPM files (e.g. VS Code PPM plugin, or an online PPM viewer).
