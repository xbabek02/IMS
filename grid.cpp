#include "grid.hpp"
#include <iostream>

Grid::Grid()
{
    for (int y = 0; y < heigth; y++)
    {
        for (int x = 0; x < width; x++)
        {
            field.push_back(AIR);
        }
    }
}

void Grid::Display()
{
    for (int y = 0; y < heigth; y++)
    {
        for (int x = 0; x < width; x++)
        {
            cout << GetAtCors(x, y);
        }
        cout << endl;
    }
}

inline char Grid::GetAtCors(unsigned x, unsigned y)
{
    return field.at(y * width + x);
}

Grid::~Grid()
{
}