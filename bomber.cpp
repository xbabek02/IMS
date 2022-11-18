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

    /*int fighter = Plane::IsAnyEnemyFighterDangerouslyBehind();
    if(fighter != -1){
        // SHOOTING
        Plane &plane = simulation->GetById(target_id);
        float clash = simulation->ClashFuncBomber(plane, *this);
        
        if (ShouldTryShooting(clash))
        {
            Shoot(clash);
        }
    }*/
}

bool Bomber::ShouldTryShooting(float clash)
{
    if (number_of_battles > 50)
    {
        if (clash > 0.6)
            return true;
        else
            return false;
    }
    else
    {
        if (clash > 0.4)
            return true;
        else
            return false;
    }
}

void Bomber::Shoot(float clash)
{
    int clash_int = round(clash * 100);
    if (rnd::range(0, 100) < clash_int)
    {
        simulation->AddEnemyToDestruction(GetID(), target_id);
    }
}