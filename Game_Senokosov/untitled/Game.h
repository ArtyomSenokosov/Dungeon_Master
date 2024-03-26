#ifndef GAME_H
#define GAME_H

#include <string>
#include <iostream>
#include <ncurses.h>
#include <thread>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <algorithm>

#include "Player.h"
#include "Level.h"
#include "Projectile.h"
#include "Items/Sword.h"
#include "Items/Item.h"
#include "Enemies/Enemy.h"
#include "Enemies/Goblin.h"
#include "Items/Bow.h"
#include "Enemies/Orc.h"
#include "UI.h"
#include "Items/Armor.h"
#include "Items/Shield.h"

using namespace std;

class Game {
public:
    Game();

    void run();

private:
    UI ui;
    Player player;
    Level level;

    void handleInput(int ch);

    void checkPlayerAttack();

    void checkEnemyAttack();

    bool isEnemyWithinAttackRange(const Player &player, const Enemy &enemy);

    void handleGoblinBehavior(Goblin *goblin);

    void handleOrcBehavior(Orc *orc, bool &orcRangedAttackInProgress, int &orcRangedAttackCooldown);

    bool isWithinInteractionRange(int x1, int y1, int x2, int y2);

    bool isWallOrOutOfRange(int x, int y) const;

    bool isWallBetween(int x1, int y1, int x2, int y2) const;

    bool isCollisionWithEnemies(int x, int y);

    bool isCollisionWithPlayer(int x, int y);

    bool isPlayerNearChest(int chestX, int chestY);

    void handleMerchantInteraction();

    void buyItemFromMerchant(Item *item, const Merchant &merchant, Player &player);

    void upgradeItemFromMerchant(Item *item, const Merchant &merchant, Player &player);

    void adjustMerchantItems(Merchant merchant);

    void saveGame() const;

    void loadGame();
};

#endif // GAME_H
