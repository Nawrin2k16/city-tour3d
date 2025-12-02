
# CITY TOUR 3D

## How To Run The Code ##
This is an Opengl based Computer Graphics Game. The texture and sounds files are kept in the D drive folder. If you want to change the file locations, update the path in cpp file. The game can be played in two ways-with keyboard or mouse pointer. To play the game using keyboard follow the instructions in the instruction bar. To play with mouse pointer, 'right click' on the mouse after starting the game and point to the direction you want the car to follow.

# README.md

## 3D Race Game — OpenGL (Legacy GLUT Version)

This is the **original version** of the project built using **legacy OpenGL (Immediate Mode)**, **FreeGLUT**, and **Win32-exclusive audio**. *This version can only be compiled and executed on **Windows*** because it relies on `PlaySound` from the Windows API and `.wav` playback via `winmm.lib`.

---

# **Dependencies**

### Required (Windows only)

| Dependency                | Purpose             | Notes                                 |
| ------------------------- | ------------------- | ------------------------------------- |
| **FreeGLUT**              | Windowing & Input   | Must be linked against `freeglut.lib` |
| **OpenGL (opengl32.lib)** | Rendering           | Comes with Windows SDK                |
| **GLU (glu32.lib)**       | Legacy utilities    | Also part of Windows SDK              |
| **WinMM (winmm.lib)**     | Audio playback      | For `PlaySound()`                     |
| **BmpLoader** (included)  | BMP texture loading | Uses raw pixel data                   |
| **C++ Compiler**          | Building            | MinGW-w64, MSVC, or CodeBlocks        |

### Optional Tools

* CodeBlocks IDE
* Visual Studio 2019/2022
* MinGW / MSYS2

---

# **Project Structure**

```
/images
    (all your .bmp texture files)

/Sounds
    (all .wav files used for engine, UI, win/lose, etc.)

main.cpp
BmpLoader.h
BmpLoader.cpp  (if exists)
README.md
```

---

# **How To Build & Run (Windows only)**

## Method 1 — Using **CodeBlocks**

1. Open CodeBlocks
2. Create a new “Empty Project”
3. Add `main.cpp` and `BmpLoader.cpp`
4. Go to **Project → Build Options → Linker settings**
5. Add these libraries:

```
freeglut
opengl32
glu32
winmm
```

6. Make sure your compiler can find:

   * `freeglut/include/`
   * `freeglut/lib/`

7. Copy the following DLLs next to your `.exe` (if using MinGW):

```
freeglut.dll
```

8. Make sure the folders `images/` and `Sounds/` are placed **next to your executable**.

9. Build & Run.

---

## ✅ Method 2 — Using **Visual Studio (MSVC)**

### 1. Create a new Win32 Console Application

No CLR, no MFC.

### 2. Add your `.cpp` files to the project.

### 3. Configure Include/Library Directories (Project → Properties):

```
C/C++ → Additional Include Directories:
    path_to_freeglut/include

Linker → Additional Library Directories:
    path_to_freeglut/lib
```

### 4. Link the required libs:

```
opengl32.lib
glu32.lib
freeglut.lib
winmm.lib
```

### 5. Place `freeglut.dll` next to your `.exe`.

### 6. Ensure the project directory contains:

```
/images
/Sounds
```

### 7. Build & Run.
---

# **Important Notes**

### This project **cannot run on macOS or Linux**

Because:

1. Uses **Win32 PlaySound()**
2. Uses **winmm.lib**
3. Expects **.bmp** textures via `GL_BGR_EXT`, which depends on Windows GL headers
4. Uses deprecated `GLUT` functions not supported by macOS system OpenGL

A modern portable version requires:

* GLFW
* GLAD
* Shader-based rendering
* stb_image
* OpenAL (or SDL2) for sound
* Core OpenGL 3.3+

---

#  **Troubleshooting**

### ** Program window opens but textures are missing**

You must keep the directory structure:

```
YourExecutable.exe
/images/*.bmp
/Sounds/*.wav
```

### ** “PlaySound failed”**

Ensure all `.wav` files exist and paths in `main.cpp` are correct.

### ** Black screen**

Usually caused by:

* Wrong path to textures
* Missing BMP files
* freeglut not found
* Depth test not enabled

### ** The executable closes instantly**

Run from command prompt:

```
cmd → Run → 1607028.exe
```

to see the error output.

---

## What this game has ##
+ Dynamic day and night mode.
+ Collision detection and reward count.
+ Various lighting effect
+ Both mouse and keyboard control
+ Added sound effect

