#ifndef __GRID_HPP__
#define __GRID_HPP__

#define ATTACKER 'A'
#define DEFENDER 'D'
#define BOMBER 'B'
#define AIR ' '
#define AIRPORT 'X'

#include <vector>
#include <iostream>
#include <cmath>

#include "distance.hpp"

using namespace std;

class Grid
{
private:
    int size;
    vector<int> center;
    vector<char> field;

public:
    Grid(int size);
    void Display();
    char GetAt(int x, int y);
    void SetAt(int x, int y, char c);
    int GetSize() const;

    void DrawBattlefield(int radius, vector<int> center);
    void DrawTargetZone(int radius, vector<int> center);

    ~Grid();
};

#endif