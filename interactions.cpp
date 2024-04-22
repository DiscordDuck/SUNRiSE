#include <iostream>
#include <vector>
#include <algorithm>
#include <time.h>
#include "drivers.h"
#include "ascii.h"
#include "interactions.h"
using namespace std;

double BASE_LOGS;
double BASE_ROCKS;
double BASE_IRON;
double BASE_STEEL;
double BASE_OBSIDIAN;
double COSTS[5] = { };

void init(double LOGS, double ROCKS, double IRON, double STEEL, double OBSIDIAN) {
    BASE_LOGS = LOGS;
    BASE_ROCKS = ROCKS;
    BASE_IRON = IRON;
    BASE_STEEL = STEEL;
    BASE_OBSIDIAN = OBSIDIAN;
    COSTS[0] = LOGS;
    COSTS[1] = ROCKS;
    COSTS[2] = IRON;
    COSTS[3] = STEEL;
    COSTS[4] = OBSIDIAN;
}

void *shop(bool tutorialState, int inv[]) {

    int shopChoice;

    do {
        system("cls");

        cout << space(5, 4);
        type("\b\bWalking through the forest, a merchant's shop appears.\n\n");
        generateMerchantsHouse();
        // 1: View Shop, 2: Sell Items, 3: Chat with Merchant
        cout << space(0, 4);
        type("Welcome, traveller. What are you here for? "); /// TODO: rename traveller to entered name
        pause(1);
        cout << "[   ]\b\b\b";
        cin >> shopChoice;
        /// TODO: Error Trap to only 1
        if (shopChoice == 1) {
            system("cls");

            int choice;

            cout << space(5, 6) << "\b\bMerchant's Shop" << endl << space(0, 2) << ".-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-.";
            if (tutorialState) generateTShop();
            else if (!inv[items::hammer]) generateTShop();
            else if (!inv[items::axe]) generateHShop();
            else generateFShop();
            // else generateFShop();

            if (tutorialState) {
                pause(4);
                cout << space(1, 5) << "Enter any key to continue. ";
                char c;
                cin >> c;
                shopChoice = 0;
                return 0;
            }
            vector<int> buyableItems;
            for (int i = 0; i < sizeof(SHOP_PRICES) / sizeof(SHOP_PRICES[0]); i++) {
                int item = SHOP_PRICES[i][0];
                bool hasAllResources = true;
                if (inv[SHOP_PRICES[i][1]]) {
                    continue;
                }
                if (SHOP_PRICES[i][7] > inv[items::coins]) hasAllResources = false;
                else {
                    for (int j = 2; j <= 6; j++) {
                        if (SHOP_PRICES[i][j] > inv[j - 2]) {
                            /// NOTE: inv[j - 2] is hardcoded with the j - 2 part to avoid repetition, please edit accordingly
                            hasAllResources = false;
                            continue;
                        }
                        if (hasAllResources && j == 6) {
                            buyableItems.push_back(item);
                            break;
                        }
                    }
                }
            }

            if (!buyableItems.size()) {
                cout << space(1, 5) << "\b\bNothing can be bought right now.";
                pause(1);
                cout << space(1, 5) << "Enter any key to continue. ";
                char c;
                cin >> c;
                shopChoice = 0;
                system("cls");
                return 0;
            }
            else {
                cout << space(1, 5) << "   Right now, you can buy: ";
                for (auto i = buyableItems.begin(); i != buyableItems.end(); ++i) cout << "[" << *i << "] ";
                type(space(1, 5) + "    What do you want to buy? [   ]");
                cout << "\b\b\b";
                // sourced from: https://www.geeksforgeeks.org/vectorpush_back-vectorpop_back-c-stl/
            }
            cin >> choice;
            if (choice == 0) return 0;
            /// TODO: Error trap

            if (find(buyableItems.begin(), buyableItems.end(), choice) != buyableItems.end()) {
                for (int i = 0; i < sizeof(SHOP_PRICES) / sizeof(SHOP_PRICES[0]); i++) {
                    if (SHOP_PRICES[i][0] == choice) {
                        inv[SHOP_PRICES[i][1]]++; // NOTE: Each item is bought one at a time
                        for (int j = 2; j <= 6; j++) {
                            inv[j - 2] -= SHOP_PRICES[i][j];
                        }
                        if (SHOP_PRICES[i][7]) inv[items::coins] -= SHOP_PRICES[i][7];
                        cout << space(0, 4) << "You received a " << ENUM_TO_ITEM[SHOP_PRICES[i][1]] << ". Thank you for your purchase!";
                        pause(2);
                        system("cls");
                        break;
                    }
                }
            }
            break;
        }
        else if (shopChoice == 2) {
            system("cls");
            type(space(3, 4) + "    You currently have: ");
            int worth[5] = { // keep user balance as INT - double values are unneeded in terms of precision
                inv[items::logs] * BASE_LOGS,
                inv[items::rocks] * BASE_ROCKS,
                inv[items::iron] * BASE_IRON,
                inv[items::steel] * BASE_STEEL,
                inv[items::obsidian] * BASE_OBSIDIAN
            };
            int n = 0;
            for (int i = 0; i < 5; i++) {
                if (inv[i]) {
                    n = 1;
                    cout << space(1, 5) << "[" << i + 1 << "]" << " - " << inv[i] << " " << ENUM_TO_ITEM[i] << " worth $" << worth[i] << endl;
                }
            }
            if (!n) {
                cout << space(3, 6) << " Nothing!" << space(1, 5) << "\b(you don't have any items...)";
                cout << space(1, 5) << "Enter any key to continue. ";
                char c;
                cin >> c;
                system("cls");
                return 0;
            }
            cout << space(1, 5) << "[0] - Exit";
            cout << space(2, 4) << "    Which item would you like to sell today? ";
            int item;
            cin >> item;
            if (item == 0) return 0;
            /// TODO: Error trap
            cout << space(1, 4) << "    How many of this item do you want to sell? ";
            int count;
            cin >> count;
            if (count == 0) return 0;
            /// TODO: Error trap the if statement
            /*
            int a = count < inv[item - 1];
            cout << "Item: " << item - 1 << endl;
            cout << "Count: " << inv[item - 1] << endl;
            cout << "Entered: " << count << endl;
            cout << "T/F: " << a << endl;
            cout << "COST: " << COSTS[item - 1] << endl;
            cout << "Cost: " << count * COSTS[item - 1] << endl;
            */
            if (count <= inv[item - 1] && count * COSTS[item - 1] > 0) {
                inv[items::coins] += count * COSTS[item - 1];
                inv[item - 1] -= count;
                cout << space(0, 6) << "Thanks for your offer! (+" << count * COSTS[item - 1] << ")" << endl;
                pause(2);
                system("cls");
                break;
            }
        }
        else if (shopChoice == 3) {

        }
    }
    while (shopChoice != 0);
    return 0;
}

