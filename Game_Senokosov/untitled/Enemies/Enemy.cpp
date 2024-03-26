#include "Enemy.h"

Enemy::Enemy(int health, int damage, int level, EnemyType enemyType)
        : health(health), damage(damage), level(level), type(enemyType), attacking(false) {
    if (level > maxLevel) {
        level = maxLevel;
    }
    this->level = level;
}

Enemy::EnemyType Enemy::getType() const {
    return type;
}

void Enemy::takeDamage(int damage) {
    health -= damage;
    if (health <= 0) {
        alive = false;
    }
}

void Enemy::setPosition(int x, int y) {
    this->x = x;
    this->y = y;
}

int Enemy::getX() const {
    return x;
}

int Enemy::getY() const {
    return y;
}

int Enemy::getHealth() const {
    return health;
}

int Enemy::getDamage() const {
    return damage;
}

int Enemy::getLevel() const {
    return level;
}

bool Enemy::isAlive() const {
    return alive;
}