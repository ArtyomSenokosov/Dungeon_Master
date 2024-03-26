#include "UI.h"

UI::UI() {
    initNcurses();
}

UI::~UI() {
    cleanupNcurses();
}

void UI::initNcurses() {
    initscr();
    curs_set(0);
    start_color();

    init_pair(1, COLOR_CYAN, COLOR_BLACK);
    init_pair(2, COLOR_WHITE, COLOR_CYAN);
    attron(COLOR_PAIR(1));

    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);

    if (max_y < 45 || max_x < 120) {
        endwin();
        printf("Minimum screen size is 120x45\n");
        exit(1);
    }

    const char *message[] = {
            "________                                                       _____                     __                   ",
            "\\______ \\   __ __   ____     ____    ____    ____    ____     /     \\  _____     _______/  |_   ____  _______ ",
            " |    |  \\ |  |  \\ /    \\   / ___\\ _/ __ \\  /  _ \\  /    \\   /  \\ /  \\ \\__  \\   /  ___/\\   __\\_/ __ \\ \\_  __ \\",
            " |    `   \\|  |  /|   |  \\ / /_/  >\\  ___/ (  <_> )|   |  \\ /    Y    \\ / __ \\_ \\___ \\  |  |  \\  ___/  |  | \\/",
            "/_______  /|____/ |___|  / \\___  /  \\___  > \\____/ |___|  / \\____|__  /(____  //____  > |__|   \\___  > |__|   ",
            "        \\/             \\/ /_____/       \\/              \\/          \\/      \\/      \\/             \\/         "
    };

    int width = 115;
    int height = 6;
    int start_y = (max_y - height) / 2;
    int start_x = (max_x - width) / 2;
    for (int i = 0; i < height; ++i) {
        mvprintw(start_y + i, start_x, "%s", message[i]);
    }

    refresh();
    sleep(3);

    clear();

    int menu_width = 30;
    int menu_height = 5;
    int menu_start_y = (max_y - menu_height) / 2;
    int menu_start_x = (max_x - menu_width) / 2;

    if (menu_height > max_y || menu_width > max_x) {
        endwin();
        printf("Minimum menu window size is 30x5\n");
        exit(1);
    }

    menuWin = newwin(menu_height, menu_width, menu_start_y, menu_start_x);
    box(menuWin, 0, 0);

    const char *menu_items[] = {
            "New Game",
            "Continua Game",
            "Info",
            "Exit"
    };
    menuItemsCount = sizeof(menu_items) / sizeof(menu_items[0]);

    currentMenuItem = 0;

    for (int i = 0; i < menuItemsCount; ++i) {
        if (i == currentMenuItem) {
            wattron(menuWin, COLOR_PAIR(2));
        }
        mvwprintw(menuWin, i + 1, 1, "%s", menu_items[i]);
        wattroff(menuWin, COLOR_PAIR(2));
    }

    wrefresh(menuWin);
    keypad(menuWin, true);
}

void UI::cleanupNcurses() {
    delwin(menuWin);
    if (gameWin != nullptr) {
        delwin(gameWin);
    }
    attroff(COLOR_PAIR(1));
    endwin();
}

int UI::runMenuLoop() {
    const char *menu_items[] = {
            "New Game",
            "Continua Game",
            "Info",
            "Exit"
    };
    int menu_items_count = sizeof(menu_items) / sizeof(menu_items[0]);

    int ch;
    while ((ch = wgetch(menuWin)) != '\n') {
        switch (ch) {
            case KEY_UP:
                if (currentMenuItem > 0) {
                    currentMenuItem--;
                }
                break;
            case KEY_DOWN:
                if (currentMenuItem < menu_items_count - 1) {
                    currentMenuItem++;
                }
                break;
        }

        for (int i = 0; i < menu_items_count; ++i) {
            if (i == currentMenuItem) {
                wattron(menuWin, COLOR_PAIR(2));
            }
            mvwprintw(menuWin, i + 1, 1, "%s", menu_items[i]);
            wattroff(menuWin, COLOR_PAIR(2));
        }

        wrefresh(menuWin);
    }

    return currentMenuItem;
}

