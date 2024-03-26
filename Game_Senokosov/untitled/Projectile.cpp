#include "Projectile.h"

Projectile::Projectile(int startX, int startY, int deltaX, int deltaY, int speed)
        : x(startX), y(startY), deltaX(deltaX), deltaY(deltaY), speed(speed), active(true) {}

int Projectile::getX() const {
    return x;
}

int Projectile::getY() const {
    return y;
}

void Projectile::update() {
    x += deltaX;
    y += deltaY;
}

bool Projectile::isActive() const {
    return active;
}

void Projectile::setActive(bool active) {
    this->active = active;
}