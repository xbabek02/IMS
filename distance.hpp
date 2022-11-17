#ifndef __DISTANCE_HPP__
#define __DISTANCE_HPP__

#include <vector>
#include <cmath>
#include "enums.hpp"

class Distance
{
public:
    static int CountDistance(std::vector<int> p1, std::vector<int> p2);
    static int CountDistance2D(std::vector<int> p1, std::vector<int> p2);
    static bool InRadiusFrom2D(std::vector<int> p1, std::vector<int> p2, int radius);
    static bool InRadiusFrom(std::vector<int> p1, std::vector<int> p2, int radius);
    static Directions GetBestDirection(const std::vector<int> &p1, const std::vector<int> &p2);
    static Directions AngleToDirection(double angle);
    static Directions BestPossibleFromCurrentDirection(Directions current, Directions desired);
    static std::vector<int> DirectionToVector(Directions direction);
    static std::vector<int> NewPointInDirection(Directions direction, std::vector<int> position, int times = 1);
    static int AngleOfTwoPoints(std::vector<int> p1, std::vector<int> p2);
};

#endif