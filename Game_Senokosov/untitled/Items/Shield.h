#ifndef SHIELD_H
#define SHIELD_H

#include "Item.h"

class Shield : public Item {
public:
    Shield(int level, int price, int blockCount);

    ItemType getType() const override;

    string getName() const override;

    int getDamage() const override;

    int getDefense() const override;

    void upgrade() override;

    int getBlockCount() const;

    void blockAttack();

    void setRemainingBlocks(int blocks);

private:
    int blockCount;
    int remainingBlocks;
};

#endif // SHIELD_H