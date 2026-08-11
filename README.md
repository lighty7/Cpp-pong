# 🏓 Ping-Pong (C++ GUI Edition)

[![C++ Standard](https://img.shields.io/badge/C%2B%2B-17-blue.svg)](https://isocpp.org/)
[![CMake](https://img.shields.io/badge/CMake-3.16%2B-brightgreen.svg)](https://cmake.org/)
[![GUI Engine](https://img.shields.io/badge/SDL2-2D%20GUI-blue.svg)](https://www.libsdl.org/)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

An industry-standard, object-oriented **2D Graphical Ping-Pong game written in modern C++17** using **SDL2**. Features interactive mouse/keyboard navigation, 60 FPS hardware rendering, custom neon visual themes, and an intelligent Bot opponent with 3 difficulty levels.

This repository serves as both a production-ready application and an **in-depth C++ tutorial reference** explaining Object-Oriented Programming (OOP), RAII, SDL2 2D hardware graphics rendering, trajectory prediction, and modern CMake build pipelines.

---

## 📚 Tutorial & Language Reference Guide

### 1. Key C++ Concepts Demonstrated

| Concept | File Location | Description & Syntax Reference |
| :--- | :--- | :--- |
| **SDL2 2D Hardware Graphics** | [`Renderer.cpp`](src/Renderer.cpp) | `SDL_RenderFillRect`, `SDL_SetRenderDrawColor`, and double-buffering. |
| **Encapsulation & Visibility** | [`Ball.hpp`](include/Ball.hpp), [`Paddle.hpp`](include/Paddle.hpp) | Private data members (`m_x`, `m_y`) protected behind getter methods (`getX() const`). |
| **Const Correctness** | [`Ball.hpp`](include/Ball.hpp) | Marking getter methods with `const` guarantees no instance state mutation. |
| **Header Guards** | [`Config.hpp`](include/Config.hpp) | `#ifndef CONFIG_HPP ... #define ... #endif` avoids header redefinition errors. |
| **Namespaces** | [`Config.hpp`](include/Config.hpp) | `namespace PingPong { ... }` prevents identifier collisions in global scope. |
| **Scoped Enums** | [`Config.hpp`](include/Config.hpp) | `enum class BotDifficulty { Easy, Medium, Hard };` provides strongly typed enumerations. |

---

## 🏗️ Architecture & Component Design

```
                     +--------------------------+
                     |        main.cpp          |
                     +------------+-------------+
                                  |
                                  v
                     +--------------------------+
                     |        Game.cpp          |
                     +----+-------+--------+----+
                          |       |        |
        +-----------------+       |        +-----------------+
        |                         v                          |
+-------+--------+       +----------------+        +---------+------+
|    Ball.cpp    |       |   Paddle.cpp   |        |  Renderer.cpp  |
+----------------+       +----------------+        +----------------+
(Kinematics &    )       (Player & Bot    )        (SDL2 2D         )
(Bounce Physics  )       (Tracking Logic  )        (Hardware Render )
```

### Bot Opponent Algorithm
The Bot paddle uses an adaptive tracking heuristic based on the selected difficulty:
1. **Easy**: Delays tracking until the ball moves towards its boundary (`dirX > 0`) and moves at 55% speed.
2. **Medium**: Moves at 80% speed with center alignment deadzones to prevent jittering.
3. **Hard**: Predicts ball trajectory intercept coordinates at 110% speed with linear trajectory estimation.

---

## 🚀 Getting Started

### Prerequisites

#### Native Build:
- **C++ Compiler**: GCC 9+, Clang 10+, or MSVC supporting C++17.
- **Build System**: CMake 3.16+ and Ninja or Make.

#### Docker Build:
- **Docker Engine** (or Docker Desktop) & `docker-compose`.

---

## 🛠️ Building & Running Natively

```bash
# 1. Clone the repository
git clone https://github.com/your-username/cpp-pingpong.git
cd cpp-pingpong

# 2. Generate build scripts via CMake
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release

# 3. Compile executable and unit tests
cmake --build build --config Release

# 4. Run automated physics unit tests
ctest --test-dir build --output-on-failure

# 5. Launch the game!
./build/pingpong
```

---

## 🐳 Running via Docker (Any Machine)

Docker ensures the game runs on **any Linux, macOS, or Windows host** without installing C++ compilers.

### Option A: Using `docker-compose` (Recommended)
```bash
# Build and run interactively
docker-compose run --rm cpp-pingpong
```

### Option B: Using `docker` CLI directly
```bash
# Build Docker image
docker build -t cpp-pingpong .

# Run container interactively with TTY allocated
docker run -it --rm cpp-pingpong
```

---

## 🎮 Game Controls

| Key | Action |
| :---: | :--- |
| `W` | Move Player Paddle **Up** |
| `S` | Move Player Paddle **Down** |
| `P` | **Pause / Resume** Game |
| `Q` | **Quit** Game |

---

## 🧪 Testing

The codebase includes automated unit tests covering collision detection, angle reflections, speed increments, and score management:

```bash
./build/test_physics
```

---

## 📄 License

Distributed under the **MIT License**. See [`LICENSE`](LICENSE) for details.
