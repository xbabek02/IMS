#include "bomber.hpp"

Bomber::Bomber(std::string name, int battles, Simulation *simulation) : Plane(name, battles, Team::Attackers, simulation)
{
}

void Bomber::Iterate(SimulationState sim_state)
{

    for (int i = 0; i < max_steps; i++)
    {
        HeadTo(simulation->GetTarget());
    }

    if (!last_dropped)
    {
        if (Distance::InRadiusFrom2D(simulation->GetTarget(), position, simulation->GetTargetRadius()))
            simulation->DropBomb();
    }
    else
        last_dropped = false;

    int fighterId = Plane::IsAnyEnemyFighterDangerouslyBehind();
    if (fighterId != -1)
    {
        // SHOOTING
        auto fighter = simulation->GetById(fighterId);
        float clash = simulation->ClashFuncBomber(fighter, *this);

        if (ShouldTryShooting(clash))
        {
            Shoot(clash, fighterId);
        }
    }
}

bool Bomber::ShouldTryShooting(float clash)
{
    if (number_of_battles > 50)
    {
        if (clash > 0.40)
            return true;
        else
            return false;
    }
    else
    {
        if (clash > 0.3)
            return true;
        else
            return false;
    }
}

void Bomber::Shoot(float clash, int whoId)
{
    int clash_int = round(clash * 100);
    if (rnd::range(0, 100) < clash_int)
    {
        simulation->AddEnemyToDestruction(GetID(), whoId);
    }
}