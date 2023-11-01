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

bool upgrades = false;

void *shop(bool tutorialState, int day, int inv[]) {

    int shopChoice;

    do {
        system("cls");

        cout << space(5, 4);
        type("\b\bWalking through the forest, a merchant's shop appears.\n\n");
        generateMerchantsHouse();
        // 1: View Shop, 2: Sell Items, 3: Chat with Merchant
        cout << space(0, 4);
        type("Welcome, traveller. What are you here for? ");
        pause(1);
        cout << "[   ]\b\b\b";
        shopChoice = errorTrap(0, 4);

        if (shopChoice == 1) {
            system("cls");

            int choice;

            cout << space(5, 6) << "\b\bMerchant's Shop" << endl << space(0, 2) << ".-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-.";
            if (tutorialState) generateTShop();
            else if (!inv[items::hammer]) generateTShop();
            else if (!inv[items::axe]) generateHShop();
            else if (!upgrades) generateHShop();
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
            choice = errorTrap(0, 6);
            if (choice == 0) return 0;

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
            item = errorTrap(0, 4);
            if (item == 0) return 0;
            
            cout << space(1, 4) << "    How many of this item do you want to sell? ";
            int count;
            cin >> count;
            if (count == 0) return 0;
            while (count > inv[item - 1] || count * COSTS[item - 1] <= 0 || !cin.good() ) {
                if( !cin.good() ) {
                    cin.clear();
                    string str;
                    cin >> str;
                }
                cin >> count;
            };

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
            system("cls");
            if (tutorialState) {
                type(space(5, 4) + "You look new here. Why are you here?");
                pause(2);
                return 0;
            }
            type(space(5, 6) + "Today is Day ");
            cout << day << ".";
            if (day < 15) {
                type(space(2, 4) + "Buy some of my gear to get more resources quicker!");
                pause(2);
            }
            else if (day < 30) {
                type(space(2, 4) + "Be wary of Day 30! Something might happen on that day...");
                pause(2);
            }
            else if (day < 40) {
                type(space(2, 4) + "Day 40 sounds horrible. A tool from my shop would help.");
                pause(2);
            }
            else if (day < 50) {
                type(space(2, 4) + "Give it up for Day 50!! Or... don't give up on day 50.");
                pause(2);
            }
            else {
                type(space(2, 4) + "If only war would end in the next 10 days...");
                pause(2);
            }
            cout << space(1, 5) << "Enter any key to continue. ";
            char c;
            cin >> c;
            return 0;
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
        pause(2, 1);
        system("cls");
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
        int agility; // % of evasion (0 - 100)
        vector<string> dialog;
        vector<string> wdialog;
        vector<string> ldialog;
};

void basicField(int Rpos, int Upos) {
    for (int i = 0; i < Rpos - 1; i++) cout << "_";
    cout << "M";
    for (int i = 0; i < Upos - Rpos - 1; i++) cout << "_";
    cout << "o";
    for (int i = 0; i < 20 - Upos; i++) cout << "_";
}

void attack(int Rpos, int Upos, int Rhealth, int Uhealth, int shotByUser) {
    if (shotByUser) {
        for (int i = 0; i < Rpos - 1; i++) cout << "_";
        cout << "M";
        for (int i = 0; i < Upos - Rpos - 1; i++) cout << "_";
        cout << "o";
        for (int i = 0; i < 19 - Upos; i++) cout << "_";
        mspause(42);
        cout << "\r                    ";

        for (int i = 0; i < Rpos - 1; i++) cout << "_";
        if (Upos - Rpos == 1) cout << "_";
        else cout << "M";
        for (int i = 0; i < Upos - Rpos - 2; i++) cout << "_";
        if (Upos - Rpos != 1) cout << "\\";
        cout << "\\o";
        for (int i = 0; i < 19 - Upos; i++) cout << "_";
        mspause(300);

        if (Upos - Rpos == 1 && Rhealth == 1) return;
        cout << "\r                    ";

        for (int i = 0; i < Rpos - 1; i++) cout << "_";
        cout << "M";
        for (int i = 0; i < Upos - Rpos - 1; i++) cout << "_";
        cout << "o";
        for (int i = 0; i < 19 - Upos; i++) cout << "_";
    }
    else {
        for (int i = 0; i < Rpos - 1; i++) cout << "_";
        cout << "M";
        for (int i = 0; i < (Upos - Rpos - 1) / 3; i++) cout << "_";
        cout << "-";
        for (int i = 0; i < (Upos - Rpos - 1) * 2 / 3; i++) cout << "_";
        cout << "o";
        for (int i = 0; i < 19 - Upos; i++) cout << "_";
        mspause(150);

        for (int i = 0; i < Rpos - 1; i++) cout << "_";
        cout << "M";
        for (int i = 0; i < (Upos - Rpos - 1) * 2/ 3; i++) cout << "_";
        cout << "-";
        for (int i = 0; i < (Upos - Rpos - 1) / 3; i++) cout << "_";
        cout << "o";
        for (int i = 0; i < 19 -  Upos; i++) cout << "_";
        mspause(150);

        for (int i = 0; i < Rpos - 1; i++) cout << "_";
        cout << "M";
        for (int i = 0; i < Upos - Rpos - 1; i++) cout << "_";
        if (Uhealth == 1) cout << "_";
        else cout << "o";
        for (int i = 0; i < 19 -  Upos; i++) cout << "_";
    }
    mspause(100);
};

int fight(int stats[], Robot r1, Robot user, string name) {
    pause(2, 1);

    int stime, nextRAttack, nextUAttack, Rdiff, Udiff, Rpos, Upos;
    char userinput = 5;
    stime = time(0);
    user.health = stats[0];
    nextRAttack = stime + r1.atkspd;
    nextUAttack = stime + stats[1];
    Rdiff = nextRAttack - stime;
    Udiff = nextUAttack - stime;
    Rpos = 8;
    Upos = 15;
    do {
        system("cls");
        cout << space(3, 0);
        cout << space(0, 3) << "[???]" << space(0, 4) << name << endl;
        cout << space(0, 3) << "Health: " << r1.health << space(0, 3) << "Health: " << user.health << endl;
        stime = time(0);
        cout << space(2, 3) << "Enter [0] to attack | Use [A D] to move";
        cout << space(2, 3) << "Cooldown >";
        for (int i = 0; i < 20 * ((nextUAttack - time(0)) / stats[1]); i++) {
            cout << "-";
        }
        cout << space(6, 4);

        basicField(Rpos, Upos);

        cin >> userinput;
        // error trap unneeded for chars?

        if (time(0) > nextRAttack) {
            attack(Rpos, Upos, r1.health, user.health, 0);
            int multiplier = r1.damage * (time(0) - nextRAttack) / r1.atkspd + 1;
            if (!rng(stats[3])) user.health -= multiplier;
            nextRAttack = time(0) + r1.atkspd + 1;
        }
        if (userinput == '0') {
            cin.ignore();
            if (time(0) > nextUAttack) {
                if (Upos - Rpos == 1) {
                    if (rng(100 - r1.agility)) r1.health -= user.damage;
                    Upos += 3;
                    Rpos -= 3;
                    if (Upos > 20) Upos = 20;
                    if (Rpos < 1) Rpos = 1;
                }
                attack(Rpos, Upos, r1.health, user.health, 1);
                nextUAttack = time(0) + stats[1];
            }
        }
        else {
            if (userinput == 'A' || userinput == 'a') {
                cin.ignore();
                if (Upos - Rpos == 1) {
                    Upos--;
                    Rpos--;
                }
                else {
                    if (Upos > 2) Upos--;
                    rng(60) ? Rpos++ : Rpos--;
                }
            }
            else if (userinput == 'D' || userinput == 'd') {
                cin.ignore();
                if (Upos < 20) Upos++;
                rng(60) ? Rpos++ : Rpos--;
            }
            if (Rpos == Upos) Upos = Rpos + 1; // edge case
            if (Rpos > 20) Rpos = 20;
            else if (Rpos < 1) Rpos = 1;
            mspause(500);
        }
    } while(r1.health > 0 && user.health > 0);
    if (user.health > 0) return 1;
    else return 0;
}

int battle(int stats[], Robot r1, Robot r2, Robot r3, string name) {
    Robot user;
    user.health = stats[0];
    user.damage = stats[2];
    for (string i : r1.dialog) {
        type(i);
        pause(2);
    }
    int stime, nextRAttack, nextUAttack, Rdiff, Udiff, Rpos, Upos;
    char userinput = 5;
    if (!r2.health) { // Day 15, 30
        int i = fight(stats, r1, user, name);
        if (i) user.health = 1;
        else user.health = 0;
    }
    else if (!r3.health) { // Day 40, 50
        int i = fight(stats, r1, user, name);
        if (i) {
            system("cls");
            int j = fight(stats, r2, user, name);
            if (j) user.health = 1;
            else user.health = 0;
        }
        else user.health = 0;
    }
    else { // Day 60
        int i = fight(stats, r1, user, name);
        if (i) {
            system("cls");
            int j = fight(stats, r2, user, name);
            if (j) {
                system("cls");
                int k = fight(stats, r3, user, name);
                if (k) user.health = 1;
                else user.health = 0;
            }
            else user.health = 0;
        }
        else user.health = 0;
    }

    if (user.health > 0) {
        for (string i : r1.wdialog) {
            type(i);
            pause(2);
        }
        return 1;
    }
    else {
        for (string i : r1.ldialog) {
            type(i);
            pause(2);
        }
        return 0;
    }
}

void *wander(int& day, int inv[], int stats[], string name) {
    system("cls");
    if (day == 15) {
        Robot r1, r2, r3;
        r2.health = 0;
        r1.health = 5;
        r1.atkspd = 5;
        r1.damage = 1;
        r1.agility = 0;
        string dialog_[3] = {
            space(3, 4) + "         The sky shifts into a dark violet hue.",
            space(1, 4) + "     A small red light descends upon your very eyes.",
            space(1, 4) + "An enemy robot appears in front of you. Prepare for battle!",
        };
        string wdialog_[5] = {
            space(3, 4) + "     As the evening sun started to dim, the robot fell.",
            space(1, 4) + " There were a few items you scavenged from its body's scraps.",
            space(2, 4) + "            Congratulations on your first victory!",
            space(1, 4) + "       You can now upgrade your stats with the merchant.",
            space(2, 4) + "              Beware for another invasion soon......",
        };
        string ldialog_[4] = {
            space(3, 4) + "         You lost the battle as the robot defeated you.",
            space(1, 4) + "  Better luck next time! You have been backtracked to Day 5.",
            space(1, 4) + "    As help, you have a few extra resources in your inventory.",
            space(2, 4) + "                          Good luck!",
        };
        for (int i = 0; i < 3; i++) r1.dialog.push_back(dialog_[i]);
        for (int i = 0; i < 5; i++) r1.wdialog.push_back(wdialog_[i]);
        for (int i = 0; i < 4; i++) r1.ldialog.push_back(ldialog_[i]);

        int res = battle(stats, r1, r2, r3, name);
        if (res) {
            inv[items::logs] += 25;
            inv[items::rocks] += 15;
            inv[items::iron] += 5;
            upgrades = true;
        }
        if (!res) {
            inv[items::logs] += 15;
            inv[items::rocks] += 10;
        }
    }
    else if (day == 30) {
        Robot r1, r2, r3;
        r2.health = 0;
        r1.health = 10;
        r1.atkspd = 4;
        r1.damage = 1;
        r1.agility = 10;
        string dialog_[3] = {
            space(3, 4) + "          The sky dims into a dark shade of blue.",
            space(1, 4) + "     One red light shines brightly amongst the clouds.",
            space(1, 4) + "An enemy robot appears in front of you. Prepare for battle!",
        };
        string wdialog_[4] = {
            space(3, 4) + "     As the evening sun started to dim, the robot fell.",
            space(1, 4) + " There were a few items you scavenged from its body's scraps.",
            space(2, 4) + "            Congratulations on your second victory!",
            space(2, 4) + "        The war rages on, and more enemies will come......",
        };
        string ldialog_[4] = {
            space(3, 4) + "         You lost the battle as the robot defeated you.",
            space(1, 4) + "  Better luck next time! You have been backtracked to Day 20.",
            space(1, 4) + "    As help, you have a few extra resources in your inventory.",
            space(2, 4) + "                          Good luck!",
        };
        for (int i = 0; i < 3; i++) r1.dialog.push_back(dialog_[i]);
        for (int i = 0; i < 4; i++) r1.wdialog.push_back(wdialog_[i]);
        for (int i = 0; i < 4; i++) r1.ldialog.push_back(ldialog_[i]);

        int res = battle(stats, r1, r2, r3, name);
        if (res) {
            inv[items::logs] += 50;
            inv[items::rocks] += 30;
            inv[items::iron] += 15;
            inv[items::steel] += 8;
            inv[items::obsidian] += 2;
        }
        if (!res) {
            inv[items::logs] += 35;
            inv[items::rocks] += 20;
            inv[items::iron] += 12;
            day -= 10;
        }
    }
    else if (day == 40) {
        Robot r1, r2, r3;
        r3.health = 0;
        r2.health = 15;
        r2.atkspd = 10;
        r2.damage = 2;
        r2.agility = 10;
        r1.health = 15;
        r1.atkspd = 4;
        r1.damage = 1;
        r1.agility = 10;
        string dialog_[3] = {
            space(3, 4) + "       The sky shrivels into a pitch black darkness.",
            space(1, 4) + "     Two red lights spin circles amongst the night air.",
            space(1, 4) + "An enemy robot appears in front of you. Prepare for battle!",
        };
        string wdialog_[4] = {
            space(3, 4) + "     As the evening sun started to dim, the robots fell.",
            space(1, 4) + " There were a few items you scavenged from its bodies' scraps.",
            space(2, 4) + "            Congratulations on your third victory!",
            space(2, 4) + "                  The war is at its peak......",
        };
        string ldialog_[4] = {
            space(3, 4) + "         You lost the battle as the robot defeated you.",
            space(1, 4) + "  Better luck next time! You have been backtracked to Day 30.",
            space(1, 4) + "    As help, you have a few extra resources in your inventory.",
            space(2, 4) + "                          Good luck!",
        };
        for (int i = 0; i < 3; i++) r1.dialog.push_back(dialog_[i]);
        for (int i = 0; i < 4; i++) r1.wdialog.push_back(wdialog_[i]);
        for (int i = 0; i < 4; i++) r1.ldialog.push_back(ldialog_[i]);

        int res = battle(stats, r1, r2, r3, name);
        if (res) {
            inv[items::logs] += 100;
            inv[items::rocks] += 60;
            inv[items::iron] += 30;
            inv[items::steel] += 12;
            inv[items::obsidian] += 18;
        }
        if (!res) {
            inv[items::logs] += 50;
            inv[items::rocks] += 30;
            inv[items::iron] += 20;
            inv[items::obsidian] += 10;
            day -= 10;
        }
    }
    else if (day == 50) {
        Robot r1, r2, r3;
        r3.health = 0;
        r2.health = 20;
        r2.atkspd = 8;
        r2.damage = 2;
        r2.agility = 10;
        r1.health = 15;
        r1.atkspd = 5;
        r1.damage = 1;
        r1.agility = 10;
        string dialog_[3] = {
            space(3, 4) + "         The sky fades into a deep red colour.",
            space(1, 4) + "     Two red lights encricle the island in the sky.",
            space(1, 4) + "An enemy robot appears in front of you. Prepare for battle!",
        };
        string wdialog_[4] = {
            space(3, 4) + "     As the evening sun started to dim, the robots fell.",
            space(1, 4) + " There were a few items you scavenged from its bodies' scraps.",
            space(2, 4) + "            Congratulations on your fourth victory!",
            space(2, 4) + "                 The war is soon to be over......",
        };
        string ldialog_[4] = {
            space(3, 4) + "         You lost the battle as the robot defeated you.",
            space(1, 4) + "  Better luck next time! You have been backtracked to Day 40.",
            space(1, 4) + "    As help, you have a few extra resources in your inventory.",
            space(2, 4) + "                          Good luck!",
        };
        for (int i = 0; i < 3; i++) r1.dialog.push_back(dialog_[i]);
        for (int i = 0; i < 4; i++) r1.wdialog.push_back(wdialog_[i]);
        for (int i = 0; i < 4; i++) r1.ldialog.push_back(ldialog_[i]);

        int res = battle(stats, r1, r2, r3, name);
        if (res) {
            inv[items::logs] += 150;
            inv[items::rocks] += 90;
            inv[items::iron] += 50;
            inv[items::steel] += 20;
            inv[items::obsidian] += 18;
        }
        if (!res) {
            inv[items::logs] += 100;
            inv[items::rocks] += 60;
            inv[items::iron] += 40;
            inv[items::obsidian] += 10;
            day -= 10;
        }
    }
    else if (day == 60) {
        Robot r1, r2, r3;
        r3.health = 25;
        r3.atkspd = 6;
        r3.damage = 2;
        r3.agility = 20;
        r2.health = 20;
        r2.atkspd = 8;
        r2.damage = 2;
        r2.agility = 15;
        r1.health = 15;
        r1.atkspd = 4;
        r1.damage = 1;
        r1.agility = 15;
        string dialog_[3] = {
            space(3, 4) + "         The sky explodes into a fiery red aura.",
            space(1, 4) + "     Three red lights crash down at the speed of light.",
            space(1, 4) + "An enemy robot appears in front of you. Prepare for battle!",
        };
        string wdialog_[1] = {
            space(3, 4) + "     As the evening sun started to dim, the robots fell.",
        };
        string ldialog_[4] = {
            space(3, 4) + "         You lost the battle as the robot defeated you.",
            space(1, 4) + "  Better luck next time! You have been backtracked to Day 50.",
            space(1, 4) + "    As help, you have a few extra resources in your inventory.",
            space(2, 4) + "                          Good luck!",
        };
        for (int i = 0; i < 3; i++) r1.dialog.push_back(dialog_[i]);
        for (int i = 0; i < 1; i++) r1.wdialog.push_back(wdialog_[i]);
        for (int i = 0; i < 4; i++) r1.ldialog.push_back(ldialog_[i]);

        int res = battle(stats, r1, r2, r3, name);
        if (!res) {
            inv[items::logs] += 100;
            inv[items::rocks] += 60;
            inv[items::iron] += 40;
            inv[items::steel] += 20;
            inv[items::obsidian] += 20;
            day -= 10;
        }
    }
    else if(rng(30)) {
        if (day < 15) {
            Robot r1, r2, r3;
            r2.health = 0;
            r1.health = 3;
            r1.atkspd = 6;
            r1.damage = 1;
            r1.agility = 0;
            string dialog_[2] = {
                space(3, 4) + "         You found a weak, moving robot.",
                space(3, 4) + "                 Time for battle!"
            };
            string wdialog_[2] = {
                space(3, 4) + "     As the evening sun started to dim, the robot fell.",
                space(1, 4) + " There were a few items you scavenged from its body's scraps.",
            };
            string ldialog_[4] = {
                space(3, 4) + "         You lost the battle as the robot defeated you.",
                space(1, 4) + "                     Better luck next time!"
            };
            for (int i = 0; i < 2; i++) r1.dialog.push_back(dialog_[i]);
            for (int i = 0; i < 2; i++) r1.wdialog.push_back(wdialog_[i]);
            for (int i = 0; i < 4; i++) r1.ldialog.push_back(ldialog_[i]);

            int res = battle(stats, r1, r2, r3, name);
            if (res) {
                inv[items::logs] += 10;
                inv[items::rocks] += 5;
                inv[items::iron] += 1;
            }
        }
        else if (day < 40) {
            Robot r1, r2, r3;
            r2.health = 0;
            r1.health = 10;
            r1.atkspd = 4;
            r1.damage = 1;
            r1.agility = 10;
            string dialog_[2] = {
                space(3, 4) + "         You found a mysterious, moving robot.",
                space(3, 4) + "                   Time for battle!"
            };
            string wdialog_[2] = {
                space(3, 4) + "     As the evening sun started to dim, the robot fell.",
                space(1, 4) + " There were a few items you scavenged from its body's scraps.",
            };
            string ldialog_[4] = {
                space(3, 4) + "         You lost the battle as the robot defeated you.",
                space(1, 4) + "                     Better luck next time!"
            };
            for (int i = 0; i < 2; i++) r1.dialog.push_back(dialog_[i]);
            for (int i = 0; i < 2; i++) r1.wdialog.push_back(wdialog_[i]);
            for (int i = 0; i < 4; i++) r1.ldialog.push_back(ldialog_[i]);

            int res = battle(stats, r1, r2, r3, name);
            if (res) {
                inv[items::logs] += 20;
                inv[items::rocks] += 10;
                inv[items::iron] += 5;
                inv[items::obsidian] += 1;
            }
        }
        else if (day < 60) {
            Robot r1, r2, r3;
            r2.health = 0;
            r1.health = 15;
            r1.atkspd = 5;
            r1.damage = 2;
            r1.agility = 15;
            string dialog_[2] = {
                space(3, 4) + "         You found a strong-looking robot.",
                space(3, 4) + "                   Time for battle!"
            };
            string wdialog_[2] = {
                space(3, 4) + "     As the evening sun started to dim, the robot fell.",
                space(1, 4) + " There were a few items you scavenged from its body's scraps.",
            };
            string ldialog_[4] = {
                space(3, 4) + "         You lost the battle as the robot defeated you.",
                space(1, 4) + "                     Better luck next time!"
            };
            for (int i = 0; i < 2; i++) r1.dialog.push_back(dialog_[i]);
            for (int i = 0; i < 2; i++) r1.wdialog.push_back(wdialog_[i]);
            for (int i = 0; i < 4; i++) r1.ldialog.push_back(ldialog_[i]);

            int res = battle(stats, r1, r2, r3, name);
            if (res) {
                inv[items::logs] += 50;
                inv[items::rocks] += 25;
                inv[items::iron] += 10;
                inv[items::obsidian] += 3;
            }
        }
    }
    else {
        type(space(3, 5) + "You walked around the island without luck.");
        type(space(1, 6) + "You returned home.");
        pause(2);
    }
    system("cls");
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
