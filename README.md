# README - Game Project
This game project was developed to create an immersive gaming experience with several features, including item classes, games and levels, players, UI, enemies, merchants, and chests. The project is based on an object-oriented design and integrates with an existing game engine.
# Implemented Classes
## Object Classes
- Armor: represents armor that can be equipped with defense attributes.
- Bow: represents a bow that can be equipped with attack attributes.
- Shield: represents a shield that can be equipped with defense attributes.
- Sword: represents a sword that can be equipped with attack attributes.
## Game Classes and Levels
- Game: manages the overall logic of the game, including events, transitions between levels and game state.
- Level: represents a specific level of the game, with unique characteristics, enemies and objectives.
## Player Class
- Player: represents the character controlled by the player, with methods to manage movement, attack, inventory and other interactions.
## UI class
- UI: Manages the game's user interface, including graphics, menus, control keys, and display of game information.
## Enemy Classes
- Enemy: basic class to represent an enemy, with attributes such as health, damage, level and general behavior.
- Goblin: subclass of Enemy, represents a goblin with specific attributes.
- Orc: subclass of Enemy, represents an orc with specific attributes.
## Trader Class
- Merchant: Represents a merchant in the game, with location attributes and methods for managing items available for sale.
## Chest class
- Chest: represents a chest in the game, with location attributes and methods for opening it and obtaining a reward.
# Contributions
g++  Game.cpp Level.cpp main.cpp Merchant.cpp Chest.cpp Player.cpp Projectile.cpp UI.cpp Enemies/Enemy.cpp Enemies/Goblin.cpp Enemies/Orc.cpp Items/Armor.cpp Items/Bow.cpp Items/Item.cpp Items/Shield.cpp Items/Sword.cpp  -lncurses  -o  game
