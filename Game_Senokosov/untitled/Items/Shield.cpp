#include "Shield.h"

Shield::Shield(int level, int price, int blockCount)
        : Item(level, price, 3, Item::ItemType::Shield), blockCount(blockCount), remainingBlocks(blockCount) {
}

Item::ItemType Shield::getType() const {
    return type;
}

string Shield::getName() const {
    return "Shield";
}

int Shield::getDamage() const {
    return 0;
}

int Shield::getDefense() const {
    return level;
}

void Shield::upgrade() {
    increaseLevel();
}

int Shield::getBlockCount() const {
    return blockCount;
}

void Shield::blockAttack() {
    if (remainingBlocks > 0) {
        remainingBlocks--;
    }
}

void Shield::setRemainingBlocks(int blocks) {
    remainingBlocks = blocks;
}