int rng(int p) {
    int n = rand() % 100 + 1;
    if (n <= p) return 1;
    else return 0;
}

void *gatherResources(int day, int inv[]) {
    system("cls");

    if (day == 15 || day == 30 || day == 40 || day == 50 || day == 60) {
        print(space(3, 5) + "\b\b\b\bYou have an eerie feeling about the island...", 2);
        print(space(3, 5) + "No resources can be gathered today.", 2);
        return 0;
    }

    srand(time(0));
    int logs = rand() % 3 + 2, rocks = rng(65), iron = 0, steel = 0, obsidian = 0;
    int weather;

    print(space(3, 5) + "\b\b\b", 2);
    // ASCII downtime

    int n = rand() % 100 + 1;
    if (n <= 60) { weather = 0; }
    else if (n <= 95) { weather = 1; }
    else weather = 2;
    if (inv[items::axe]) {
        logs = (weather == 0)
            ? rand() % 5 + 8
            : (weather == 1)
                ? rand() % 2 + 1
                : rand() % 20 + 50;
        rocks = (weather == 0)
            ? rand() % 4 + 8
            : (weather == 1)
                ? rand() % 2 + 1
                : rand() % 15 + 25;
        steel = rng(90) * (rand() % 2 + 3);
        obsidian = rand() % 2 + 1;
        iron = rng(75) * (rand() % 2 + 2);
        type("You ventured off into a steep mountainside.");
    }
    else if (inv[items::hammer]) {
        logs = (weather == 0)
            ? rand() % 3 + 4
            : (weather == 1)
                ? rand() % 2 + 1
                : rand() % 15 + 20;
        rocks = (weather == 0)
            ? rand() % 4 + 3
            : (weather == 1)
                ? rand() % 2 + 1
                : rand() % 15 + 10;
        steel = rng(60) * (rand() % 2 + 2);
        obsidian = rng(40);
        iron = rng(65) * (rand() % 2 + 1);
        type("You descended down a large valley.");
    }
    else if (inv[items::pickaxe]) {
        logs = (weather == 0)
            ? rand() % 3 + 4
            : (weather == 1)
                ? rand() % 2 + 1
                : rand() % 15 + 20;
        rocks = (weather == 0)
            ? rand() % 4 + 3
            : (weather == 1)
                ? rand() % 2 + 1
                : rand() % 15 + 10;
        iron = rng(25);
        type("You journeyed deep into the forest.");
    }
    else {
        type("You walked on a small path along the shoreline.");
        pause(1);
        generateShoreLine();
        pause(2);
    }


    int res[5] = {logs, rocks, iron, steel, obsidian};
    string str[5] = {"Logs", "Rocks", "Iron", "Steel", "Obsidian"};
    inv[items::logs] += logs;
    inv[items::rocks] += rocks;
    inv[items::iron] += iron;
    inv[items::steel] += steel;
    inv[items::obsidian] += obsidian;
    type(space(2, 4));
    type(
         (weather == 0)
            ? "\b\b\bThe weather was fair today :). As you returned, you now gained:"
            : (weather == 1)
                ? "\b\b\bThe weather was rainy today :( As you returned, you gained:"
                : "\b\b\bThe weather was brilliant today!!. As you returned, you now obtained:");
    cout << "\n\n";
    for (int i = 0; i < sizeof(res) / sizeof(int); i++) {
        if (res[i] > 0) cout << space(0, 5) << "+ " << res[i] << " " << str[i] << endl;
    }
    pause(4);
    print(space(3, 5) + "Enter any key to continue. ");
    char c;
    cin >> c;
    system("cls");
    return 0;
};

