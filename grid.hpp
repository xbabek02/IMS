#ifndef __GRID_HPP__
#define __GRID_HPP__

#define FIGHTER 'F'
#define BOMBER 'B'
#define AIR ' '

#define CORS(x, y) field.at(y *width + x)

#include <vector>

using namespace std;

class Grid
{
private:
    int width = 120;
    int heigth = 30;
    vector<char> field;

public:
    Grid(/* args */);
    void Display();
    char GetAtCors(int x, int y);

    ~Grid();
};

#endif