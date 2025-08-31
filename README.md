# Sokoban Game - Enhanced Edition

## Overview
This is a modern C implementation of the classic Sokoban puzzle game, featuring:
- Multiple levels (4 included)
- Auto-solver (BFS-based, works for most levels)
- Undo system
- Interactive menu and buttons
- Level selection
- PNG-based tile graphics (Raylib)
- Animated transitions

## Features
- **Multi-Level System:** Play through 4 hand-crafted levels of increasing difficulty.
- **Auto-Solver:** Press `S` to let the game solve the current level automatically (may fail for complex levels).
- **Undo:** Press `U` to undo your last move.
- **Level Selection:** Choose any level to play from the menu.
- **Interactive Buttons:** Mouse-driven menu and navigation.
- **Animated UI:** Smooth fade-in and completion transitions.

## Controls
- **Arrow Keys:** Move the player
- **U:** Undo last move
- **R:** Restart current level
- **S:** Auto-solve current level
- **M:** Return to menu
- **ESC:** Exit game
- **Mouse:** Click buttons in menus

## Symbols
- `@` - Player
- `$` - Box
- `.` - Target
- `#` - Wall
- `*` - Box on target
- `+` - Player on target

## Assets
All tile graphics are stored in the `assets/` folder:
- `wall.png`, `floor.png`, `target.png`, `box.png`, `box_on_target.png`, `player.png`

## How to Run
1. **Requirements:**
   - [Raylib](https://www.raylib.com/) library installed
   - C compiler (GCC or MSVC)
2. **Build:**
   - Compile the source code with Raylib and GLFW linked
   - **GCC (MinGW) Example:**
     ```sh
     gcc -o "SOKOBAN GAME.exe" sokoban.c -lraylib -lopengl32 -lgdi32 -lwinmm
     ```
    - **MSVC (cl.exe) Example:**
       Open the x64 Native Command Prompt and run (replace `<RAYLIB_DIR>` and `<GLFW_DIR>` with your actual paths):
       ```bat
       cd /d <RAYLIB_DIR>
       cl /I src /I <GLFW_DIR>\include /DPLATFORM_DESKTOP /D_GNU_SOURCE "SOKOBAN GAME.c" src\rcore.c src\raudio.c src\rshapes.c src\rtextures.c src\rtext.c src\utils.c src\rmodels.c src\rglfw.c <GLFW_DIR>\lib-vc2022\glfw3.lib user32.lib gdi32.lib shell32.lib winmm.lib ole32.lib oleaut32.lib uuid.lib advapi32.lib /Fe:"SOKOBAN GAME.exe"
       ```
3. **Run:**
   - Double-click `SOKOBAN GAME.exe` or run from terminal

## Level Data
Levels are defined as arrays of strings in the source code. Each character represents a tile type. Four levels are included, ranging from simple to advanced.

## About
- **Author:** Sinobi Trinity(Arijit, Riddhi, Ifthe)
- **Year:** 2025
- **Version:** Enhanced Edition v1.1
- **Built with:** Raylib

## License
This project is provided for educational and personal use. See LICENSE for details.