#include "Goblin.h"

Goblin::Goblin(int health, int damage, int level)
        : Enemy(health, damage, level, Enemy::EnemyType::Goblin) {}

Enemy::EnemyType Goblin::getType() const {
    return EnemyType::Goblin;
}

int Goblin::attack() const {
    return damage;
}

bool Goblin::isAttacking() const {
    return attacking;
}

void Goblin::takeDamage(int damage) {
    Enemy::takeDamage(damage);
}

void Goblin::setAttack(bool attacking) {
    this->attacking = attacking;
}