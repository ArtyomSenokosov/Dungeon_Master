#ifndef SWORD_H
#define SWORD_H

#include "Item.h"

class Sword : public Item {
public:

    Sword(int level, int price);

    ItemType getType() const override;

    string getName() const override;

    int getDamage() const override;

    int getDefense() const override;

    void upgrade() override;

private:
    int baseDamage = 10;
};

#endif // SWORD_H