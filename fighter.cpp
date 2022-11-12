#include "fighter.hpp"

Fighter::Fighter(std::string name, int battles, Team team, Simulation *simulation) : Plane(name, battles, team, simulation)
{
}

void Fighter::Iterate(SimulationState sim_state)
{
}

void Fighter::Escort(const Bomber &bomber)
{
    SetState(Escorting);
    SetTarget(bomber);
}

Fighter::~Fighter()
{
}