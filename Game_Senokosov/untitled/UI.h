#ifndef UI_H
#define UI_H

#include <ncurses.h>
#include <unistd.h>
#include <fstream>
#include <sstream>
#include <chrono>
#include <thread>
#include <string>
#include "Level.h"
#include "Player.h"

class UI {
public:

    UI();

    ~UI();

    int runMenuLoop();

    void displayGameInterface(Level& level, Player& player);

    int displayMerchantMenu(const Merchant& merchant);

private:
    void initNcurses();

    void cleanupNcurses();

    void displayLevel(const Level& level, int start_x, int start_y, int width, int height);

    void displayPlayer(const Player& player, int startX, int startY);

    void displayEnemies(const Level& level, int startX, int startY);

    void displayMerchant(const Level& level, int start_x, int start_y);

    const char (*displayItem(const Item* item))[1];

    WINDOW* menuWin;
    WINDOW* gameWin;
    int menuItemsCount;
    int currentMenuItem;
};

#endif  // UI_H