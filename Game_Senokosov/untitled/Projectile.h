#ifndef PROJECTILE_H
#define PROJECTILE_H

class Projectile {
public:

    Projectile(int startX, int startY, int deltaX, int deltaY, int speed);

    int getX() const;

    int getY() const;

    void update();

    bool isActive() const;

    void setActive(bool active);

private:
    int x;
    int y;
    int deltaX;
    int deltaY;
    int speed;
    bool active;
};

#endif // PROJECTILE_H