void UI::displayGameInterface(Level &level, Player &player) {
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);

    int game_width = max_x - 32;
    int game_height = max_y;
    int game_start_y = 0;
    int game_start_x = 0;

    gameWin = newwin(game_height, game_width, game_start_y, game_start_x);
    box(gameWin, 0, 0);

    refresh();

    // Calculate the center position of the level
    int level_center_x = level.WIDTH / 2;
    int level_center_y = level.HEIGHT / 2;

    // Calculate the top-left position of the visible portion of the level
    int start_x = level_center_x - game_width / 2;
    int start_y = level_center_y - game_height / 2;

    // Display the level
    displayLevel(level, start_x, start_y, game_width, game_height);

    // Calculate the player's position relative to the visible portion of the level
    int player_x = player.getX() - start_x;
    int player_y = player.getY() - start_y;

    // Display the player
    displayPlayer(player, player_x, player_y);

    // Display the enemies
    displayEnemies(level, start_x, start_y);

    displayMerchant(level, start_x, start_y);

    // Display the projectiles
    for (const Projectile &projectile: level.getProjectiles()) {
        if (projectile.isActive()) {
            mvprintw(projectile.getY() - start_y + 2, projectile.getX() - start_x,
                     "*");  // Display the projectile symbol
        }
    }

    wrefresh(gameWin);

    // Create a separate window for the interface
    int interface_width = max_x - game_width - 1;
    int interface_height = max_y;
    int interface_start_y = 0;
    int interface_start_x = game_width + 1;

    WINDOW *interfaceWin = newwin(interface_height, interface_width, interface_start_y, interface_start_x);
    box(interfaceWin, 0, 0);

    // Display the game information in the interface window
    mvwprintw(interfaceWin, 1, 1, "Current Level: %d", level.getCurrentLevel());
    mvwprintw(interfaceWin, 2, 1, "Player Level: %d", player.getLevel());
    mvwprintw(interfaceWin, 3, 1, "HP: %d/%d", player.getCurrentHealth(), player.getMaxHealth());
    mvwprintw(interfaceWin, 4, 1, "Lives: %d/%d", player.getCurrentLives(), player.getMaxLives());
    mvwprintw(interfaceWin, 5, 1, "Money: %d", player.getMoney());

// Display equipped items
    mvwprintw(interfaceWin, 7, 1, "Equipped Items:");
    int itemY = 8;
    for (const auto &item: player.getEquippedItems()) {
        mvwprintw(interfaceWin, itemY, 1, "%s (Level %d) - Price: %d", item->getName().c_str(), item->getLevel(),
                  item->getPrice());
        itemY++;
    }

// Display item information
    mvwprintw(interfaceWin, 12, 1, "Item Information:");
    int itemInfoY = 13;
    for (const auto &item: player.getEquippedItems()) {
        mvwprintw(interfaceWin, itemInfoY, 1, "Name: %s, Level: %d, Price: %d", item->getName().c_str(),
                  item->getLevel(), item->getPrice());
        itemInfoY++;
    }

// Display inventory items
    mvwprintw(interfaceWin, 19, 1, "Inventory:");
    int inventoryY = 20;
    for (const auto &item: player.getInventory()) {
        mvwprintw(interfaceWin, inventoryY, 1, "%s (Level %d) - Price: %d", item->getName().c_str(), item->getLevel(),
                  item->getPrice());
        inventoryY++;
    }

// Display control instructions
    mvwprintw(interfaceWin, interface_height - 13, 1, "Controls:");
    mvwprintw(interfaceWin, interface_height - 12, 1, "Arrow Keys: Move");
    mvwprintw(interfaceWin, interface_height - 11, 1, "1: Equip Melee Weapon");
    mvwprintw(interfaceWin, interface_height - 10, 1, "2: Equip Ranged Weapon");
    mvwprintw(interfaceWin, interface_height - 9, 1, "3: Equip Armor");
    mvwprintw(interfaceWin, interface_height - 8, 1, "4: Equip Shield");
    mvwprintw(interfaceWin, interface_height - 7, 1, "o: Open Chest");
    mvwprintw(interfaceWin, interface_height - 6, 1, "i: Interact with Merchant");
    mvwprintw(interfaceWin, interface_height - 5, 1, "g: Perform Melee Attack");
    mvwprintw(interfaceWin, interface_height - 4, 1, "f: Perform Ranged Attack");
    mvwprintw(interfaceWin, interface_height - 3, 1, "q: Quit");


    wrefresh(interfaceWin);
}

