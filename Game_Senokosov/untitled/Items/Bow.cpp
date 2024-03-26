#include "Bow.h"

Bow::Bow(int level, int price)
        : Item(level, price, 1, Item::ItemType::Bow) {}

Item::ItemType Bow::getType() const {
    return type;
}

string Bow::getName() const {
    return "Bow";
}

int Bow::getDamage() const {
    return baseDamage + level * 2;
}

int Bow::getDefense() const {
    return 0;
}

void Bow::upgrade() {
    increaseLevel();
}