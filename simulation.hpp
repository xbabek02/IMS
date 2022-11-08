#ifndef __SIMULATION_HPP__
#define __SIMULATION_HPP__

#include <vector>
#include <iostream>

#include "plane.hpp"
#include "bomber.hpp"
#include "fighter.hpp"
#include "grid.hpp"

enum SimulationState
{
    PreStart,
    NotDetected,
    AirportTakeOf,
    Combat,
    TargetBombarded,
    End
};

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

    int sim_width;
    int sim_length;
    int sim_center;

    SimulationState state = PreStart;

    void InitAttackers();

public:
    Grid ToGrid();
    Simulation(int battlefield_radius);
    void AddBomber(const Bomber &bomber);
    void AddAttacker(const Fighter &fighter);
    void AddDefender(const Fighter &fighter);
    void LogStatus();
    void Run();
    ~Simulation();
};

#endif