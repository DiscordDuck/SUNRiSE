#include <iostream>
#include <windows.h>
#include <time.h>
#include "drivers.h"
#include "ascii.h"
#include "interactions.h"

using namespace std;
int day = 1, tutorialHP = 999, stats[4] = {10, 3, 1, 20}; // health, attack speed, damage, agility
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
    /// TODO: Error Trap, str length

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
        cout << space(10);
        cout << " •" << space(0, 14) << "•\b";
        pause(2);
    }
    pause(4);
    /// TODO: Add unicode instead of dot point

    system("cls");
    shop(true, inv);

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

    /// TODO: Title Screen + short on-screen tutorial (instant) e.g. You will have 60 Days to escape the island before the war is over.\n + full intro Each day, you can do ONE activity. + pacing fast or slow
    /// NOTE: ENTERING MAIN LOOP

    while (!END) {
        /// TODO: SUNRiSE, ASCII ("Day 1" + extra quote or something or TIP: Chat with Merchant for game-related hints)
        type(space(3, 6) + "The Sun has risen.\n");
        pause(1);

        int activityChoice;
        do {
            cout << space(2, 7) << "\b\b\b(Day " << day << ")" << space(1); /// TODO: Assisting popups when shop items are available
            generateActivityOptions();
            pause(2);

            string activities[4] = {"Trade with Local Merchants", "Gather Resources", "Wander Around the Island", "View Your Inventory"};

            cout << space(2, 5) << "What do you want to do today? ";

            cin >> activityChoice;
            /// TODO: Error Trap!!! - probably make a global error trap driver function

            if (activityChoice == CHEAT_CODE) {
                inv[items::logs] += 500;
                inv[items::rocks] += 200;
                inv[items::iron] += 50;
                inv[items::coins] += 350;
                activityChoice = 4;
                system("cls");
                continue;
            }
            cout << space(2, 5) << "< " << activities[activityChoice - 1] << " >";
            pause(activityChoice == 4 ? 1 : 2);

            if (activityChoice == 1) {
                shop(false, inv);
                system("cls");
                activityChoice = 4;
            }
            else if (activityChoice == 2) { gatherResources(inv); }
            else if (activityChoice == 3) { wander(day, inv, stats, name); }
            else if (activityChoice == 4) { showInventory(inv); }
        } while (activityChoice == 4);
        day++;
    }

    return 0;
}
