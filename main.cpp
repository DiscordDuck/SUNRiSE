#include <iostream>
#include <windows.h>
#include <time.h>
#include "drivers.h"
#include "ascii.h"
#include "interactions.h"

using namespace std;
int day = 1, tutorialHP = 999, hp = 3;
int END = 0;

int inv[20] = { };
enum items { logs = 0, rocks = 1, iron = 2, steel = 3, obsidian = 4, pickaxe = 5, hammer = 6, axe = 7 } item;
// Use ENUM to locate index, item = itemName

/// NOTE: C++23 print() might clash with imported print() from drivers.h file

/// NOTE: If base values are changed, please manually change ASCII sell prices in ascii.cpp
/// const double ITEM = $
const double BASE_LOGS = 4;
const double BASE_ROCKS = 2;
const double BASE_IRON = 40;
const double BASE_STEEL = 100;
const double BASE_OBSIDIAN = 600;

int main()
{
    SetConsoleOutputCP(CP_UTF8);
    srand(time(0));
    init(BASE_LOGS, BASE_ROCKS, BASE_IRON, BASE_STEEL, BASE_OBSIDIAN);

    /// add system("cls") per line, indents "\t"
    print("\n\n");
    print("Welcome to SUNRiSE.\n", 3);
    /// add history here
    print("You have crashed onto an island with your robot suit.\n", 4);
    print("ASCII HERE\n", 3);
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
        cout << "•" << space(0, 14) << "•";
        cout << space(6, 6) + "Welcome, " << name << endl;
        cout << space(3, 6) << "Loading";
        for (int j = 0; j < i % 3 + 1; j++) { cout << " ."; }
        cout << space(10, 2) << " NOTE: For a better experience, resize the console window to the printed dots";
        cout << space(10);
        cout << "•" << space(0, 14) << "•\b";
        pause(2);
    }
    pause(4);
    /// TODO: Add unicode instead of dot point

    system("cls");
    shop(true);

    system("cls");
    print("Oh? You don't have anything?\n", 1);
    print("\t\t<  your empty pockets rustle... >\n", 1);
    print("Ah, that sucks.\n", 1);
    print("Well, remember to come back here if you find anything new.\n", 1);
    print("I'll always be here to trade my junk for RESOURCES.\n");
    print("< You leave. >", 2);
    pause(5); // TESTING ONLY
    system("cls");
    pause(2);

    /// TODO: Title Screen + short on-screen tutorial (instant) e.g. You will have 60 Days to escape the island before the war is over.\n + full intro Each day, you can do ONE activity.
    /// NOTE: ENTERING MAIN LOOP

    while (!END) {
        /// TODO: SUNRiSE, ASCII ("Day 1" + extra quote or something or TIP: Chat with Merchant for game-related hints)
        print(space(3, 6) + "The Sun has risen.\n", 1);
        
        cout << space(2, 0);
        generateActivityOptions();
        pause(1);

        cout << space(0, 5) << "What do you want to do today? ";
        int activityChoice;
        cin >> activityChoice;
        /// TODO: Error Trap

        if (activityChoice == 1) { shop(false); }
        else if (activityChoice == 2) {
            
        }
        else if (activityChoice == 3) {

        }
    }

    return 0;
}
