#include "Game.h"

Game::Game() {
    initscr();
    keypad(stdscr, true);
    nodelay(stdscr, true);
    curs_set(0);
    noecho();
    timeout(100);
}

void Game::run() {
    int menuChoice = ui.runMenuLoop();

    switch (menuChoice) {
        case 0:
            system("rm -rf Saves/Levels/*");
            system("rm -rf Saves/Chests/*");
            system("rm -rf Saves/Enemies/*");
            system("rm -rf Saves/Game/*");

            player.reset();
            level.generateLevel();
            level.generateRandomEnemies();
            level.generateRandomChests();
            level.generateMerchant();

            break;
        case 1:
            if (std::filesystem::exists("Saves/Game/save.txt")) {
                loadGame();
                level.loadEnemiesFromFile();
                level.loadChestsFromFile();
            } else {
                player.reset();
                level.generateLevel();
                level.generateRandomEnemies();
                level.generateRandomChests();
                level.generateMerchant();
            }
            break;
        case 2:
            system("open info.txt");
            return;
        case 3:
            return;
    }

    int ch;
    bool gameOver = false;
    bool bulletActive = false;

    bool playerRangedAttackInProgress = false;
    int playerRangedAttackCooldown = 0;
    const int playerRangedAttackCooldownDuration = 3;

    bool orcRangedAttackInProgress = false;
    int orcRangedAttackCooldown = 0;
    const int orcRangedAttackCooldownDuration = 3;

    while (!player.isGameOver() && !gameOver) {
        ch = getch();

        switch (ch) {
            case KEY_UP:
                if (!level.isCollision(player.getX(), player.getY() - 1)
                    && !level.isCollision(player.getX() + 1, player.getY() - 1)
                    && !level.isCollision(player.getX() + 2, player.getY() - 1)
                    && !isCollisionWithEnemies(player.getX(), player.getY() - 1)) {
                    player.setMoving(true);
                    player.setDirection(Player::Up);
                    player.setY(player.getY() - 1);
                }
                break;
            case KEY_DOWN:
                if (!level.isCollision(player.getX(), player.getY() + 3)
                    && !level.isCollision(player.getX() + 1, player.getY() + 3)
                    && !level.isCollision(player.getX() + 2, player.getY() + 3)
                    && !isCollisionWithEnemies(player.getX(), player.getY() + 3)) {
                    player.setMoving(true);
                    player.setDirection(Player::Down);
                    player.setY(player.getY() + 1);
                }
                break;
            case KEY_LEFT:
                if (!level.isCollision(player.getX() - 1, player.getY())
                    && !level.isCollision(player.getX() - 1, player.getY() + 1)
                    && !level.isCollision(player.getX() - 1, player.getY() + 2)
                    && !isCollisionWithEnemies(player.getX() - 1, player.getY())) {
                    player.setMoving(true);
                    player.setDirection(Player::Left);
                    player.setX(player.getX() - 1);

                    if (level.isExclamation(player.getX(), player.getY())) {
                        level.loadSaveFromFile("level_" + to_string(level.getCurrentLevel() - 1) + ".txt");
                        level.generateMerchant();
                        player.resetPosition();
                    }
                }
                break;
            case KEY_RIGHT:
                if (!level.isCollision(player.getX() + 3, player.getY())
                    && !level.isCollision(player.getX() + 3, player.getY() + 1)
                    && !level.isCollision(player.getX() + 3, player.getY() + 2)
                    && !isCollisionWithEnemies(player.getX() + 3, player.getY())) {
                    player.setMoving(true);
                    player.setDirection(Player::Right);
                    player.setX(player.getX() + 1);
                    if (level.isQuestionMark(player.getX() + 3, player.getY())) {
                        level.generateNextLevel();
                        player.resetPosition();
                    }
                }
                break;
            case '1':
                for (auto item: player.getInventory()) {
                    if (item->getType() == Item::ItemType::Sword) {
                        if (player.isItemEquipped(item)) {
                            player.unequipItem(item);
                        } else {
                            player.equipItem(item);
                        }
                        break;
                    }
                }
                break;
            case '2':
                for (auto item: player.getInventory()) {
                    if (item->getType() == Item::ItemType::Bow) {
                        if (player.isItemEquipped(item)) {
                            player.unequipItem(item);
                        } else {
                            player.equipItem(item);
                        }
                        break;
                    }
                }
                break;
            case '3':
                for (auto item: player.getInventory()) {
                    if (item->getType() == Item::ItemType::Armor) {
                        if (player.isItemEquipped(item)) {
                            player.unequipItem(item);
                        } else {
                            player.equipItem(item);
                        }
                        break;
                    }
                }
                break;
            case '4':
                for (auto item: player.getInventory()) {
                    if (item->getType() == Item::ItemType::Shield) {
                        if (player.isItemEquipped(item)) {
                            player.unequipItem(item);
                        } else {
                            player.equipItem(item);
                        }
                        break;
                    }
                }
                break;
            case 'o':
            case 'O':
                handleInput(ch);
                break;
            case 'I':
            case 'i':
                handleMerchantInteraction();
                break;
            case 'G':
            case 'g':
                checkPlayerAttack();
                break;
            case 'F':
            case 'f':
                player.setDirection(Player::Left);
                if (player.getAttackType() == Player::AttackType::Ranged && !playerRangedAttackInProgress) {
                    int startX = player.getX();
                    int startY = player.getY();
                    int deltaX = 0;
                    int deltaY = 0;

                    switch (player.getDirection()) {
                        case Player::Up:
                            deltaY = -1;
                            break;
                        case Player::Down:
                            deltaY = 1;
                            break;
                        case Player::Left:
                            deltaX = -1;
                            break;
                        case Player::Right:
                            deltaX = 1;
                            break;
                        default:
                            break;
                    }

                    Projectile playerProjectile(startX, startY, deltaX, deltaY, 1);
                    level.addProjectile(playerProjectile);
                    playerRangedAttackInProgress = true;
                    playerRangedAttackCooldown = playerRangedAttackCooldownDuration;
                }
                break;
            case 'q':
            case 'Q':
                saveGame();
                gameOver = true;
                break;
            default:
                player.setMoving(false);
                player.setDirection(Player::None);
                break;
        }

        for (Enemy *enemy: level.getEnemies()) {
            if (enemy->getType() == Enemy::EnemyType::Goblin) {
                handleGoblinBehavior(static_cast<Goblin *>(enemy));
            } else if (enemy->getType() == Enemy::EnemyType::Orc) {
                handleOrcBehavior(static_cast<Orc *>(enemy), orcRangedAttackInProgress, orcRangedAttackCooldown);
            }
        }

        checkEnemyAttack();

        if (playerRangedAttackInProgress) {
            // Update the projectiles
            for (Projectile &projectile: level.getProjectiles()) {
                if (projectile.isActive()) {
                    projectile.update();

                    // Check for collision with enemies
                    for (Enemy *enemy: level.getEnemies()) {
                        int enemyX = enemy->getX();
                        int enemyY = enemy->getY();
                        if (projectile.getX() == enemyX && projectile.getY() == enemyY) {
                            // Handle collision between projectile and enemy
                            enemy->takeDamage(player.rangedAttack());
                            projectile.setActive(false);
                            break;
                        }
                    }

                    // Check for collision with walls
                    if (level.isWall(projectile.getX(), projectile.getY())) {
                        projectile.setActive(false);
                    }
                }
            }

            // Check if all projectiles are inactive
            bool allProjectilesInactive = true;
            for (const Projectile &projectile: level.getProjectiles()) {
                if (projectile.isActive()) {
                    allProjectilesInactive = false;
                    break;
                }
            }

            if (allProjectilesInactive) {
                playerRangedAttackInProgress = false;
                level.clearProjectiles();
            }

            // Decrement projectile cooldown
            if (playerRangedAttackCooldown > 0) {
                playerRangedAttackCooldown--;
            }

        }

        clear();

        ui.displayGameInterface(level, player);
        refresh();
    }

    if (player.isGameOver()) {
        clear();
        int max_y, max_x;
        getmaxyx(stdscr, max_y, max_x);
        mvprintw(max_y / 2, (max_x - 9) / 2, "GAME OVER");
        refresh();
        this_thread::sleep_for(chrono::seconds(2));
    }

    endwin();
}

