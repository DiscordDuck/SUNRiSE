#ifndef DRIVERS_H_INCLUDED
#define DRIVERS_H_INCLUDED

inline const int RELEASE_TARGET = 1; // 0 for Dev: minimal pauses 
inline const std::string ENUM_TO_ITEM[20] = {"Logs", "Rocks", "Iron", "Steel", "Obsidian", "Pickaxe", "Hammer", "Axe", "Smoke Bomb", "Glowstick Spear", "Samurai Armour", "S-Coins"};
enum items { logs = 0, rocks = 1, iron = 2, steel = 3, obsidian = 4, pickaxe = 5, hammer = 6, axe = 7, smoke = 8, spear = 9, armour = 10, coins = 19 };

inline const int SHOP_PRICES[6][8] = { // {SHOP_NUMBER, ENUM, LOGS, ROCKS, IRON, STEEL, OBSIDIAN, COST}
    {1, items::pickaxe, 6, 2, 0, 0, 0, 0},          // pickaxe
    {2, items::hammer, 24, 18, 2, 0, 0, 0},         // hammer
    {3, items::axe, 0, 0, 15, 0, 5, 0},             // axe
    {4, items::smoke, 100, 0, 0, 12, 0, 200},       // smoke
    {5, items::spear, 0, 20, 2, 0, 10, 400},        // spear
    {6, items::armour, 0, 0, 15, 0, 25, 727},       // armour
};

void pause(int seconds = 1, int battle = 0);
void mspause(int ms);
void print(std::string str, int t = 0);
void type(std::string str);
int errorTrap(int lowerRange, int upperRange);
std::string space(int vertical = 0, int horizontal = 0);


#endif // DRIVERS_H_INCLUDED