class Robot {
    public:
        int health;
        int atkspd; // 1 attack per <atkspd> seconds
        int damage;
        int spdamage;
        int spchance; // % of special attack (0 - 100)
        int agility; // % of evasion (0 - 100)
        string dialog[];
};

void battle(int stats[], Robot r1, Robot r2, Robot r3) {
    for (int i = 0; i < sizeof(r1.dialog) / sizeof(string); i++) {
        if (i % 2) pause(r1.dialog[i]);
        else type(r1.dialog[i]);
    }
    int stime = time(0);
    if (!r2.health) { // Day 15, 30
        do {
            cout << space(3, 0);
            cout << space(0, 3) << "[???]" << space(0, 3) << endl;
            cout << space(0, 3) << "Health: " << r1.health << space(0, 3) << stats[0] << endl;
            do {
                stime = time(0);
                cout << space(2, 3) << "Enter [0] to attack";
                cout << space(2, 3) << "Cooldown >--------------------"

            } while (time(0) < stime + 1)
        }
    }
    else if (!r3.health) { // Day 40, 50

    }
    else { // Day 60

    }
}

void *wander(int day, int inv[], int stats[], string name) {
    if (day == 15) {
        Robot r1;
        r1.health = 5;
        r1.atkspd = 5;
        r1.damage = 1;
        r1.spdamage = 2;
        r1.spchance = 40;
        r1.agility = 0;
        r1.dialog = [
            space(0, 4) + "         The sky shifted into a dark violet hue.", 3
            space(0, 4) + "     A small red light descends upon your very eyes.", 4
            space(0, 4) + "An enemy robot appears in front of you. Prepare for battle!", 2
        ];
    }
    else if (day == 30) {

    }
    else if (day == 40) {

    }
    else if (day == 50) {

    }
    else if (day == 60) {

    }

    if(rng(30)) {
        if (day < 15) {

        }
        else if (day < 30) {

        }
        else if (day < 40) {

        }
        else if (day < 50) {

        }
        else if (day < 60) {

        }
        else if (day == 60) {

        }
    }
    return 0;
};

void showInventory(int inv[]) {
    system("cls");

    cout << space(5, 4);
    type("  As you went through your bag, you saw:");
    pause(2);
    cout << space(3);

    int n = 0;
    for (int i = 0; i < 20; i++) {
        if (i == 19 && n) cout << space(2, 5) << "    You also have $" << inv[i] << "!";
        else if (inv[i]) {
            if (!n) n = 1;
            cout << space(1, 6) << "- " << inv[i] << " " << ENUM_TO_ITEM[i] << endl;
        }
    }
    if (!n) cout << space(3, 6) << " Nothing!" << space(1, 5) << "\b(you don't have any items...)";

    pause(2);
    print(space(3, 5) + "Enter any key to continue. ");
    char c;
    cin >> c;
    system("cls");
    return;
}
