#include "grid.hpp"
#include <iostream>

Grid::Grid(int length, int width) : width(width), length(length)
{
    for (int y = 0; y < width; y++)
    {
        for (int x = 0; x < length; x++)
        {
            field.push_back(' ');
        }
    }
}

void Grid::Display()
{
    for (int y = 0; y < width; y++)
    {
        for (int x = 0; x < length; x++)
        {
            cout << GetAt(x, y);
        }
        cout << endl;
    }
}

inline char Grid::GetAt(int x, int y)
{
    return field.at(y * length + x);
}

inline void Grid::SetAt(int x, int y, char c)
{
    field.at(y * length + x) = c;
}

void Grid::DrawBattlefield(int radius, vector<int> center)
{
    int boundary = 2;

    int x_start, y_start, x_end, y_end;
    x_start = center[0] - radius - boundary;

    x_end = y_end = (center[0] + radius + boundary);

    int inner_square_size = round(sqrt(2) * radius);
    int inner_square_x = center[0] - inner_square_size / 2 + boundary;
    int inner_square_y = center[1] - inner_square_size / 2 + boundary;

    SetAt(center[0], center[1], 'C');

    for (; x_start < x_end; x_start++)
    {
        y_start = center[0] - radius - boundary;
        for (; y_start < y_end; y_start++)
        {
            // inside the circle
            if ((x_start > inner_square_x) && (x_start < (inner_square_x + inner_square_size - boundary * 2)) &&
                (y_start > inner_square_y) && (y_start < (inner_square_y + inner_square_size - boundary * 2)))
            {
                continue;
            }

            int distance = Distance::CountDistance(center, {x_start, y_start, 0});
            if (distance == radius)
            {
                SetAt(x_start, y_start, '@');
            }
        }
    }
}

void Grid::DrawTargetZone(int radius, vector<int> center)
{
    int boundary = 2;

    int x_start, y_start, x_end, y_end;
    x_start = center[0] - radius - boundary;

    x_end = y_end = (center[0] + radius + boundary);

    SetAt(center[0], center[1], 'C');

    for (; x_start < x_end; x_start++)
    {
        y_start = center[0] - radius - boundary;
        for (; y_start < y_end; y_start++)
        {
            int distance = Distance::CountDistance(center, {x_start, y_start, 0});
            if (distance == radius)
            {
                SetAt(x_start, y_start, '@');
            }
            if (distance < radius)
            {
                SetAt(x_start, y_start, '/');
            }
        }
    }
}

void Grid::DrawAirport(vector<int> position)
{
    SetAt(position[0], position[1], 'X');
}

Grid::~Grid()
{
}