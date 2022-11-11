#include "fighter.hpp"

Fighter::Fighter(std::string name, int battles, Team team, Simulation *simulation) : Plane(name, battles, team, simulation)
{
}

Fighter Fighter::Iterate(SimulationState state)
{
}

Fighter::~Fighter()
{
}