void Game::handleInput(int ch) {
    if (ch == 'o' || ch == 'O') {
        int playerX = player.getX();
        int playerY = player.getY();

        for (auto &chest: level.getChests()) {
            int chestX = chest.getX();
            int chestY = chest.getY();
            if (isPlayerNearChest(chestX, chestY) && !chest.isOpened()) {
                level.openChest(chestX, chestY);
                int money = chest.open();
                player.addMoney(money);
                break;
            }
        }
    }
}

void Game::handleMerchantInteraction() {
    const Merchant &merchant = level.getMerchant();
    const int playerX = player.getX();
    const int playerY = player.getY();
    const int merchantX = merchant.getX();
    const int merchantY = merchant.getY();

    // Check if the player is within the interaction range of the merchant
    if (isWithinInteractionRange(playerX, playerY, merchantX, merchantY)) {
        // Adjust the merchant's available items based on the player's items and level
        adjustMerchantItems(merchant);

        // Display the merchant menu
        int selectedItem = ui.displayMerchantMenu(merchant);

        // Handle the selected item
        if (selectedItem >= 0 && selectedItem < merchant.getAvailableItems().size()) {
            Item *item = merchant.getAvailableItems()[selectedItem];
            if (player.hasItem(item)) {
                // Upgrade the player's item using the merchant's item
                upgradeItemFromMerchant(item, merchant, player);
            } else {
                // Buy the item from the merchant
                buyItemFromMerchant(item, merchant, player);
            }
        }
    }
}

