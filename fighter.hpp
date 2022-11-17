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
    int ammo_cnt = 1840;
    int rounds_per_second = 75;
    int max_steps = 3;
    int assignedEscort = -1;

    void LookForDefenders();

    void WhenScouting();
    void WhenEscorting();
    void WhenLookingForTarget();
    void WhenFlyingToTarget();
    void WhenChasing();
    void WhenEvading();
    void WhenRetreating();

    void BackToBattlefield(Speed speed);

    bool ShouldTryShooting(float clash);
    void Shoot(float clash);

public:
    Fighter(std::string name, int battles, Team team, Simulation *simulation);
    virtual ~Fighter() = default;

    void Escort();
    void Escort(int target_id);
    void ChaseBomber(int target_id);
    void ChaseFighter(int target_id);
    void Evade(int target_id);
    bool IterateCommon();
    void IterateAttacker(SimulationState sim_state);
    void IterateDefender(SimulationState sim_state);
    void SwapSkillCheck();
};

#endif