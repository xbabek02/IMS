#ifndef __PLANE_HPP__
#define __PLANE_HPP__

#include <vector>
#include <string>

#include "withid.hpp"
#include "enums.hpp"
#include "distance.hpp"

class Simulation;

class Plane : public WithId
{
protected:
    std::vector<int> sight;
    int fuel;

    Directions direction;
    PlaneState state = PlaneState::PreLauch;

    std::string pilotName;
    int number_of_battles;
    Team team;

    int takedowns = 0;

    std::vector<int> position;

    Simulation *simulation;

    int target_id;            // for state events (escorted plane, chased plane ...)
    bool last_turned = false; // has to go forward the next
    bool last_rised = false;  // can't rise in the next iteration

    std::vector<int> PointSameHigh(std::vector<int>);

public:
    Plane(std::string name, int battles, Team team, Simulation *simulation);
    std::vector<int> GetPosition() const;
    bool GetActive() const;
    Team GetTeam() const;
    void SetPosition(std::vector<int> pos);
    void SetState(PlaneState state);
    void SetTarget(const Plane &plane);
    void SetDirection(Directions direction);
    Plane &GetTarget();
    void HeadTo(std::vector<int> pos);

    Plane Iterate(SimulationState sim_state);

    ~Plane();
};

#include "simulation.hpp"

#endif