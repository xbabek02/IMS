#ifndef __DISTANCE_HPP__
#define __DISTANCE_HPP__

#include <vector>
#include <cmath>

class Distance
{
public:
    static int CountDistance(std::vector<int> p1, std::vector<int> p2);
    static bool InRadiusFrom(std::vector<int> p1, std::vector<int> p2, int radius);
};

#endif