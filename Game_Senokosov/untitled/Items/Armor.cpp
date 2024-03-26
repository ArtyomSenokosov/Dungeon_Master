#include "Armor.h"

Armor::Armor(int level, int price)
        : Item(level, price, 2, Item::ItemType::Armor) {}

Item::ItemType Armor::getType() const {
    return type;
}

string Armor::getName() const {
    return "Armor";
}

int Armor::getDamage() const {
    return 0;
}

int Armor::getDefense() const {
    return level;
}

void Armor::upgrade() {
    increaseLevel();
}

bool Armor::blockAttack() {
    int blockChance = level;
    int randomNum = rand() % 100 + 1;
    if (randomNum <= blockChance) {
        return true;
    } else {
        return false;
    }
}