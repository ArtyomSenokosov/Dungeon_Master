#ifndef ITEM_H
#define ITEM_H

#include <iostream>
#include <string>

using namespace std;

class Item {
public:

    enum class ItemType {
        Armor,
        Bow,
        Shield,
        Sword
    };

    Item(int level, int price, int slot, ItemType type);

    virtual ItemType getType() const;

    virtual string getName() const = 0;

    virtual int getDamage() const = 0;

    virtual int getDefense() const = 0;

    virtual void upgrade() = 0;

    int getLevel() const;

    int getPrice() const;

    int getSlot() const;

    void increaseLevel();

    int getUpgradePrice() const;

protected:
    ItemType type;
    int price;
    int slot;
    int level;
    int maxLevel = 99;
};

#endif // ITEM_H