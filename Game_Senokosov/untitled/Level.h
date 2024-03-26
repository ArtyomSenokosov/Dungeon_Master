#ifndef LEVEL_H
#define LEVEL_H

#include <iostream>
#include <ncurses.h>
#include <string>
#include <list>
#include <cstdlib>
#include <fstream>
#include <filesystem>
#include <sstream>

#include "Enemies/Enemy.h"
#include "Enemies/Goblin.h"
#include "Enemies/Orc.h"
#include "Items/Armor.h"
#include "Items/Bow.h"
#include "Items/Shield.h"
#include "Chest.h"
#include "Merchant.h"
#include "Projectile.h"

using namespace std;

class Level {
public:
    static const int WIDTH = 100;
    static const int HEIGHT = 45;

    Level();

    void generateLevel();

    void generateNextLevel();

    bool isWall(int x, int y) const;

    bool isCollision(int x, int y) const;

    bool isExclamation(int x, int y) const;

    bool isQuestionMark(int x, int y) const;

    void setCurrentLevel(int level);

    list<Enemy *> getEnemies() const;

    list<Chest> &getChests() const;

    bool isChest(int x, int y) const;

    bool isChestOpened(int x, int y) const;

    void openChest(int x, int y);

    bool hasMerchant() const;

    const Merchant &getMerchant() const;

    void loadSaveFromFile(const string &filename);

    // Projectile methods
    void addProjectile(const Projectile &projectile);

    list<Projectile> &getProjectiles();

    void clearProjectiles();

    int getCurrentLevel() const;

    void generateRandomEnemies();

    void generateRandomChests();

    void generateMerchant();

    void loadEnemiesFromFile();

    void loadChestsFromFile();

    void loadFromFile(const string &filename);

    void saveToFile(const string &filename) const;

    void saveEnemiesToFile() const;

    void saveChestsToFile() const;

private:
    int currentLevel;
    char levelData[WIDTH][HEIGHT];
    list<Enemy *> enemies;
    list<Chest> chests;
    Merchant merchant;

    list<Projectile> projectiles;

    void generateRandomMap();

    void addEnemy(Enemy *enemy);

    bool isEnemy(int x, int y) const;

    void loadLevelIfExists();
};

#endif // LEVEL_H