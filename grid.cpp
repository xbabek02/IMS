#include "grid.hpp"

Grid::Grid(int size) : size(size)
{
    for (int y = 0; y < size; y++)
    {
        for (int x = 0; x < size; x++)
        {
            field.push_back(' ');
        }
    }
}

int Grid::GetSize() const
{
    return size;
}

void Grid::Display()
{
    for (int y = 0; y < size; y++)
    {
        for (int x = 0; x < size; x++)
        {
            cout << GetAt(x, y) << "  ";
        }
        cout << endl;
    }
}

inline char Grid::GetAt(int x, int y)
{
    return field.at(y * size + x);
}

inline void Grid::SetAt(int x, int y, char c)
{
    if (x > size - 1 || y > size - 1)
    {
        throw;
    }
    field.at(y * size + x) = c;
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

            int distance = Distance::CountDistance2D(center, {x_start, y_start, 0});
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
            int distance = Distance::CountDistance2D(center, {x_start, y_start, 0});
            if (distance == radius)
            {
                SetAt(x_start, y_start, '@');
            }
            if (distance < radius)
            {
                SetAt(x_start, y_start, ' ');
            }
        }
    }
}

Grid::~Grid()
{
}