## How To Play The Game ## 
+ If you stop auto-forwarding of the car, your score will be deducted as long as you don't start auto forwarding again.
+ To win the game you have to return the starting position and your score must be above 1000
+ Return to the starting point before fuel exhusts within allowed number of moves
+ Collition with any high object or falling into river or lake will result in GAME OVER
+ If car fuel gets empty or the allowed moves exceed, it will be GAME OVER
+ Collect stars to replenish fuel and slow down the car while collecting stars
+ While fuel is above 900, collecting more stars will add penalty by deducting the remaining number of allowed moves
+ Score will go up with the distance covered and the speed of the car

![1607028_Project-3D RACE 6_15_2021 6_56_06 AM](https://user-images.githubusercontent.com/45265281/121977019-da8ef800-cda6-11eb-92ac-ad4597d193ce.png)
![1607028_Project-3D RACE 6_15_2021 4_37_56 AM](https://user-images.githubusercontent.com/45265281/121968667-88dd7200-cd94-11eb-9bf1-0c825df912cb.png)
![1607028_Project-3D RACE 6_15_2021 4_30_34 AM](https://user-images.githubusercontent.com/45265281/121968195-b0800a80-cd93-11eb-86ee-daecccd3d698.png)
![1607028_Project-3D RACE 6_15_2021 4_30_37 AM](https://user-images.githubusercontent.com/45265281/121968217-baa20900-cd93-11eb-92ec-0cf92b791e62.png)
![1607028_Project-3D RACE 6_15_2021 4_30_59 AM](https://user-images.githubusercontent.com/45265281/121968277-db6a5e80-cd93-11eb-9019-63299cc3e374.png)
![1607028_Project-3D RACE 6_15_2021 4_32_48 AM](https://user-images.githubusercontent.com/45265281/121968531-503d9880-cd94-11eb-873a-fe8c76bc58fb.png)
![1607028_Project-3D RACE 6_15_2021 4_33_31 AM](https://user-images.githubusercontent.com/45265281/121968560-5df31e00-cd94-11eb-9b39-304a067f4477.png)
![1607028_Project-3D RACE 6_15_2021 4_34_02 AM](https://user-images.githubusercontent.com/45265281/121968586-6b100d00-cd94-11eb-9e3c-4c730c9fb47f.png)
![1607028_Project-3D RACE 6_15_2021 4_34_17 AM](https://user-images.githubusercontent.com/45265281/121968593-6e0afd80-cd94-11eb-953c-6f26e02eaaa5.png)
![1607028_Project-3D RACE 6_15_2021 4_34_28 AM](https://user-images.githubusercontent.com/45265281/121968601-706d5780-cd94-11eb-94c9-df2cbdffecf4.png)
![1607028_Project-3D RACE 6_15_2021 4_34_48 AM](https://user-images.githubusercontent.com/45265281/121968612-73684800-cd94-11eb-8c66-8ab512339394.png)
![1607028_Project-3D RACE 6_15_2021 4_35_01 AM](https://user-images.githubusercontent.com/45265281/121968618-75caa200-cd94-11eb-970e-8f5b902694dc.png)
![1607028_Project-3D RACE 6_15_2021 4_35_14 AM](https://user-images.githubusercontent.com/45265281/121968626-795e2900-cd94-11eb-8236-7d17cf836e54.png)
![1607028_Project-3D RACE 6_15_2021 4_35_56 AM](https://user-images.githubusercontent.com/45265281/121968631-7c591980-cd94-11eb-9d9a-ec1ad690510b.png)
![1607028_Project-3D RACE 6_15_2021 4_36_23 AM](https://user-images.githubusercontent.com/45265281/121968636-7ebb7380-cd94-11eb-8d86-3133df1a7168.png)
![1607028_Project-3D RACE 6_15_2021 4_36_46 AM](https://user-images.githubusercontent.com/45265281/121968642-81b66400-cd94-11eb-90a3-02122d5082c2.png)
![1607028_Project-3D RACE 6_15_2021 4_36_55 AM](https://user-images.githubusercontent.com/45265281/121968652-8418be00-cd94-11eb-9c3f-fd35108b7ca7.png)
![1607028_Project-3D RACE 6_15_2021 4_37_19 AM](https://user-images.githubusercontent.com/45265281/121968659-867b1800-cd94-11eb-9176-e5f5def8feb5.png)
Game video link : https://drive.google.com/file/d/1O3Mitq5Bi5kfhABlvLDLXPzgzV6_9iqo/view?usp=sharing