bool Game::isWithinInteractionRange(int x1, int y1, int x2, int y2) {
    bool isWithinRangeX = (x2 >= x1 - 3) && (x2 <= x1 + 3);
    bool isWithinRangeY = (y2 >= y2 - 3) && (y2 <= y1 + 3);

    bool isRight = (x2 == x1 + 3) && (y2 == y1);
    bool isLeft = (x2 == x1 - 3) && (y2 == y1);
    bool isUp = (x2 == x1) && (y2 == y1 - 3);
    bool isDown = (x2 == x1) && (y2 == y1 + 3);
    bool isDiagonalUpRight = (x2 == x1 + 3) && (y2 == y1 - 3);
    bool isDiagonalUpLeft = (x2 == x1 - 3) && (y2 == y1 - 3);
    bool isDiagonalDownRight = (x2 == x1 + 3) && (y2 == y1 + 3);
    bool isDiagonalDownLeft = (x2 == x1 - 3) && (y2 == y1 + 3);

    return isWithinRangeX && isWithinRangeY ||
           isRight || isLeft || isUp || isDown ||
           isDiagonalUpRight || isDiagonalUpLeft ||
           isDiagonalDownRight || isDiagonalDownLeft;
}

void Game::upgradeItemFromMerchant(Item *item, const Merchant &merchant, Player &player) {
    // Check if the player has enough money to upgrade the item
    if (player.getMoney() >= item->getUpgradePrice()) {
        // Deduct the upgrade price from the player's money
        player.addMoney(-item->getUpgradePrice());
        // Upgrade the player's item
        player.upgradeItem(item);
        // Increase the merchant's item level
        merchant.upgradeItem(item);
    }
}

void Game::buyItemFromMerchant(Item *item, const Merchant &merchant, Player &player) {
    // Check if the player has enough money to buy the item
    if (player.getMoney() >= item->getPrice()) {
        // Deduct the item price from the player's money
        player.addMoney(-item->getPrice());
        // Add the item to the player's inventory
        player.addItemToInventory(item);
        // Remove the item from the merchant's available item
        merchant.upgradeItem(item);
    }
}

