#include <iostream>
#include <time.h>
#include "drivers.h"
#include "ascii.h"
#include "interactions.h"
using namespace std;

void shop(bool tutorialState) {
    cout << space(5, 4);
    type("\b\bWalking through the forest, a merchant's shop appears.\n\n");
    generateMerchantsHouse();
    // 1: View Shop, 2: Sell Items, 3: Chat with Merchant

    cout << space(0, 4);
    type("Welcome, traveller. What are you here for? ");
    pause(1);
    cout << "[   ]\b\b\b";

    int shopChoice;
    cin >> shopChoice;
    /// TODO: Error Trap

    do {
        if (shopChoice == 1) {
            system("cls");

            int choice = 0;

            cout << space(5, 6) << "\b\bMerchant's Shop" << endl << space(0, 2) << ".-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-.";
            if (tutorialState) generateTShop();
            // else generateFShop();
            
            if (tutorialState) {
                pause(4);
                cout << space (1, 5) << "Enter any key to continue. ";
                char c;
                cin >> c;
                shopChoice = 0;
                return;
            }
            cin >> choice;
        }
        else if (shopChoice == 2) {

        }
        else if (shopChoice == 3) {

        }
    }
    while (shopChoice != 0);
    return;
}

double BASE_LOGS;
double BASE_ROCKS;
double BASE_IRON;
double BASE_STEEL;
double BASE_OBSIDIAN;

void init(double LOGS, double ROCKS, double IRON, double STEEL, double OBSIDIAN) {
    BASE_LOGS = LOGS;
    BASE_ROCKS = ROCKS;
    BASE_IRON = IRON;
    BASE_STEEL = STEEL;
    BASE_OBSIDIAN = OBSIDIAN;
}

void resources();
void wander();
