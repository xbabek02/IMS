#ifndef __SIMULATION_HPP__
#define __SIMULATION_HPP__

#include <vector>
#include <iostream>

#include "fighter.hpp"
#include "bomber.hpp"
#include "plane.hpp"

#include "grid.hpp"
#include "distance.hpp"
#include "enums.hpp"
#include <unistd.h>

class Plane;
class Bomber;
class Fighter;

class Simulation
{
private:
    std::vector<Bomber> bombers;
    std::vector<Fighter> attackers;
    std::vector<Fighter> defenders;

    std::vector<Plane> not_active;

    int iteration = 0;

    std::vector<int> airport_pos;
    std::vector<int> target_pos;

    int target_radius;
    int battlefield_radius;
    std::vector<int> center;

    int sim_width;
    int sim_length;

    int bombs_dropped = 0;
    int bombs_goal = 100;

    SimulationState state = SimulationState::PreStart;

    void InitAttackers();
    bool InsideBoundary(const Plane &plane) const;
    bool AnyAttackerInsideBoundary();
    void Iterate();

public:
    Grid ToGrid();
    Simulation(int battlefield_radius);
    void AddBomber(const Bomber &bomber);
    void AddAttacker(const Fighter &fighter);
    void AddDefender(const Fighter &fighter);
    void LogStatus();
    void Run();
    std::vector<Fighter> &ReturnAllEnemyFighters(const Plane &plane);

    std::vector<int> GetTarget();

    ~Simulation();
};

#endif