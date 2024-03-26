#include "Player.h"

Player::Player(int x, int y)
        : moving(false), direction(Direction::None), x(x), y(y), initialX(x), initialY(y), maxHealth(100),
          currentHealth(100), money(1000), maxLives(3), currentLives(3) {
}

Player::AttackType Player::getAttackType() const {
    if (equippedItems.empty()) {
        return AttackType::Melee;
    } else {
        // Check the equipped item type for attack type
        Item *equippedItem = equippedItems.front();
        if (dynamic_cast<Sword *>(equippedItem) != nullptr) {
            return AttackType::Melee;
        } else if (dynamic_cast<Bow *>(equippedItem) != nullptr) {
            return AttackType::Ranged;
        } else {
            return AttackType::Melee;
        }
    }
}

Player::Direction Player::getDirection() const {
    return direction;
}

void Player::setDirection(Direction direction) {
    this->direction = direction;
}

int Player::getX() const {
    return x;
}

int Player::getY() const {
    return y;
}

void Player::setX(int _x) {
    Player::x = _x;
}

void Player::setY(int _y) {
    Player::y = _y;
}

void Player::reset() {
    x = 1;
    y = 1;
    level = 1;
    money = 1000;
    currentHealth = maxHealth;
    inventory.clear();
}

void Player::resetPosition() {
    x = initialX;
    y = initialY;
}

int Player::getLevel() const {
    return level;
}

int Player::getCurrentHealth() const {
    return currentHealth;
}

void Player::setCurrentHealth(int health) {
    currentHealth = health;
}

int Player::getMaxHealth() const {
    return maxHealth;
}

int Player::getMaxLives() const {
    return maxLives;
}

int Player::getCurrentLives() const {
    return currentLives;
}

int Player::getMoney() const {
    return money;
}

void Player::setMoney(int money) {
    this->money = money;
}

void Player::addMoney(int money) {
    this->money += money;
}

list<Item *> Player::getEquippedItems() const {
    return equippedItems;
}

list<Item *> Player::getInventory() const {
    return inventory;
}

void Player::addItemToInventory(Item *item) {
    inventory.push_back(item);
}

bool Player::hasItem(const Item *item) const {
    // Check if the item is in the player's inventory
    for (const auto &inventoryItem: inventory) {
        if (inventoryItem->getType() == item->getType()) {
            return true;
        }
    }

    // Check if the item is equipped by the player
    for (const auto &equippedItem: equippedItems) {
        if (equippedItem->getType() == item->getType()) {
            return true;
        }
    }
    return false;
}

void Player::upgradeItem(Item *item) {
    // Upgrade the item's properties based on the item type
    switch (item->getType()) {
        case Item::ItemType::Sword: {
            Sword *sword = dynamic_cast<Sword *>(item);
            if (sword) {
                sword->increaseLevel();
            }
            break;
        }
        case Item::ItemType::Armor: {
            Armor *armor = dynamic_cast<Armor *>(item);
            if (armor) {
                armor->increaseLevel();
            }
            break;
        }
        case Item::ItemType::Shield: {
            Shield *shield = dynamic_cast<Shield *>(item);
            if (shield) {
                shield->increaseLevel();
            }
            break;
        }
        case Item::ItemType::Bow: {
            Bow *bow = dynamic_cast<Bow *>(item);
            if (bow) {
                bow->increaseLevel();
            }
            break;
        }
    }
}

void Player::equipItem(Item *item) {
    if (inventory.empty()) {
        return;
    }

    auto it = equippedItems.begin();
    while (it != equippedItems.end()) {
        if (*it == item) {
            equippedItems.erase(it);
            inventory.push_back(item);
            break;
        }
        ++it;
    }
    if (it != inventory.end()) {
        switch (item->getSlot()) {
            case 1: {
                for (auto equippedItem: equippedItems) {
                    if (equippedItem->getSlot() == 1) {
                        unequipItem(equippedItem);
                        break;
                    }
                }
                break;
            }
            case 2: {
                for (auto equippedItem: equippedItems) {
                    if (equippedItem->getSlot() == 2) {
                        unequipItem(equippedItem);
                        break;
                    }
                }
                break;
            }
            case 3: {
                for (auto equippedItem: equippedItems) {
                    if (equippedItem->getSlot() == 3) {
                        unequipItem(equippedItem);
                        break;
                    }
                }
                break;
            }
        }
        equippedItems.push_back(item);

        auto it_inventory = inventory.begin();
        while (it_inventory != inventory.end()) {
            if (*it_inventory == item) {
                inventory.erase(it_inventory);
                break;
            }
            ++it_inventory;
        }
    }
}

bool Player::isItemEquipped(Item *item) const {
    for (auto equippedItem: equippedItems) {
        if (equippedItem == item) {
            return true;
        }
    }
    return false;
}

void Player::unequipItem(Item *item) {
    if (equippedItems.empty()) {
        return;
    }

    auto it = equippedItems.begin();
    while (it != equippedItems.end()) {
        if (*it == item) {
            equippedItems.erase(it);
            inventory.push_back(item);
            break;
        }
        ++it;
    }
}

bool Player::isMoving() const {
    return moving;
}

void Player::setMoving(bool val) {
    moving = val;
}

int Player::attack() const {
    if (equippedItems.empty())
        return 0;

    Item *equippedItem = equippedItems.front();
    if (equippedItem->getType() != Item::ItemType::Sword) {
        return 0;
    } else {
        return equippedItem->getDamage();
    }
}

int Player::rangedAttack() const {
    return equippedItems.empty() ? 0 : equippedItems.front()->getDamage();
}

void Player::takeDamage(int damage) {
    int totalDamage = damage;

    for (auto equippedItem: equippedItems) {
        if (equippedItem->getType() == Item::ItemType::Shield) {
            Shield *shield = static_cast<Shield *>(equippedItem);
            int blockCount = shield->getBlockCount();
            int remainingBlocks = shield->getBlockCount();
            int blockedDamage = 0;

            if (remainingBlocks > 0) {
                blockedDamage = std::min(totalDamage, shield->getDefense());
                totalDamage -= blockedDamage;
                shield->blockAttack();
                --remainingBlocks;

                cout << "Blocked " << blockedDamage << " damage with the shield." << endl;

                shield->setRemainingBlocks(remainingBlocks);
            }
        }
    }

    int blockedDamage = 0;

    for (auto equippedItem: equippedItems) {
        if (equippedItem->getType() == Item::ItemType::Armor) {
            Armor *armor = static_cast<Armor *>(equippedItem);
            int armorDefense = armor->getDefense();

            bool isBlocked = armor->blockAttack();

            if (isBlocked) {
                blockedDamage = std::min(totalDamage, armorDefense);
                totalDamage -= blockedDamage;
            }
        }
    }

    currentHealth -= totalDamage + blockedDamage;

    if (currentHealth <= 0) {
        --currentLives;
        currentHealth = maxHealth;
        if (currentLives <= 0) {
            cout << "Game over!" << endl;
            return;
        }
    }
}

bool Player::isGameOver() const {
    return currentLives <= 0;
}