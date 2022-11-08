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
    Escorting,
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
public:
    std::vector<int> sight;
    int fuel;

    Directions direction;
    bool turned_last_iteration; // has to go forward the next
    PlaneState state;

    std::string pilotName;
    int number_of_battles;
    Team team;

    int takedowns = 0;

    std::vector<int> position;

public:
    Plane(std::string name, int battles, Team team);
    ~Plane();
};

#endif