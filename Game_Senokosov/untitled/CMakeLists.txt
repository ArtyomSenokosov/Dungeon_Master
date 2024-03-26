cmake_minimum_required(VERSION 3.25)
project(untitled)

set(CMAKE_CXX_STANDARD 17)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_executable(untitled main.cpp Game.cpp Game.h Player.cpp Player.h Level.cpp Level.h Enemies/Enemy.cpp Enemies/Enemy.h Merchant.cpp Merchant.h Items/Item.cpp Items/Item.h UI.cpp UI.h Items/Bow.cpp Items/Bow.h Items/Sword.cpp Items/Sword.h Items/Shield.cpp Items/Shield.h Items/Armor.cpp Items/Armor.h Enemies/Goblin.cpp Enemies/Goblin.h Enemies/Orc.cpp Enemies/Orc.h Chest.cpp Chest.h Projectile.cpp Projectile.h)

target_link_libraries(untitled ${CURSES_LIBRARIES})
