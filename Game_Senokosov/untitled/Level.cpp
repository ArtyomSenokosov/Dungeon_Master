#include "Level.h"

Level::Level() : merchant(-1, -1) {
    // Generate the initial level
    generateRandomEnemies();
    currentLevel = 1;
    // Check if the level has a merchant
    if (hasMerchant()) {
        generateMerchant();
    }
}

void Level::generateLevel() {
    // Generate a new level with a random map
    generateRandomMap();
}

void Level::generateNextLevel() {
    // Increment the current level
    currentLevel++;

    // Save the current level state
    saveToFile("level_" + to_string(currentLevel - 1) + ".txt");
    saveEnemiesToFile();
    saveChestsToFile();

    // Check if the next level file exists, and if so, load it
    if (filesystem::exists("level_" + to_string(currentLevel) + ".txt")) {
        loadLevelIfExists();
    } else {
        // Generate a new level with a random map
        generateRandomMap();
        generateRandomEnemies();
        generateRandomChests();

        // Load the enemies and chests for the new level
        loadEnemiesFromFile();
        loadChestsFromFile();
    }
}

void Level::generateRandomMap() {
    // Get a list of available map filenames from a directory
    vector<string> mapFiles;
    const string mapDir = "maps/"; // Directory containing map files

    for (const auto &entry: filesystem::directory_iterator(mapDir)) {
        if (entry.is_regular_file()) {
            mapFiles.push_back(entry.path().string());
        }
    }

    // Randomly select a map file
    if (!mapFiles.empty()) {
        string randomMapFile = mapFiles[rand() % mapFiles.size()];
        loadFromFile(randomMapFile);
    }
}

void Level::generateRandomEnemies() {
    const int maxEnemies = 5; // Maximum number of enemies per level
    const int maxEnemyLevel = currentLevel + 1; // Maximum enemy level based on current level

    // Clear the existing enemies
    enemies.clear();

    // Generate a random number of enemies
    int numEnemies = rand() % (maxEnemies + 1);

    for (int i = 0; i < numEnemies; ++i) {
        // Generate random enemy parameters (level, health, damage)
        int enemyLevel = rand() % maxEnemyLevel + 1;
        int enemyHealth = enemyLevel * 10;
        int enemyDamage = enemyLevel * 5;

        // Generate a random enemy type
        Enemy::EnemyType enemyType;
        if (currentLevel >= 10) {
            enemyType = static_cast<Enemy::EnemyType>(rand() % 2); // Goblin or Orc
        } else {
            enemyType = Enemy::EnemyType::Goblin; // Only generate Goblins before level 10
        }

        // Create a new enemy object based on the random parameters and type
        Enemy *enemy = nullptr;
        if (enemyType == Enemy::EnemyType::Goblin) {
            enemy = new Goblin(enemyHealth, enemyDamage, enemyLevel);
        } else if (enemyType == Enemy::EnemyType::Orc) {
            enemy = new Orc(enemyHealth, enemyDamage, enemyLevel);
        }

        // Generate random positions until a valid position is found
        int enemyX, enemyY;
        bool validPosition = false;
        while (!validPosition) {
            enemyX = rand() % (WIDTH - 3) + 1;
            enemyY = rand() % (HEIGHT - 3) + 1;

            // Check if there is a wall within the designated area
            bool hasWall = false;
            for (int x = enemyX; x <= enemyX + 3; ++x) {
                for (int y = enemyY; y <= enemyY + 3; ++y) {
                    if (isWall(x, y)) {
                        hasWall = true;
                        break;
                    }
                }
                if (hasWall) {
                    break;
                }
            }
            validPosition = !hasWall;
        }
        enemy->setPosition(enemyX, enemyY);
        addEnemy(enemy);
    }
}

void Level::generateRandomChests() {
    const int minChests = 5;  // Minimum number of chests per level
    const int maxChests = 20; // Maximum number of chests per level

    // Generate a random number of chests within the specified range
    int numChests = rand() % (maxChests - minChests + 1) + minChests;

    for (int i = 0; i < numChests; ++i) {
        // Generate random chest position until a valid position is found
        int chestX, chestY;
        bool validPosition = false;
        while (!validPosition) {
            chestX = rand() % (WIDTH - 2) + 1; // Generate x position within the level boundaries
            chestY = rand() % (HEIGHT - 2) + 1; // Generate y position within the level boundaries

            // Check if there is a wall, enemy, or another chest at the designated position
            if (!isWall(chestX, chestY) && !isChest(chestX, chestY)) {
                validPosition = true;
            }
        }
        Chest chest(chestX, chestY);
        chests.push_back(chest);
    }
}

