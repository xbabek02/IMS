#include "distance.hpp"

int Distance::CountDistance(std::vector<int> p1, std::vector<int> p2)
{
    return round(sqrt(pow(p1[0] - p2[0], 2) + pow(p1[1] - p2[1], 2) + pow(p1[2] - p2[2], 2)));
}

bool Distance::InRadiusFrom(std::vector<int> p1, std::vector<int> p2, int radius)
{
    return CountDistance(p1, p2) <= radius;
}