---Overview

This repository contains a software renderer implemented in C++ that reproduces several classical graphics techniques without relying on an external GPU pipeline. It is ideal for learning how vertex transforms, shading, and rasterization work end-to-end.

The renderer:

Loads and draws 3D models (OBJ or similar).

Implements programmable-like stages (vertex and fragment shader simulations).

Provides two per-vertex/per-fragment lighting models (Gouraud and Phong).

Uses SDL2 for display and input and glm for linear algebra.

Offers simple post-processing passes and primitives (points, lines, triangles).


---Features

Shader support

Vertex transform stage with configurable per-vertex attributes.

Fragment shading stage (simulated in CPU) for color & lighting computation.

Lighting

Gouraud shading: vertex lighting with interpolation across the triangle.

Phong shading: per-fragment normal interpolation with per-fragment lighting for smoother highlights.

Model loading

Wavefront .obj model loading (vertices, normals, uvs).

Materials

Basic material parameters (diffuse, specular, shininess).

Post-processing

Simple effects such as gamma correction, exposure tone mapping, and optional convolution passes (e.g., blur).

Primitives

Software rasterization of triangles, lines, and points with depth buffering.

Dependencies

SDL2 for windowing and input handling.

glm for math (vectors/matrices/quaternions).

Extras

Frame capture / screenshot support (optional).

Configurable rendering modes (wireframe, flat, textured).


---Installation

These are recommended installation steps — adapt as needed for your specific environment.

---Prerequisites

C++17 compatible compiler (Visual Studio 2019/2022, clang, or gcc).

CMake >= 3.16 (recommended).

SDL2 development libraries.

glm (header-only).

(Optional) vcpkg for Windows package management, or your system package manager on Linux/macOS.

Windows — Visual Studio + vcpkg (example)

Install Visual Studio 2022 (Desktop development with C++).

Install vcpkg
 and bootstrap it.

Install dependencies:
  .\vcpkg install sdl2:x64-windows glm
From the repo root:
  mkdir build
  cd build
  cmake .. -DCMAKE_TOOLCHAIN_FILE=C:/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake -A x64
  cmake --build . --config Release

Run the produced executable (e.g., Release\software_renderer.exe).

---Running the Project (Visual Studio Only)

Follow these steps to build and run the software renderer directly inside Visual Studio 2022 (or VS2019):

1. Open the Project

Launch Visual Studio.

Select File → Open → Folder…
(or Open Project/Solution if you use a .sln file).

Choose the root folder of the repository (OpenGL/).

2. Select the Build Configuration

In the top toolbar:

Set Configuration → Debug or Release

Set Platform → x64

Tip:
Debug is best for development;
Release gives full performance for screenshots/demos.

3. Ensure SDL2 and glm Are Detected

If you installed them using vcpkg, Visual Studio should automatically detect them when the "vcpkg" toolchain is configured.

To confirm:

Go to Project → Properties

Under VC++ Directories, ensure you see the include & library paths from vcpkg.

If they are missing, reinstall using:

SDL2

glm

4. Build the Project

There are several ways:

Press Ctrl + Shift + B

OR choose Build → Build Solution

OR click the hammer icon in the toolbar

Visual Studio will compile the program and show success/failure in the Output Window.

5. Run the Program

To launch the renderer:

Press F5 to Run with Debugging

Press Ctrl + F5 to Run without Debugging

OR click the green “Run” arrow in the toolbar

A window should appear, created by SDL, displaying your rendered scene.





--Technical details
Shader support

This project simulates a programmable pipeline in software. Shader code is represented by:

Vertex stage: transforms model-space positions to clip space using model/view/projection matrices. Vertex attributes (pos, normal, uv) are processed and any varying outputs (e.g., color for Gouraud) are passed to the rasterizer.

Fragment stage: executed per-pixel after interpolation. This is where Phong shading (per-fragment normal and lighting computation) and texturing are applied.


---Lighting techniques

Gouraud lighting

Computes lighting per-vertex in the vertex stage.

The computed colors are linearly interpolated across the triangle.

Advantage: cheaper; Disadvantage: misses specular highlights that don't fall on vertices.

Phong lighting

Interpolates normals across the triangle and computes lighting per-fragment.

Captures accurate specular highlights and smoother shading.

More expensive but higher quality.


--Materials & Texturing

Materials contain kd (diffuse), ks (specular), and shininess.

Textures are sampled in the fragment stage if available; bilinear sampling may be supported.

Tangent-space normal mapping is optional if normal maps are provided.

--Post-processing

After rasterization, a full-screen pass may apply:

Tone mapping (exposure/gamma)

Simple convolution kernels (blur)

Color grading (basic LUT or multiply)

Implementation: framebuffer is an image buffer that passes through the post-processing shader (CPU).

--Anti-aliasing (note)

Since this is a software rasterizer, anti-aliasing can be implemented through:

Supersampling (render at higher resolution & downsample).

MSAA-like sample masks (per-subpixel coverage).


<img width="1906" height="1113" alt="Screenshot 2025-11-25 110627" src="https://github.com/user-attachments/assets/ef3caa20-77a1-4188-b39b-7455be0064b8" />

<img width="1916" height="1132" alt="Screenshot 2025-11-19 160633" src="https://github.com/user-attachments/assets/f7813f09-80e7-4261-824b-0dd6295f5634" />

<img width="1278" height="1069" alt="Screenshot 2025-11-13 110531" src="https://github.com/user-attachments/assets/e667f781-6949-4224-aaaa-88d84079cbb0" />
<img width="1283" height="1067" alt="Screenshot 2025-11-13 110506" src="https://github.com/user-attachments/assets/2f4c32b4-e06f-46c1-91c5-c36da926972d" />

<img width="1283" height="1065" alt="Screenshot 2025-11-05 155611" src="https://github.com/user-attachments/assets/71168b72-426d-4ce5-9575-7c97ad379ad2" />
<img width="1278" height="1043" alt="Screenshot 2025-11-06 101214" src="https://github.com/user-attachments/assets/de09895f-7457-4dc4-81ae-c8d1a020538c" />
<img width="1283" height="1070" alt="Screenshot 2025-11-11 102452" src="https://github.com/user-attachments/assets/49273e45-7cd7-4ee6-9842-59c108c66ec2" />
<img width="1282" height="1063" alt="Screenshot 2025-11-11 102525" src="https://github.com/user-attachments/assets/bbff1419-54cb-4d92-a80b-f3f3965e5f51" />







--Attribution & third-party libraries

This project uses / references the following third-party code and libraries:

SDL2 — Simple DirectMedia Layer for windowing and input. (https://www.libsdl.org/)

glm — OpenGL Mathematics (header-only). (https://glm.g-truc.net/)

Wavefront OBJ loader code
