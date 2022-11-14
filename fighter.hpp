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
    void WhenLookingForTarget();
    void WhenChasing();
    void WhenEvading();
    void WhenRetreating();

    void BackToBattlefield(Speed speed);

public:
    Fighter(std::string name, int battles, Team team, Simulation *simulation);
    virtual ~Fighter() = default;

    void Escort(const Bomber &bomber);
    void ChaseBomber(int target_id);
    void IterateAttacker(SimulationState sim_state);
    void IterateDefender(SimulationState sim_state);
};

#endif