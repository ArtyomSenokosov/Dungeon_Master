#ifndef CHEST_H
#define CHEST_H

#include <cstdlib>

class Chest {
public:

    Chest(int x, int y);

    int getX() const;

    int getY() const;

    int open();

    bool isOpened() const;

    void setOpen(bool open);

private:
    int x;
    int y;
    bool opened;
};

#endif // CHEST_H