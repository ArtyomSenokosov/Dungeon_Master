#ifndef ENEMY_H
#define ENEMY_H

#include <ncurses.h>
#include <string>

using namespace std;

class Enemy {
public:

    enum class EnemyType {
        Goblin,
        Orc
    };

    Enemy(int health, int damage, int level, EnemyType enemyType);

    virtual EnemyType getType() const;

    virtual int attack() const = 0;

    virtual bool isAttacking() const = 0;

    virtual void setAttack(bool attacking) = 0;

    virtual void takeDamage(int damage);

    void setPosition(int x, int y);

    int getX() const;

    int getY() const;

    int getHealth() const;

    int getDamage() const;

    int getLevel() const;

    bool isAlive() const;

protected:
    EnemyType type;
    int x, y;
    int level;
    int maxLevel = 99;
    int health;
    int damage;
    bool alive;
    bool attacking;
};

#endif // ENEMY_H