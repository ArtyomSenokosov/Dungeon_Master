#ifndef GOBLIN_H
#define GOBLIN_H

#include "Enemy.h"

class Goblin : public Enemy {
public:

    Goblin(int health, int damage, int level);

    EnemyType getType() const override;

    int attack() const override;

    bool isAttacking() const override;

    void takeDamage(int damage) override;

    void setAttack(bool attacking) override;
};

#endif // GOBLIN_H