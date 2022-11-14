#ifndef __FIGHTER_HPP__
#define __FIGHTER_HPP__

#include "bomber.hpp"
#include "plane.hpp"

class Simulation;
class Bomber;
class Plane;

class Fighter : public Plane
{
private:
    int ammo_cnt;
    int max_steps = 3;

    void WhenScouting();
    void WhenEscorting();
    void BackToBattlefield(Speed speed);

public:
    Fighter(std::string name, int battles, Team team, Simulation *simulation);
    ~Fighter();

    void Escort(const Bomber &bomber);
    void IterateAttacker(SimulationState sim_state);
    void IterateDefender(SimulationState sim_state);
};

#endif