void Game::adjustMerchantItems(Merchant merchant) {
    // Get the player's equipped items
    const list<Item *> &equippedItems = player.getEquippedItems();

    // Get the highest level among the player's equipped items
    int highestLevel = 0;
    for (const auto &item: equippedItems) {
        if (item->getLevel() > highestLevel) {
            highestLevel = item->getLevel();
        }
    }

    // Increase the level of the merchant's available items to be one level higher than the player's highest level
    vector<Item *> availableItems = merchant.getAvailableItems();
    for (auto &item: availableItems) {
        if (item->getLevel() <= highestLevel) {
            item->increaseLevel();
        }
    }

    // Set the adjusted available items in the merchant
    merchant.setAvailableItems(availableItems);
}

void Game::handleGoblinBehavior(Goblin *goblin) {
    int playerX = player.getX();
    int playerY = player.getY();

    int goblinX = goblin->getX();
    int goblinY = goblin->getY();

    double distance = std::sqrt(std::pow(playerX - goblinX, 2) + std::pow(playerY - goblinY, 2));

    if (distance <= 20) {
        int deltaX = playerX - goblinX;
        int deltaY = playerY - goblinY;

        if (deltaY < 0) {
            if (!level.isCollision(goblinX, goblinY - 1)
                && !level.isCollision(goblinX + 1, goblinY - 1)
                && !level.isCollision(goblinX + 2, goblinY - 1)
                && !isCollisionWithPlayer(goblinX, goblinY - 1)
                && !isCollisionWithEnemies(goblinX, goblinY - 1)) {
                goblin->setPosition(goblinX, goblinY - 1);
            }
        } else if (deltaY > 0) {
            if (!level.isCollision(goblinX, goblinY + 3)
                && !level.isCollision(goblinX + 1, goblinY + 3)
                && !level.isCollision(goblinX + 2, goblinY + 3)
                && !isCollisionWithPlayer(goblinX, goblinY + 3)
                && !isCollisionWithEnemies(goblinX, goblinY + 3)) {
                goblin->setPosition(goblinX, goblinY + 1);
            }
        }

        if (deltaX < 0) {
            if (!level.isCollision(goblinX - 1, goblinY)
                && !level.isCollision(goblinX - 1, goblinY + 1)
                && !level.isCollision(goblinX - 1, goblinY + 2)
                && !isCollisionWithPlayer(goblinX - 1, goblinY)
                && !isCollisionWithEnemies(goblinX - 1, goblinY)) {
                goblin->setPosition(goblinX - 1, goblinY);
            }
        } else if (deltaY > 0) {
            if (!level.isCollision(goblinX + 3, goblinY)
                && !level.isCollision(goblinX + 3, goblinY + 1)
                && !level.isCollision(goblinX + 3, goblinY + 2)
                && !isCollisionWithPlayer(goblinX + 3, goblinY)
                && !isCollisionWithEnemies(goblinX + 3, goblinY)) {
                goblin->setPosition(goblinX + 1, goblinY);
            }
        }
    } else {
        vector<pair<int, int>> directions;

        if (!level.isCollision(goblinX, goblinY - 1)
            && !level.isCollision(goblinX + 1, goblinY - 1)
            && !level.isCollision(goblinX + 2, goblinY - 1)
            && !isCollisionWithPlayer(goblinX, goblinY - 1)
            && !isCollisionWithEnemies(goblinX, goblinY - 1)) {
            directions.emplace_back(0, -1);
        }
        if (!level.isCollision(goblinX, goblinY + 3)
            && !level.isCollision(goblinX + 1, goblinY + 3)
            && !level.isCollision(goblinX + 2, goblinY + 3)
            && !isCollisionWithPlayer(goblinX, goblinY + 3)
            && !isCollisionWithEnemies(goblinX, goblinY + 3)) {
            directions.emplace_back(0, 1);
        }
        if (!level.isCollision(goblinX - 1, goblinY)
            && !level.isCollision(goblinX - 1, goblinY + 1)
            && !level.isCollision(goblinX - 1, goblinY + 2)
            && !isCollisionWithPlayer(goblinX - 1, goblinY)
            && !isCollisionWithEnemies(goblinX - 1, goblinY)) {
            directions.emplace_back(-1, 0);
        }
        if (!level.isCollision(goblinX + 3, goblinY)
            && !level.isCollision(goblinX + 3, goblinY + 1)
            && !level.isCollision(goblinX + 3, goblinY + 2)
            && !isCollisionWithPlayer(goblinX + 3, goblinY)
            && !isCollisionWithEnemies(goblinX + 3, goblinY)) {
            directions.emplace_back(1, 0);
        }
        if (!directions.empty()) {
            int randomIndex = rand() % directions.size();
            int deltaX = directions[randomIndex].first;
            int deltaY = directions[randomIndex].second;
            goblin->setPosition(goblinX + deltaX, goblinY + deltaY);
        }
    }
}

