#ifndef ORC_H
#define ORC_H

#include "Enemy.h"

class Orc : public Enemy {
public:

    Orc(int health, int damage, int level);

    EnemyType getType() const override;

    int attack() const override;

    bool isAttacking() const override;

    void takeDamage(int damage) override;

    void setAttack(bool attacking) override;
};

#endif // ORC_H