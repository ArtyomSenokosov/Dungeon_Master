#include "Item.h"

Item::Item(int level, int price, int slot, ItemType type)
        : price(price), slot(slot), type(type) {
    if (level > maxLevel) {
        level = maxLevel;
    }
    this->level = level;
}

Item::ItemType Item::getType() const {
    return type;
}

int Item::getLevel() const {
    return level;
}

int Item::getPrice() const {
    return price;
}

int Item::getSlot() const {
    return slot;
}

void Item::increaseLevel() {
    if (level > maxLevel) {
        level = maxLevel;
    }
    level++;
}

int Item::getUpgradePrice() const {
    return static_cast<int>(price * 1.5);
}