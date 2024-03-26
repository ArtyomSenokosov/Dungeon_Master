#ifndef ARMOR_H
#define ARMOR_H

#include "Item.h"

class Armor : public Item {
public:

    Armor(int level, int price);

    ItemType getType() const override;

    string getName() const override;

    int getDamage() const override;

    int getDefense() const override;

    void upgrade() override;

    bool blockAttack();
};

#endif // ARMOR_H