void Level::generateMerchant() {
    // Create the available items
    Item *item1 = new Sword(1, 100);
    Item *item2 = new Armor(1, 200);
    Item *item3 = new Bow(1, 100);
    Item *item4 = new Shield(1, 100, 10);

    // Add the items to the merchant's available items
    merchant.addAvailableItem(item1);
    merchant.addAvailableItem(item2);
    merchant.addAvailableItem(item3);
    merchant.addAvailableItem(item4);

    int merchantX, merchantY;
    bool validPosition = false;

    while (!validPosition) {
        // Generate a random position for the merchant within the level boundaries
        merchantX = rand() % WIDTH;
        merchantY = rand() % HEIGHT;

        // Check if the position is valid (not a wall, chest, or enemy)
        validPosition =
                !isWall(merchantX, merchantY) && !isChest(merchantX, merchantY) && !isEnemy(merchantX, merchantY);
    }
    // Set the position of the merchant
    merchant.setPosition(merchantX, merchantY);
}

bool Level::isWall(int x, int y) const {
    return levelData[x][y] == '#';
}

bool Level::isCollision(int x, int y) const {
    return isWall(x, y);
}

bool Level::isExclamation(int x, int y) const {
    return levelData[x][y] == '!';
}

bool Level::isQuestionMark(int x, int y) const {
    return levelData[x][y] == '?';
}

int Level::getCurrentLevel() const {
    return currentLevel;
}

void Level::setCurrentLevel(int level) {
    currentLevel = level;
}

void Level::addEnemy(Enemy *enemy) {
    enemies.push_back(enemy);
}

list<Enemy *> Level::getEnemies() const {
    return enemies;
}

bool Level::isEnemy(int x, int y) const {
    for (const auto &enemy: enemies) {
        int enemyX = enemy->getX();
        int enemyY = enemy->getY();

        if (enemyX >= x && enemyX <= x + 2 && enemyY >= y && enemyY <= y + 2) {
            return true;
        }
    }
    return false;
}

list<Chest> &Level::getChests() const {
    return const_cast<list<Chest> &>(chests);
}

bool Level::isChest(int x, int y) const {
    for (const auto &chest: chests) {
        if (chest.getX() == x && chest.getY() == y && !chest.isOpened()) {
            return true;
        }
    }
    return false;
}

bool Level::isChestOpened(int x, int y) const {
    for (const auto &chest: chests) {
        if (chest.getX() == x && chest.getY() == y && chest.isOpened()) {
            return true;
        }
    }
    return false;
}

void Level::openChest(int x, int y) {
    for (auto &chest: chests) {
        if (chest.getX() == x && chest.getY() == y && !chest.isOpened()) {
            chest.setOpen(true);
            levelData[x][y] = ' ';
            break;
        }
    }
}

bool Level::hasMerchant() const {
    return currentLevel == 1 || (currentLevel > 1 && rand() % 100 < 80);
}

const Merchant &Level::getMerchant() const {
    return merchant;
}

void Level::loadFromFile(const string &filename) {
    ifstream file(filename);
    if (file.is_open()) {
        for (int y = 0; y < HEIGHT; ++y) {
            string line;
            if (getline(file, line)) {
                for (int x = 0; x < WIDTH; ++x) {
                    if (x < line.length()) {
                        levelData[x][y] = line[x];
                    } else {
                        levelData[x][y] = ' ';
                    }
                }
            } else {
                // If failed to read a line, fill the remaining part of the level with spaces
                for (int x = 0; x < WIDTH; ++x) {
                    levelData[x][y] = ' ';
                }
            }
        }
        file.close();
    } else {
        // If failed to open the file, generate a default level
        generateLevel();
    }
}

