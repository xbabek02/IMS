#ifndef __FIGHTER_HPP__
#define __FIGHTER_HPP__

#include "plane.hpp"

class Fighter : public Plane
{
private:
    int ammo_cnt;

public:
    Fighter(std::string name, int battles, Team team, Simulation *simulation);
    ~Fighter();

    Fighter Iterate(SimulationState state);
};

#endif