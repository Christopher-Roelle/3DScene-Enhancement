# 3D Scene Enhancement - Enhancement of CS-330-H7034
Capstone enhancement of my 3D scene

## Table of Contents
- [About](#about)
- [Enhancements](#enhancements)
- [Prerequisites](#prerequisites)
- [Building](#building)
- [Usage](#usage)
- [Controls](#controls)
- [Known Bugs](#known-bugs)

## About
Written to learn the intricacies of OpenGL, and how the framework can be used to write code that can run on the GPU and affect the frame buffers.
The code generates a 3D scene using just vertex data, and produces a mockup of some vases, a candle and some pumpkin objects.

## Enhancements
- Implemented a primitive 'Mesh' class that handles all shared functionality to reduce redundant code.
- Changed the code to allow objects to manage their textures rather than the programmer needing to swap texture banks.
- Enhanced logic to allow objects to notify the shader if they are using overlay textures (banks 2 and 3).
- Objects now handle their texture settings and set them during their Draw Method.
- Expanded modularity of objects in preparation for future updates.

## Prerequisites
- Visual Studio IDE (Or C++ Compiler)
- C++ 14 Compiler
- Git/Github for Desktop (Or Site Clone)

## Building
A pre-compiled x64 version is available on the Github page.
If you want to compile yourself, please follow the below steps:

### Pre-Steps
1. Create 'Includes' and 'Libs' directory on machine.
2. Download GLFW from: https://www.glfw.org/download.html and place the includes files in your 'Includes' and 'Libs' directories
3. Download GLAD from: https://glad.dav1d.de/ (Set language to C++, API to 3.3, Profile to Core, and tick 'Generate a Loader', then click Generate)
   Move both the glad and KHR folder to your 'Includes' directory. Move the glad.c file to the project root (My file may work, but I have found I have to replace with a new one on different machines)
4. Download GLEW from: https://glew.sourceforge.net/index.html and place it in your 'Libs' folder.
5. Download GLM from: https://glm.g-truc.net/0.9.8/index.html and place it in your 'Includes' folder.

### Build Steps
1. Clone this repository
    ```bash
    git clone https://github.com/Christopher-Roelle/3DScene-Enhancement.git
    ```
2. Navigate to project directory root
3. Open OpenGLSample.sln
4. Change Build to 'Release' and Architecture to desired (i.e. x64, x86, ARM)
5. Open 'Solution Explorer' (Ctrl+Alt+L)
6. Right click MyScene (Under Solution 'OpenGLSample'), Select Properties
7. On VC++ Directories tab, Add 'Includes' directory from Pre-Steps to Include Directories.
8. On VC++ Directories tab, Add 'Libs' directory from Pre-steps to Library Directories.
9. Expand Linker tab, then select Input tab.
10. Ensure Additional Dependencies include: 
```bash
glfw3.lib;opengl32.lib;glew32.lib;glu32.lib;
```
11. Click Apply and Ok.
12. Click Build (Ctrl+F5)
13. Copy 'textures' and 'ShaderFiles' folders from OpenGLSample to the build folder

## Usage
Double click the built .exe file. Ensure that both the 'textures' and 'shaderFiles' are in the same directory as the exe, otherwise the graphics will not initialize properly.

## Controls
ESC - Close Program
1 - Wireframe View
2 - Normal View
W - Move Forward
A - Move Left
S - Move Back
D - Move Right
Q | Left CTRL - Move Down
E | Spacebar - Move Up
F - Flashlight
Scroll Wheel Up - Increase Movement Speed
Scroll Wheel Down - Decrease Movement Speed
Scroll Wheel Button - Reset Movement Speed

## Known Bugs
- Backfaces cause flashlight to cull unintentionally, causing it to not show on certain faces of complex meshes.