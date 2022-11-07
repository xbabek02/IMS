#ifndef __PLANE_HPP__
#define __PLANE_HPP__

#include <vector>
#include <string>
#include "withid.hpp"

enum Team
{
    Attackers,
    Defenders
};

enum PlaneState
{
    Scouting,
    Evading,
    Chasing,
    Retreating,
    OutOfFight
};

enum Directions
{
    Up,
    UpLeft,
    Left,
    DownLeft,
    Down,
    DownRight,
    Right,
    UpRight,
};

class Plane : public WithId
{
protected:
    std::vector<int> sight;
    int fuel;

    Directions direction;
    PlaneState state;
    Team team;

    std::string pilotName;
    int number_of_battles;
    int takedowns = 0;

    std::vector<int> position;

public:
    Plane(std::string name, int battles, Team team);
    ~Plane();
};

#endif