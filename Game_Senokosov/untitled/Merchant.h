#ifndef MERCHANT_H
#define MERCHANT_H

#include <algorithm>
#include <vector>

#include "Items/Item.h"
#include "Player.h"

class Merchant {
public:

    Merchant(int x, int y);

    int getX() const;

    int getY() const;

    void setPosition(int x, int y);

    vector<Item *> getAvailableItems() const;

    void setAvailableItems(const vector<Item *> &items);

    void addAvailableItem(Item *item);

    void upgradeItem(Item *item) const;

private:
    int x;
    int y;
    vector<Item *> availableItems;
};

#endif // MERCHANT_H