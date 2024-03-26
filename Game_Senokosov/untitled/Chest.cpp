#include "Chest.h"

Chest::Chest(int x, int y) {
    this->x = x;
    this->y = y;
    this->opened = false;
}

int Chest::getX() const {
    return x;
}

int Chest::getY() const {
    return y;
}

int Chest::open() {
    opened = true;
    return rand() % 50 + 1;
}

bool Chest::isOpened() const {
    return opened;
}

void Chest::setOpen(bool open) {
    this->opened = open;
}