void Game::handleOrcBehavior(Orc *orc, bool &orcRangedAttackInProgress, int &orcRangedAttackCooldown) {
    const int orcRangedAttackCooldownDuration = 3;
    int playerX = player.getX();
    int playerY = player.getY();
    int orcX = orc->getX();
    int orcY = orc->getY();

    int distanceX = abs(playerX - orcX);
    int distanceY = abs(playerY - orcY);

    if (distanceX <= 20 && distanceY <= 20) {
        if (!orcRangedAttackInProgress && orcRangedAttackCooldown == 0) {
            int deltaX = 0;
            int deltaY = 0;

            if (playerX < orcX) {
                deltaX = -1;
            } else if (playerX > orcX) {
                deltaX = 1;
            }

            if (playerY < orcY) {
                deltaY = -1;
            } else if (playerY > orcY) {
                deltaY = 1;
            }

            Projectile orcProjectile(orcX, orcY, deltaX, deltaY, orc->getDamage());
            level.addProjectile(orcProjectile);

            orcRangedAttackInProgress = true;
            orcRangedAttackCooldown = orcRangedAttackCooldownDuration;
        }
    } else {
        vector<pair<int, int>> directions;

        if (!level.isCollision(orcX, orcY - 1)
            && !isCollisionWithPlayer(orcX, orcY - 1)
            && !isCollisionWithEnemies(orcX, orcY - 1)) {
            directions.emplace_back(0, -1);
        }
        if (!level.isCollision(orcX, orcY + 3)
            && !isCollisionWithPlayer(orcX, orcY + 3)
            && !isCollisionWithEnemies(orcX, orcY + 3)) {
            directions.emplace_back(0, 1);
        }
        if (!level.isCollision(orcX - 1, orcY)
            && !isCollisionWithPlayer(orcX - 1, orcY)
            && !isCollisionWithEnemies(orcX - 1, orcY)) {
            directions.emplace_back(-1, 0);
        }
        if (!level.isCollision(orcX + 3, orcY)
            && !isCollisionWithPlayer(orcX + 3, orcY)
            && !isCollisionWithEnemies(orcX + 3, orcY)) {
            directions.emplace_back(1, 0);
        }
        if (!directions.empty()) {
            int randomIndex = rand() % directions.size();
            int deltaX = directions[randomIndex].first;
            int deltaY = directions[randomIndex].second;
            orc->setPosition(orcX + deltaX, orcY + deltaY);
        }
        if (orcRangedAttackInProgress) {
            orcRangedAttackCooldown--;
            if (orcRangedAttackCooldown == 0) {
                orcRangedAttackInProgress = false;
            }
        }
    }
}

