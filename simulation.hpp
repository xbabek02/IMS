#ifndef __SIMULATION_HPP__
#define __SIMULATION_HPP__

#include <vector>
#include <iostream>

#include "plane.hpp"
#include "bomber.hpp"
#include "fighter.hpp"
#include "grid.hpp"

class Simulation
{
private:
    std::vector<Bomber> bombers;
    std::vector<Fighter> attackers;
    std::vector<Fighter> defenders;

    int iteration = 0;

    std::vector<int> airport_pos;
    std::vector<int> target_pos;

    int target_radius;
    int battlefield_radius;

public:
    Grid ToGrid();
    Simulation();
    void AddBomber(Bomber Bomber);
    void AddAttacker(Fighter fighter);
    void AddDefender(Fighter fighter);
    void LogStatus();
    ~Simulation();
};

#endif