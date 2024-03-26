#ifndef BOW_H
#define BOW_H

#include "Item.h"

class Bow : public Item {
public:

    Bow(int level, int price);

    ItemType getType() const override;

    string getName() const override;

    int getDamage() const override;

    int getDefense() const override;

    void upgrade() override;

private:
    int baseDamage = 10;
};

#endif // BOW_H