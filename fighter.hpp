#ifndef __FIGHTER_HPP__
#define __FIGHTER_HPP__

#include "plane.hpp"
#include "bomber.hpp"

class Fighter : public Plane
{
private:
    int ammo_cnt;

public:
    Fighter(std::string name, int battles, Team team, Simulation *simulation);
    ~Fighter();

    void Escort(Bomber *bomber);
    Fighter Iterate(SimulationState state);
};

#endif