void UI::displayLevel(const Level &level, int start_x, int start_y, int width, int height) {
    int level_width = Level::WIDTH;
    int level_height = Level::HEIGHT;
    int end_x = start_x + width;
    int end_y = start_y + height;

    // Calculate the visible portion of the level
    int visible_start_x = max(0, start_x);
    int visible_start_y = max(0, start_y);
    int visible_end_x = min(level_width, end_x);
    int visible_end_y = min(level_height, end_y);

    for (int y = visible_start_y; y < visible_end_y; y++) {
        for (int x = visible_start_x; x < visible_end_x; x++) {
            char tile = ' ';

            if (level.isWall(x, y)) {
                tile = '#';
            } else if (level.isChest(x, y) && !level.isChestOpened(x, y)) {
                tile = '@'; // Symbol representing a closed chest
            } else if (level.isExclamation(x, y)) {
                tile = '!'; // Symbol '!' representing an event
            } else if (level.isQuestionMark(x, y)) {
                tile = '?'; // Symbol '?' representing a mystery
            }

            mvwaddch(gameWin, y - start_y + 1, x - start_x + 1, tile);
        }
    }
    wrefresh(gameWin);
}

void UI::displayPlayer(const Player &player, int startX, int startY) {
    // Define the player's designs for each direction and frame of the animation
    const char walkingDesignUp[3][3] = {
            {' ', '0', ' '},
            {'/', '|', '\\'},
            {'<', ' ', '>'}
    };

    const char walkingDesignDown[3][3] = {
            {' ', '0', ' '},
            {'/', '|', '\\'},
            {'<', ' ', '>'}
    };

    const char walkingDesignLeft[3][3] = {
            {' ', '0', ' '},
            {'/', '|', '\\'},
            {'/', ' ', '<'}
    };

    const char walkingDesignRight[3][3] = {
            {' ', '0', ' '},
            {'/', '|', '\\'},
            {'>', ' ', '\\'}
    };

    const char standingDesign[3][3] = {
            {' ', '0', ' '},
            {'/', '|', '\\'},
            {'/', ' ', '\\'}
    };

    const char attackDesign[3][3] = {
            {' ', '0', ' '},
            {'<', '|', '\\'},
            {'/', ' ', '\\'}
    };

    // Get the current frame based on the player's movement direction and status
    const char (*currentDesign)[3] = standingDesign;  // Default to standing design

    if (player.isMoving()) {
        if (player.getDirection() == Player::Direction::Up) {
            currentDesign = walkingDesignUp;
        } else if (player.getDirection() == Player::Direction::Down) {
            currentDesign = walkingDesignDown;
        } else if (player.getDirection() == Player::Direction::Left) {
            currentDesign = walkingDesignLeft;
        } else if (player.getDirection() == Player::Direction::Right) {
            currentDesign = walkingDesignRight;
        }
    } else if (player.getDirection() == Player::Direction::Attack) {
        currentDesign = attackDesign;
    }

    // Display the player's design
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            mvwaddch(gameWin, startY + i + 1, startX + j + 1, currentDesign[i][j]);
        }
    }

    // Display equipped items
    int itemX = startX - 2;
    int itemY = startY + 1;
    for (const auto &item: player.getEquippedItems()) {
        const char (*itemSymbol)[1] = displayItem(item);
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 1; ++j) {
                mvwaddch(gameWin, itemY + i, itemX + item->getSlot() * 2, itemSymbol[i][j]);
            }
        }
    }
}

