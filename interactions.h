#ifndef INTERACTIONS_H_INCLUDED
#define INTERACTIONS_H_INCLUDED

void *shop(bool tutorialState, int inv[]);
void init(double LOGS, double ROCKS, double IRON, double STEEL, double OBSIDIAN);
void *gatherResources(int day, int inv[]);
void *wander(int day, int inv[], int stats[], std::string name);
void showInventory(int inv[]);


#endif // INTERACTIONS_H_INCLUDED