void Game::checkPlayerAttack() {
    if (player.getAttackType() == Player::AttackType::Melee) {
        // Check if there is an enemy within attack range
        for (const auto &enemy: level.getEnemies()) {
            if (isEnemyWithinAttackRange(player, *enemy)) {
                int damage = player.attack();
                enemy->takeDamage(damage);
                if (!enemy->isAlive()) {
                    // Remove defeated enemy from the level
                    //  level.removeEnemy(enemy);
                }
            }
        }
    } else if (player.getAttackType() == Player::AttackType::Ranged) {
        int startX = player.getX();
        int startY = player.getY();
        int deltaX = 0;
        int deltaY = 0;

        // Determine the direction of the projectile based on player's movement
        switch (player.getDirection()) {
            case Player::Direction::Up:
                deltaY = -1;
                break;
            case Player::Direction::Down:
                deltaY = 1;
                break;
            case Player::Direction::Left:
                deltaX = -1;
                break;
            case Player::Direction::Right:
                deltaX = 1;
                break;
            default:
                return;
        }

        // Create a projectile
        Projectile projectile(startX, startY, deltaX, deltaY, 1);

        // Move the projectile until it hits a wall or enemy
        while (projectile.isActive()) {
            projectile.update();

            int projectileX = projectile.getX();
            int projectileY = projectile.getY();

            if (isWallOrOutOfRange(projectileX, projectileY)) {
                break;
            }

            if (isCollisionWithEnemies(projectileX, projectileY)) {
                // Handle the collision with an enemy
                for (const auto &enemy: level.getEnemies()) {
                    if (enemy->getX() == projectileX && enemy->getY() == projectileY) {
                        int damage = player.rangedAttack();
                        enemy->takeDamage(damage);
                        if (!enemy->isAlive()) {
                            // Remove defeated enemy from the level
                            //  level.removeEnemy(enemy);
                        }
                        break;
                    }
                }
                break;
            }
        }
    }
}

bool Game::isWallBetween(int x1, int y1, int x2, int y2) const {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int x = x1;
    int y = y1;
    int n = 1 + dx + dy;
    int x_inc = (x2 > x1) ? 1 : -1;
    int y_inc = (y2 > y1) ? 1 : -1;
    int error = dx - dy;
    dx *= 2;
    dy *= 2;

    for (; n > 0; --n) {
        if (level.isWall(x, y)) {
            return true;
        }

        if (error > 0) {
            x += x_inc;
            error -= dy;
        } else {
            y += y_inc;
            error += dx;
        }
    }

    return false;
}

void Game::checkEnemyAttack() {
    for (Enemy *enemy: level.getEnemies()) {
        if (enemy->isAlive() && isEnemyWithinAttackRange(player, *enemy)
            && !isWallBetween(enemy->getX(), enemy->getY(), player.getX(), player.getY())) {
            int enemyDamage = enemy->attack();

            if (enemyDamage > 0) {
                player.takeDamage(enemyDamage);
                cout << "  " << enemyDamage << "  " << endl;
            }

            if (player.isGameOver()) {
                cout << " " << endl;
            }
        }
    }
}

bool Game::isEnemyWithinAttackRange(const Player &player, const Enemy &enemy) {
    int playerX = player.getX() + 1;
    int playerY = player.getY();
    int enemyX = enemy.getX();
    int enemyY = enemy.getY();

    bool isWithinRangeX = (enemyX >= playerX - 4) && (enemyX <= playerX + 4);
    bool isWithinRangeY = (enemyY >= playerY - 4) && (enemyY <= playerY + 4);

    bool isRight = (enemyX == playerX + 4) && (enemyY == playerY);
    bool isLeft = (enemyX == playerX - 4) && (enemyY == playerY);
    bool isUp = (enemyX == playerX) && (enemyY == playerY - 4);
    bool isDown = (enemyX == playerX) && (enemyY == playerY + 4);
    bool isDiagonalUpRight = (enemyX == playerX + 4) && (enemyY == playerY - 4);
    bool isDiagonalUpLeft = (enemyX == playerX - 4) && (enemyY == playerY - 4);
    bool isDiagonalDownRight = (enemyX == playerX + 4) && (enemyY == playerY + 4);
    bool isDiagonalDownLeft = (enemyX == playerX - 4) && (enemyY == playerY + 4);

    return isWithinRangeX && isWithinRangeY ||
           isRight || isLeft || isUp || isDown ||
           isDiagonalUpRight || isDiagonalUpLeft ||
           isDiagonalDownRight || isDiagonalDownLeft;
}

bool Game::isWallOrOutOfRange(int x, int y) const {
    return level.isWall(x, y) || x < 0 || x >= Level::WIDTH || y < 0 || y >= Level::HEIGHT;
}

