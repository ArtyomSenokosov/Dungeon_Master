#include "Sword.h"

Sword::Sword(int level, int price)
        : Item(level, price, 1, Item::ItemType::Sword) {
}

Item::ItemType Sword::getType() const {
    return type;
}

string Sword::getName() const {
    return "Sword";
}

int Sword::getDamage() const {
    return baseDamage + level * 5;
}

int Sword::getDefense() const {
    return 0;
}

void Sword::upgrade() {
    increaseLevel();
}