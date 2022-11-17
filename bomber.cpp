#include "bomber.hpp"

Bomber::Bomber(std::string name, int battles, Simulation *simulation) : Plane(name, battles, Team::Attackers, simulation)
{
}

void Bomber::Iterate(SimulationState sim_state)
{
    if (state == FlyingToTarget)
    {
        for (int i = 0; i < max_steps; i++)
        {
            HeadTo(simulation->GetTarget());
        }
    }
    if (!last_dropped)
    {
        if (Distance::InRadiusFrom2D(simulation->GetTarget(), position, simulation->GetTargetRadius()))
            simulation->DropBomb();
    }
    else
        last_dropped = false;
}