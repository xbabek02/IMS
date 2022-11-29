#ifndef __SIMULATION_HPP__
#define __SIMULATION_HPP__

#include <vector>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <unistd.h>
#include <map>
#include <climits>

#include "fighter.hpp"
#include "bomber.hpp"
#include "plane.hpp"

#include "grid.hpp"
#include "distance.hpp"
#include "enums.hpp"
#include "random.hpp"

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

    std::vector<int> target_pos;

    int target_radius;
    int battlefield_radius;
    std::vector<int> center;

    // for keeping track of which bombers are free to take on
    std::map<int, std::vector<int>> targetedBombers;
    std::vector<tuple<int, int>> toBeDestroyed;

    int sim_width;
    int sim_length;

    int bombs_dropped = 0;
    int bombs_goal = 150;

    SimulationState state = SimulationState::PreStart;

    void InitAttackers();
    void InitDefenders();
    void InitTargetingMap();
    void DefendersDefend();
    bool AnyAttackerInsideBoundary();
    void ExperienceSwapChase();
    void Iterate();
    void UpdateBomberTargeting();
    std::vector<int> RandomBattlefieldPoint();
    void DeactivatePlanes();

public:
    Simulation(int battlefield_radius);

    void AddBomber(const Bomber &bomber);
    void AddAttacker(const Fighter &fighter);
    void AddDefender(const Fighter &fighter);
    void UpdateBomberChaser(int bomberId, int chaserId);
    std::vector<Bomber> &GetAllBombers();
    std::vector<Fighter> &GetAllEnemyFighters(const Plane &plane);
    std::vector<Plane> GetNotActive();
    Plane &GetById(int id);
    bool PlaneActiveById(int id);
    int GetIteration() const;
    const std::vector<int> &GetCenter() const;
    std::vector<int> GetTarget();
    float ClashFunc(const Plane &chaser, const Plane &chasee);
    float ClashFuncBomber(const Plane &chaser, const Bomber &bomber);
    int GetTargetRadius() const;
    int GetClosestUnattackedBomber(const Plane &plane, bool orClosest = true);
    int GetClosestBomber(const Plane &plane);
    int GetAttackerId(const Plane &plane);
    int GetBattlefieldRadius() const;
    void AddEnemyToDestruction(int attacker, int toDestroy);

    void DropBomb();

    bool InsideBoundary(const Plane &plane) const;
    Grid ToGrid();

    void LogStatus();
    void Run(int speed);

    ~Simulation();
};

#endif