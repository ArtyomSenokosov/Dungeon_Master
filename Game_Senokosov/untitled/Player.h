#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <ncurses.h>
#include <list>

#include "Items/Item.h"
#include "Items/Sword.h"
#include "Items/Bow.h"
#include "Items/Armor.h"
#include "Items/Shield.h"

using namespace std;

class Player {
public:

    Player(int x = 1, int y = 1);

    enum class AttackType {
        Melee,
        Ranged
    };

    enum Direction {
        None,
        Up,
        Down,
        Left,
        Right,
        Attack
    };

    AttackType getAttackType() const;

    Direction getDirection() const;

    void setDirection(Direction direction);

    int getX() const;

    int getY() const;

    void setX(int _x);

    void setY(int _y);

    void reset();

    void resetPosition();

    int getLevel() const;

    int getCurrentHealth() const;

    void setCurrentHealth(int health);

    int getMaxHealth() const;

    int getMaxLives() const;

    int getCurrentLives() const;

    int getMoney() const;

    void setMoney(int money);

    void addMoney(int money);

    list<Item *> getEquippedItems() const;

    list<Item *> getInventory() const;

    void addItemToInventory(Item *item);

    bool hasItem(const Item *item) const;

    void upgradeItem(Item *item);

    void equipItem(Item *item);

    bool isItemEquipped(Item *item) const;

    void unequipItem(Item *item);

    bool isMoving() const;

    void setMoving(bool val);

    int attack() const;

    int rangedAttack() const;

    void takeDamage(int damage);

    bool isGameOver() const;

private:
    int x, y;
    int initialX;
    int initialY;
    int level;
    int maxHealth;
    int currentHealth;
    int money;
    int maxLives;
    int currentLives;
    bool moving;

    list<Item *> equippedItems;
    list<Item *> inventory;
    Direction direction;
};

#endif // PLAYER_H
