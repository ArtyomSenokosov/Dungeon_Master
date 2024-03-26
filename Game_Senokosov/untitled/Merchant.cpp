#include "Merchant.h"


Merchant::Merchant(int x, int y) : x(x), y(y) {}

int Merchant::getX() const {
    return x;
}

int Merchant::getY() const {
    return y;
}

void Merchant::setPosition(int x, int y) {
    this->x = x;
    this->y = y;
}

vector<Item *> Merchant::getAvailableItems() const {
    return availableItems;
}

void Merchant::setAvailableItems(const vector<Item *> &items) {
    availableItems = items;
}

void Merchant::addAvailableItem(Item *item) {
    availableItems.push_back(item);
}

void Merchant::upgradeItem(Item *item) const {
    // Increase the level of the item
    item->increaseLevel();

    // Update the item's properties based on its type
    switch (item->getType()) {
        case Item::ItemType::Sword:
            item->upgrade();
            break;
        case Item::ItemType::Armor:
            item->upgrade();
            break;
        case Item::ItemType::Shield:
            item->upgrade();
            break;
        case Item::ItemType::Bow:
            item->upgrade();
            break;
    }
}