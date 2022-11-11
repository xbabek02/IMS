#ifndef __PLANE_HPP__
#define __PLANE_HPP__

#include <vector>
#include <string>

#include "withid.hpp"
#include "enums.hpp"

class Simulation;

class Plane : public WithId
{
protected:
    std::vector<int> sight;
    int fuel;

    Directions direction;
    bool turned_last_iteration; // has to go forward the next
    PlaneState state = PlaneState::PreLauch;

    std::string pilotName;
    int number_of_battles;
    Team team;

    int takedowns = 0;

    std::vector<int> position;

    Simulation *simulation;

    Plane *target; // for state events (escorted plane, chased plane ...)

public:
    Plane(std::string name, int battles, Team team, Simulation *simulation);
    std::vector<int> GetPosition() const;
    bool GetActive() const;
    Team GetTeam() const;
    void SetPosition(std::vector<int> pos);
    void SetState(PlaneState state);
    void SetTarget(Plane *plane);

    Plane Iterate(SimulationState state);

    ~Plane();
};

#endif