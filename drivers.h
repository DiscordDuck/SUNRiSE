#ifndef DRIVERS_H_INCLUDED
#define DRIVERS_H_INCLUDED

inline const int RELEASE_TARGET = 0;

void pause(int seconds = 1);
void print(std::string str, int t = 0);
void type(std::string str);
std::string space(int vertical = 0, int horizontal = 0);


#endif // DRIVERS_H_INCLUDED
