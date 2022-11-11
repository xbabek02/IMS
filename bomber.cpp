#include "bomber.hpp"

Bomber::Bomber(std::string name, int battles, Simulation *simulation) : Plane(name, battles, Team::Attackers, simulation)
{
}

Bomber Bomber::Iterate(SimulationState state)
{
}

Bomber::~Bomber()
{
}