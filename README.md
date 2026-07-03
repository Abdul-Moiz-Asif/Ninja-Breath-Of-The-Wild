# Ninja, Breath of the Wild: 2D Game Engine

## Overview
A high-performance 2D endless runner game engineered in **C++** utilizing the **SFML** (Simple and Fast Multimedia Library) framework[cite: 5]. The project implements a custom object-oriented physics engine, real-time input handling, and dynamic memory management for entity lifecycles.

## Core Technical Implementations

### 1. Physics & Game Loop
* **Delta-Time Execution:** Engineered a frame-rate independent game loop capped at 60 FPS, utilizing delta-time calculations to ensure smooth and consistent physics execution across varied hardware[cite: 5].
* **Kinematics & Gravity:** Implemented a real-time physics simulation featuring a constant gravity vector (`300.0f`) and vertical velocity calculations for jumping mechanics[cite: 5]. 
* **State Machine Logic:** Managed complex player states (grounding, airborne) and implemented strict boolean validation for mechanics like double-jumping[cite: 5].

### 2. Entity Management & Dynamic Spawning
* Managed runtime entity lifecycles (platforms, collectibles, and spikes) using C++ Standard Template Library (`std::vector`) memory containers[cite: 5].
* Developed dynamic pseudo-random spawning algorithms utilizing interval timers to continuously generate varied obstacles and terrain heights without memory leaks[cite: 5].

### 3. Collision Detection & Rendering
* **AABB Collision:** Implemented precise Axis-Aligned Bounding Box (AABB) intersection pipelines (`getGlobalBounds().intersects()`) to handle ground alignment, point collection, and game-termination triggers[cite: 5].
* **Parallax Scrolling:** Built an infinite scrolling visual wrapper utilizing parallel dual-sprite background movement and coordinate resets to maintain seamless execution without performance degradation[cite: 5].
* **Dynamic Input Handling:** Engineered real-time event polling to manipulate game state, allowing the player to toggle scrolling speeds (`BOOSTED_SCROLL_SPEED`) via continuous keyboard inputs[cite: 5].
