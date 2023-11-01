#include <iostream>
#include <windows.h>
#include <time.h>
#include "drivers.h"
#include "ascii.h"
#include "interactions.h"

using namespace std;
int day = 1, stats[4] = {10, 4, 1, 20}; // health, attack speed, damage, agility
int END = 0, startTime;

int inv[20] = { };
// Use ENUM to locate index, item = itemName

const int CHEAT_CODE = 727;

/// NOTE: If base values are changed, please manually change ASCII sell prices in ascii.cpp
/// const double ITEM = $, for selling only
const double BASE_LOGS = 4;
const double BASE_ROCKS = 2;
const double BASE_IRON = 40;
const double BASE_STEEL = 100;
const double BASE_OBSIDIAN = 600;

int main()
{
    SetConsoleOutputCP(CP_UTF8);
    startTime = time(0);
    srand(startTime);
    init(BASE_LOGS, BASE_ROCKS, BASE_IRON, BASE_STEEL, BASE_OBSIDIAN);

    /// add system("cls") per line, indents "\t"
    print("\n\n");
    print("Welcome to SUNRiSE.\n", 3);
    /// add history here
    print("You have crashed onto an island with your robot suit.\n", 4);
    generateRobot();
    pause(3);
    print("Maybe one day, you will escape.", 1);
    print(".", 1);
    print(".", 2);
    print(" Or be trapped here forever.\n", 3);
    cout << "For now, enter a name: ";

    string name;
    cin >> name;
    while ( !cin.good() ) {
        if( !cin.good() )
        {
            cin.clear();
            int n;
            cin >> n;
        }
        cin >> name;
    };

    string prohibitedNames[3] = {"chow", "Chow", "CHOW"};
    for (int i = 0; i < sizeof(prohibitedNames) / sizeof(string); i++) {
        if (name.find(prohibitedNames[i]) != string::npos) name = "Profanity Detected";
    }

    pause(2);
    int rotations = rand() % 3 + 5;
    for (int i = 0; i < rotations; i++) {
        system("cls");
        // wchar_t lt = L'\u256D', rt = L'\u256E', lb = L'\u2570', rb = L'\u256F';
        cout << " •" << space(0, 14) << "•";
        cout << space(6, 6) + "Welcome, " << name << endl;
        cout << space(3, 6) << "Loading";
        for (int j = 0; j < i % 3 + 1; j++) { cout << " ."; }
        cout << space(10, 2) << " NOTE: For a better experience, resize the console window to the printed dots";
        cout << space(3, 3);
        cout << "*** TIP: Chat with the Merchant for some game-related hints!";
        cout << space(6);
        cout << " •" << space(0, 14) << "•\b";
        pause(2);
    }
    pause(4);

    system("cls");
    shop(true, day, inv);

    system("cls");
    type(space(3, 4) + "Oh? You don't have anything?");
    print("\n\n", 2);
    type(space(0, 6) + "<  your empty pockets rustle... >");
    print("\n\n", 1);
    type(space(0, 4) + "Ah...");
    print("\n\n", 2);
    type(space(0, 4) + "Well, one man's trash is another one's treasure is what they say.");
    print("\n\n", 1);
    type(space(0, 4) + "I'll always be here to trade for YOUR RESOURCES.");
    print("\n\n", 2);
    type(space(0, 6) + "< you leave. >");
    pause(2);
    system("cls");
    pause(2);

    /// NOTE: ENTERING MAIN LOOP

    while (!END) {
        type(space(3, 6) + "The Sun has risen.\n");
        pause(1);

        int activityChoice;
        int foughtBattle = 0;
        do {
            cout << space(2, 7) << "\b\b\b(Day " << day << ")" << space(1);
            generateActivityOptions();
            pause(2);

            string activities[4] = {"Trade with Local Merchants", "Gather Resources", "Wander Around the Island", "View Your Inventory"};

            cout << space(2, 5) << "What do you want to do today? ";

            activityChoice = errorTrap(0, 4);

            if (activityChoice == CHEAT_CODE) {
                inv[items::logs] += 500;
                inv[items::rocks] += 200;
                inv[items::iron] += 50;
                inv[items::steel] += 60;
                inv[items::obsidian] += 60;
                inv[items::coins] += 2000;
                activityChoice = 4;
                system("cls");
                continue;
            }
            cout << space(2, 5) << "< " << activities[activityChoice - 1] << " >";
            pause(activityChoice == 4 ? 1 : 2);

            if (activityChoice == 1) {
                shop(false, day, inv);
                system("cls");

                if (inv[items::spear]) stats[2] = 2;
                if (inv[items::armour]) stats[0] = 20;
                activityChoice = 4;
            }
            else if (activityChoice == 2) { gatherResources(day, inv); }
            else if (activityChoice == 3) {
                wander(day, inv, stats, name);
                foughtBattle = 1;
            }
            else if (activityChoice == 4) { showInventory(inv); }
            if ((day == 15 || day == 30 || day == 40 || day == 50 || day == 60) && !foughtBattle) activityChoice = 4;
            foughtBattle = 0;
        } while (activityChoice == 4);

        if (day == 60 && foughtBattle) {
            break;
        }
        day++;
    }

    type(space(5, 3) + "The sun rises from the east horizon, illuminating the full island.");
    pause(5);
    type(space(2, 3) + "     Your robot is finally repaired and can fly again.");
    pause(1);
    cout << ".";
    pause(1);
    cout << ".";
    pause(2);
    type(space(1, 3) + "        Of course, with the help of the merchant.");
    pause(4);
    type(space(3, 3) + "As you flew away, you bid goodbye to the island in SUNRiSE.");
    pause(3);
    cout << space(2, 6) << "Total time: " << (time(0) - startTime) / 60 << " m " << (time(0) - startTime) % 60 << "s";

    return 0;
}