void UI::displayEnemies(const Level &level, int startX, int startY) {
    const char goblinDesign[3][3] = {
            {' ', 'G', ' '},
            {'(', '|', ')'},
            {'/', ' ', '\\'}
    };

    const char orcDesign[3][3] = {
            {' ', 'R', ' '},
            {'<', '|', '>'},
            {'/', ' ', '\\'}
    };

    const char goblinAttackDesign[3][3] = {
            {' ', 'G', ' '},
            {'*', '|', '*'},
            {'/', ' ', '\\'}
    };

    const char orcAttackDesign[3][3] = {
            {' ', 'R', ' '},
            {'*', '|', '*'},
            {'/', ' ', '\\'}
    };

    for (const auto &enemy: level.getEnemies()) {
        if (enemy->isAlive()) {
            int enemyX = enemy->getX();
            int enemyY = enemy->getY();

            const char (*currentDesign)[3] = nullptr;

            if (enemy->getType() == Enemy::EnemyType::Goblin) {
                if (enemy->isAttacking()) {
                    currentDesign = goblinAttackDesign;
                } else {
                    currentDesign = goblinDesign;
                }
            } else if (enemy->getType() == Enemy::EnemyType::Orc) {
                if (enemy->isAttacking()) {
                    currentDesign = orcAttackDesign;
                } else {
                    currentDesign = orcDesign;
                }
            }

            if (currentDesign != nullptr) {
                for (int i = 0; i < 3; ++i) {
                    for (int j = 0; j < 3; ++j) {
                        mvwaddch(gameWin, enemyY - startY + i + 1, enemyX - startX + j + 1, currentDesign[i][j]);
                    }
                }
            }
        }
    }
}

const char (*UI::displayItem(const Item *item))[1] {
    // Assign a symbol to each item type
    switch (item->getType()) {
        case Item::ItemType::Armor:
            static const char armorSymbol[3][1] = {{'M'},
                                                   {'H'},
                                                   {' '}};
            return armorSymbol;
        case Item::ItemType::Bow:
            static const char bowSymbol[3][1] = {{' '},
                                                 {'{'},
                                                 {' '}};
            return bowSymbol;
        case Item::ItemType::Shield:
            static const char shieldSymbol[3][1] = {{')'},
                                                    {')'},
                                                    {')'}};
            return shieldSymbol;
        case Item::ItemType::Sword:
            static const char swordSymbol[3][1] = {{'|'},
                                                   {'T'},
                                                   {' '}};
            return swordSymbol;
        default:
            static const char defaultSymbol[3][1] = {{' '},
                                                     {' '},
                                                     {' '}};
            return defaultSymbol;
    }
}

void UI::displayMerchant(const Level &level, int start_x, int start_y) {
    const char merchantDesign[3][3] = {
            {'\\', '$', ' '},
            {' ',  '|', '\\'},
            {'/',  ' ', '\\'}
    };

    const Merchant &merchant = level.getMerchant();

    // Check if the level has a merchant and the merchant is not at the default position (-1, -1)
    if (level.hasMerchant() && merchant.getX() != -1 && merchant.getY() != -1) {
        int merchantX = merchant.getX() - start_x;
        int merchantY = merchant.getY() - start_y;

        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                mvwaddch(gameWin, merchantY + i + 1, merchantX + j + 1, merchantDesign[i][j]);
            }
        }
    }
}

int UI::displayMerchantMenu(const Merchant &merchant) {
    // Clear the menu window
    wclear(menuWin);

    // Display the available items
    vector<Item *> availableItems = merchant.getAvailableItems();
    int numItems = availableItems.size();
    int selectedItem = 0;

    // Animation variables
    bool isAnimating = true;
    int animationFrame = 0;

    // Main menu loop
    while (isAnimating) {
        // Clear the menu window content
        wclear(menuWin);

        // Display the available items
        int menuItem = 1;
        for (int i = 0; i < numItems; ++i) {
            // Highlight the selected item
            if (i == selectedItem) {
                wattron(menuWin, A_REVERSE);
            }
            mvwprintw(menuWin, menuItem, 2, "%s (Level %d) - Price: %d", availableItems[i]->getName().c_str(),
                      availableItems[i]->getLevel(), availableItems[i]->getUpgradePrice());
            wattroff(menuWin, A_REVERSE);
            menuItem++;
        }

        // Refresh the menu window
        wrefresh(menuWin);

        // Wait for user input
        int ch = wgetch(menuWin);

        switch (ch) {
            case KEY_UP:
                selectedItem = (selectedItem - 1 + numItems) % numItems;
                break;
            case KEY_DOWN:
                selectedItem = (selectedItem + 1) % numItems;
                break;
            case KEY_ENTER:
            case '\n':
                isAnimating = false;
                break;
            default:
                break;
        }

        // Animation delay (adjust as needed)
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        animationFrame++;
    }

    // Clear the menu window
    wclear(menuWin);

    // Refresh the menu window
    wrefresh(menuWin);

    // Return the selected menu item
    return selectedItem;  // Return the index of the selected item
}