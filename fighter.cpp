#include "fighter.hpp"

Fighter::Fighter(std::string name, int battles, Team team, Simulation *simulation) : Plane(name, battles, team, simulation)
{
}

Fighter Fighter::Iterate(SimulationState state)
{
}

void Fighter::Escort(Bomber *bomber)
{
    SetState(Escorting);
    SetTarget(bomber);
}

Fighter::~Fighter()
{
}