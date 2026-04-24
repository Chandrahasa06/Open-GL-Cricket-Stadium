# 3D Cricket Stadium Simulation
### Group 29 — Computer Graphics Project

A fully interactive 3D cricket stadium built with **OpenGL** and **GLUT**, featuring textured grass, floodlight towers with light cones, crowd figures, on-field players, and 6-DOF keyboard navigation.

---

## Table of Contents
- [Prerequisites](#prerequisites)
- [Installation](#installation)
  - [Linux (Ubuntu / Debian)](#linux-ubuntu--debian)
  - [macOS](#macos)
- [Building & Running](#building--running)
- [Controls](#controls)
- [Project Structure](#project-structure)
- [Troubleshooting](#troubleshooting)

---

## Prerequisites

| Requirement | Version | Notes |
|-------------|---------|-------|
| C++ Compiler | GCC 7+ / MSVC 2017+ | Must support C++11 or later |
| OpenGL | 1.x / 2.x | Included with GPU drivers |
| GLU | Any | Usually bundled with OpenGL |
| GLUT / FreeGLUT | Any | See installation steps below |

---

## Installation

### Linux (Ubuntu / Debian)

**Step 1 — Update package list**
```bash
sudo apt update
```

**Step 2 — Install OpenGL, GLU, and FreeGLUT**
```bash
sudo apt install -y freeglut3 freeglut3-dev libgl1-mesa-dev libglu1-mesa-dev
```

**Step 3 — Verify installation**
```bash
dpkg -l | grep freeglut
# Should show freeglut3 and freeglut3-dev
```

---

### macOS

**Using Homebrew**
```bash
brew install freeglut
```

> **Note:** On macOS, GLUT is available as a system framework. If using the system GLUT, replace `-lglut` with `-framework GLUT -framework OpenGL` in the compile command.

---

## Building & Running

### Linux / macOS

**Step 1 — Clone or download the project**
```bash
git clone <repository-url>
cd stadium
# OR simply place stadium.cpp in a folder and navigate there
```

**Step 2 — Compile**
```bash
g++ stadium.cpp -o stadium -lGL -lGLU -lglut -lm
```

**Step 3 — Run**
```bash
./stadium
```

A window titled **"3D Stadium"** (800 × 600) will open immediately.

---


## Controls

| Key | Action |
|-----|--------|
| `W` | Move camera up |
| `S` | Move camera down |
| `A` | Move camera left |
| `D` | Move camera right |
| `Q` | Zoom in |
| `E` | Zoom out |
| `Z` | Move camera forward |
| `X` | Move camera backward |
| `↑` | Rotate view up |
| `↓` | Rotate view down |
| `←` | Rotate view left |
| `→` | Rotate view right |
| `L` | Toggle floodlights ON / OFF |
| `ESC` | Exit |

---

## Project Structure

```
stadium/
│
├── stadium.cpp          # Main source file (entire project)
└── README.md            # This file
```

All textures are **procedurally generated at runtime** — no external image files are required.

---

## Features Overview

- **Textured field** — procedural grass texture on outfield and infield (two distinct shades)
- **Textured concrete wall** — procedural noise-based grey texture on the outer cylinder
- **Seating tiers** — two frustum rings with 480 / 640 alternating red and yellow seats
- **Crowd** — coloured block figures placed at every seat position
- **On-field scene** — 2 batsmen, 1 umpire, 11 fielders, 3 cameramen, 1 bench, 2 drink carts
- **Wickets** — 3 stumps + bail at each end of the pitch
- **Floodlight towers** — 6 poles with 2×3 light grids, togglable with `L`
- **Light cones** — alpha-blended cones cast inward onto the field
- **Flag poles** — 18 flags on the roof outline (orange / white / green)
- **Boundary boxes** — 10 extruded 6-sided structures around the perimeter

---

## Authors

**Group 29**
- Rachakonda Chandrahasa
- Rahul Kumar
- Gugulothu Venu

