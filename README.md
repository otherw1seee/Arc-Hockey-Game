# 🏒 Arc Hockey

A fast-paced local multiplayer arcade game inspired by classic air hockey, developed in *C++* using the *SFML* library.

---

## 🎮 About

**Arc Hockey** is a two-player arcade game where players compete by hitting the puck into the opponent's goal. The game features progressive puck acceleration, temporary power-ups, a clean minimalist interface, and smooth gameplay.

The project was developed as a university coursework project to demonstrate object-oriented programming principles and game development using *C++* and *SFML*.

---

## ✨ Features

- Local multiplayer (2 players)
- Progressive puck acceleration after every paddle collision
- Random bonus spawning system
- Circular timers for active bonuses
- Real-time score tracking
- Countdown before the match starts
- Victory screen
- Main menu
- Minimalistic arcade-style design

---

## 🎁 Available Bonuses

| Bonus | Effect |
|-------|--------|
| 🟢 Bigger Paddle | Increases your paddle size |
| 🔴 Double Points | The next goal gives 2 points |
| 🔵 Faster Puck | Temporarily increases puck speed |
| 🟡 Smaller Opponent Paddle | Reduces the opponent's paddle size |

Every active bonus is displayed at the top of the screen together with a circular countdown indicator showing its remaining duration.

---

## 🎮 Controls

### Left Player

- **W** — Move Up
- **S** — Move Down

### Right Player

- **↑** — Move Up
- **↓** — Move Down

### General Controls

- **Enter** — Start Game / Restart
- **Esc** — Return to Main Menu
- **Esc (Main Menu)** — Exit Game

---

## 📥 Download

Download the latest installer here:

➡️ **https://github.com/otherw1seee/Arc-Hockey/releases/latest**

---

## 🛠 Built With

- C++
- SFML 2.6
- Microsoft Visual Studio 2022

---

## 🚀 Build Instructions

1. Clone the repository.

```bash
git clone https://github.com/YourUsername/Arc-Hockey.git
```

2. Open the solution in **Microsoft Visual Studio 2022**.

3. Make sure **SFML 2.6** is installed and linked correctly.

4. Select:

- **Configuration:** Release
- **Platform:** x64

5. Build the project and run the executable.

---

## 📁 Project Structure

```
Arc-Hockey
├── assets/                 # Game resources
│   ├── fonts/
│   ├── icons/
│   
│
├── Headers/                # Header files
│   ├── Ball.h
│   ├── Bonus.h
│   ├── BonusManager.h
│   ├── BonusType.h
│   ├── Consts.h
│   ├── Game.h
│   ├── GameState.h
│   ├── Paddle.h
│   ├── PlayerSide.h
│   └── ServeDirection.h
│
├── Files/                    # Source files
│   ├── Ball.cpp
│   ├── Bonus.cpp
│   ├── BonusManager.cpp
│   ├── Game.cpp
│   ├── Paddle.cpp
│   └── main.cpp
```

---

## 🔮 Future Improvements

- 🤖 Single-player mode with AI
- 🔊 Sound effects and background music
- 🌐 Online multiplayer
- 🎨 Additional themes and arenas

---

## 📄 License

This project was created for educational purposes.
