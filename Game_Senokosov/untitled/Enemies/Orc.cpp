#include "Orc.h"

Orc::Orc(int health, int damage, int level)
        : Enemy(health, damage, level, Enemy::EnemyType::Orc) {}

Enemy::EnemyType Orc::getType() const {
    return EnemyType::Orc;
}

int Orc::attack() const {
    return damage;
}

bool Orc::isAttacking() const {
    return attacking;
}

void Orc::takeDamage(int damage) {
    Enemy::takeDamage(damage);
}

void Orc::setAttack(bool attacking) {
    this->attacking = attacking;
}