void Level::saveToFile(const string &filename) const {
    string path = "Saves/Levels/" + filename;
    ofstream file(path);
    if (file.is_open()) {
        for (int y = 0; y < HEIGHT; ++y) {
            for (int x = 0; x < WIDTH; ++x) {
                file << levelData[x][y];
            }
            file << "\n";
        }
        file.close();
    } else {
        cout << "Failed to save level to file: " << filename << endl;
    }
}

void Level::loadSaveFromFile(const string &filename) {
    string path = "Saves/Levels/" + filename;
    ifstream file(path);
    if (file.is_open()) {
        for (int y = 0; y < HEIGHT; ++y) {
            string line;
            if (getline(file, line)) {
                for (int x = 0; x < WIDTH; ++x) {
                    if (x < line.length()) {
                        levelData[x][y] = line[x];
                    } else {
                        levelData[x][y] = ' ';
                    }
                }
            } else {
                // If failed to read a line, fill the remaining part of the level with spaces
                for (int x = 0; x < WIDTH; ++x) {
                    levelData[x][y] = ' ';
                }
            }
        }
        file.close();
    } else {
        // If failed to open the file, generate a default level
        generateLevel();
    }
}

void Level::saveEnemiesToFile() const {
    string filename = "Saves/Enemies/enemies_level_" + to_string(currentLevel) + ".txt";
    ofstream file(filename);
    if (file.is_open()) {
        for (const auto &enemy: enemies) {
            file << enemy->getX() << " " << enemy->getY() << " "
                 << enemy->getHealth() << " " << enemy->getDamage() << " "
                 << enemy->getLevel() << " " << static_cast<int>(enemy->getType()) << endl;
        }
        file.close();
    } else {
        cout << "Failed to save enemies to file: " << filename << endl;
    }
}

void Level::loadEnemiesFromFile() {
    string filename = "Saves/Enemies/enemies_level_" + to_string(currentLevel) + ".txt";
    ifstream file(filename);
    if (file.is_open()) {
        enemies.clear();
        int enemyX, enemyY, enemyHealth, enemyDamage, enemyLevel, enemyTypeInt;
        while (file >> enemyX >> enemyY >> enemyHealth >> enemyDamage >> enemyLevel >> enemyTypeInt) {
            Enemy::EnemyType enemyType = static_cast<Enemy::EnemyType>(enemyTypeInt);

            Enemy *enemy = nullptr;
            if (enemyType == Enemy::EnemyType::Goblin) {
                enemy = new Goblin(enemyHealth, enemyDamage, enemyLevel);
            } else if (enemyType == Enemy::EnemyType::Orc) {
                enemy = new Orc(enemyHealth, enemyDamage, enemyLevel);
            }
            enemy->setPosition(enemyX, enemyY);
            enemies.push_back(enemy);
        }
        file.close();
    } else {
        cout << "Failed to load enemies from file: " << filename << endl;
    }
}

void Level::saveChestsToFile() const {
    string filename = "Saves/Chests/chests_level_" + to_string(currentLevel) + ".txt";
    ofstream file(filename);
    if (file.is_open()) {
        for (const auto &chest: chests) {
            file << chest.getX() << " " << chest.getY() << endl;
        }
        file.close();
    } else {
        cout << "Failed to save chests to file: " << filename << endl;
    }
}

void Level::loadChestsFromFile() {
    string filename = "Saves/Chests/chests_level_" + to_string(currentLevel) + ".txt";
    ifstream file(filename);
    if (file.is_open()) {
        chests.clear();
        int chestX, chestY;
        while (file >> chestX >> chestY) {
            Chest chest(chestX, chestY);
            chests.push_back(chest);
        }
        file.close();
    } else {
        cout << "Failed to load chests from file: " << filename << endl;
    }
}

void Level::loadLevelIfExists() {
    // Load the level file
    loadFromFile("level_" + to_string(currentLevel) + ".txt");

    // Load the enemies and chests for the level
    loadEnemiesFromFile();
    loadChestsFromFile();
}

void Level::addProjectile(const Projectile &projectile) {
    projectiles.push_back(projectile);
}

list<Projectile> &Level::getProjectiles() {
    return projectiles;
}

void Level::clearProjectiles() {
    projectiles.clear();
}