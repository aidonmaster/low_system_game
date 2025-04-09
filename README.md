🐍 low_system_game
A Snake-style game written in C++, running directly in the terminal and simulating a low-level system with manual memory allocation, pseudo registers, and ANSI escape-based rendering.

🧠 Concept
This project simulates a low-resource system environment — perfect for studying memory management, terminal I/O, and control structures in a near bare-metal style.
All of that while enjoying the classic Snake game!

🚀 Features
Simulated memory pool for managing snake and object allocations
Pseudo-registers for storing game state and movement
ASCII/ANSI rendering directly in the terminal
Keyboard input handling (raw mode)
Lightweight, efficient code
No external graphics libraries — just your terminal!

🕹️ Controls
Key	Action
W	Move Up
A	Move Left
S	Move Down
D	Move Right
Q	Quit Game
Terminal must be in raw mode for real-time input to work correctly.

🧰 Building
Requirements:

C++ compiler (e.g. g++)

Make

bash
Copiar
Editar
make
./low_system_game
⚠️ Note
After exiting the game, if your terminal appears "glitched" (e.g., overlapping text or invisible input), run reset or press Ctrl+L to clear it.
We're working on automatic terminal state restoration.

📂 Project Structure
bash
Copiar
Editar
low_system_game/
├── src/               # Source code
├── Makefile           # Build script
├── README.md          # This file
└── LICENSE            # MIT License
🛠️ Work in Progress
 Restore terminal state automatically

 Add scoring system and HUD using pseudo-registers

 Add obstacles or power-ups

 Pause functionality

📜 License
MIT © aidonmaster