bool Game::isCollisionWithEnemies(int x, int y) {
    for (Enemy *enemy: level.getEnemies()) {
        if (enemy->getX() <= x && x < enemy->getX() + 3 &&
            enemy->getY() <= y && y < enemy->getY() + 3) {
            return true;
        }
    }
    return false;
}

bool Game::isCollisionWithPlayer(int x, int y) {
    if (player.getX() <= x && x < player.getX() + 3 &&
        player.getY() <= y && y < player.getY() + 3) {
        return true;
    }
    return false;
}

bool Game::isPlayerNearChest(int chestX, int chestY) {
    for (int y = player.getY(); y < player.getY() + 3; y++) {
        for (int x = player.getX(); x < player.getX() + 3; x++) {
            bool isRight = (chestX == x + 1) && (chestY == y);
            bool isLeft = (chestX == x - 1) && (chestY == y);
            bool isUp = (chestX == x) && (chestY == y - 1);
            bool isDown = (chestX == x) && (chestY == y + 1);
            bool isDiagonalUpRight = (chestX == x + 1) && (chestY == y - 1);
            bool isDiagonalUpLeft = (chestX == x - 1) && (chestY == y - 1);
            bool isDiagonalDownRight = (chestX == x + 1) && (chestY == y + 1);
            bool isDiagonalDownLeft = (chestX == x - 1) && (chestY == y + 1);

            if (isRight || isLeft || isUp || isDown ||
                isDiagonalUpRight || isDiagonalUpLeft ||
                isDiagonalDownRight || isDiagonalDownLeft) {
                return true;
            }
        }
    }
    return false;
}

void Game::saveGame() const {
    filesystem::create_directory("Saves/Game");

    string filename = "Saves/Game/save.txt";

    ofstream file(filename);
    if (file.is_open()) {
        file << player.getX() << " " << player.getY() << "\n";
        file << level.getCurrentLevel() << " " << player.getMoney() << " " << player.getCurrentHealth() << "\n";

        const list<Item *> &inventory = player.getInventory();
        file << inventory.size() << "\n";
        for (const Item *item: inventory) {
            file << static_cast<int>(item->getType()) << " " << item->getLevel() << "\n";
        }

        level.saveToFile("level_" + std::to_string(level.getCurrentLevel()) + ".txt");

        level.saveEnemiesToFile();
        level.saveChestsToFile();

        file.close();
    } else {
        cout << "Failed to save the game.\n";
    }
}

void Game::loadGame() {
    string filename = "Saves/Game/save.txt";

    if (!filesystem::exists(filename)) {
        cout << "Save file does not exist.\n";
        return;
    }

    ifstream file(filename);
    if (file.is_open()) {
        int playerX, playerY, savedLevel, money, currentHealth;
        file >> playerX >> playerY;
        file >> savedLevel >> money >> currentHealth;
        player.setX(playerX);
        player.setY(playerY);
        player.setMoney(money);
        player.setCurrentHealth(currentHealth);

        int inventorySize;
        file >> inventorySize;
        for (int i = 0; i < inventorySize; ++i) {
            int itemTypeInt, itemLevel;
            file >> itemTypeInt >> itemLevel;
            Item::ItemType itemType = static_cast<Item::ItemType>(itemTypeInt);

            Item *item = nullptr;
            switch (itemType) {
                case Item::ItemType::Armor:
                    item = new Armor(itemLevel, 0);
                    break;
                case Item::ItemType::Bow:
                    item = new Bow(itemLevel, 0);
                    break;
                case Item::ItemType::Shield:
                    item = new Shield(itemLevel, 0, 0);
                    break;
                case Item::ItemType::Sword:
                    item = new Sword(itemLevel, 0);
                    break;
            }

            if (item) {
                player.addItemToInventory(item);
            }
        }

        level.setCurrentLevel(savedLevel);
        level.loadSaveFromFile("level_" + to_string(savedLevel) + ".txt");

        level.loadEnemiesFromFile();
        level.loadChestsFromFile();

        file.close();
    } else {
        cout << "Failed to load the game.\n";
    }
}