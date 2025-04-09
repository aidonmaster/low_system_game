# 🐍 Low System Snake Game

A Snake-style game written in C++, running directly in the terminal and simulating a low-level system with manual memory allocation, pseudo registers, and ANSI escape-based rendering.

## 🧠 Concept

This project simulates a low-resource system environment — perfect for studying memory management, terminal I/O, and control structures in a near bare-metal style.  
All of that while enjoying the classic **Snake** game!

## 🚀 Features

- Simulated **memory pool** for managing snake and object allocations    
- **Pseudo-registers** for storing game state and movement    
- ASCII/ANSI **rendering directly in the terminal**    
- Keyboard input handling (raw mode)    
- Lightweight, efficient code    
- No external graphics libraries — just your terminal!
    

## 🕹️ Controls

|Key|Action|
|---|---|
|`W`|Move Up|
|`A`|Move Left|
|`S`|Move Down|
|`D`|Move Right|
|`Q`|Quit Game|


## 🧰 Building

Requirements:

- C++ compiler (e.g. g++)    
- Make
    

bash


## ⚠️ Note

> After exiting the game, if your terminal appears "glitched" (e.g., overlapping text or invisible input), run `reset` or press `Ctrl+L` to clear it.  
> We're working on automatic terminal state restoration.


## 🛠️ Upcoming improvements

-  Restore terminal state automatically    
-  Add scoring system and HUD using pseudo-registers    
-  Add obstacles or power-ups    
-  Pause functionality
    

## 📜 License

MIT © [aidonmaster](https://github.com/aidonmaster)

---
