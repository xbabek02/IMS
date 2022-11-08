#ifndef __GRID_HPP__
#define __GRID_HPP__

#define FIGHTER 'F'
#define BOMBER 'B'
#define AIR ' '

#include <vector>

#include "distance.hpp"

using namespace std;

class Grid
{
private:
    int width = 30;
    int length = 120;
    vector<int> center;
    vector<char> field;

public:
    Grid(int length, int width);
    void Display();
    char GetAt(int x, int y);
    void SetAt(int x, int y, char c);

    void DrawBattlefield(int radius, vector<int> center);
    void DrawTargetZone(int radius, vector<int> center);

    ~Grid();
};

#endif