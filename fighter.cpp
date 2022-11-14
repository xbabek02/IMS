#include "fighter.hpp"

Fighter::Fighter(std::string name, int battles, Team team, Simulation *simulation) : Plane(name, battles, team, simulation)
{
}

void Fighter::IterateAttacker(SimulationState sim_state)
{
    if (!simulation->InsideBoundary(*this))
    {
        switch (state)
        {
        case Retreating:
            state = Retreated;
            return;

        default:
            BackToBattlefield(state == Scouting ? Slow : Fast);
            return;
        }
    }

    switch (state)
    {
    case Escorting:
        WhenEscorting();
        break;

    case Evading:
        WhenEvading();
        break;

    case Chasing:
        WhenChasing();
        break;

    case Retreating:
        WhenRetreating();
        break;

    default:
        break;
    }
}

void Fighter::IterateDefender(SimulationState sim_state)
{
    if (!simulation->InsideBoundary(*this))
    {
        switch (state)
        {
        case Retreating:
            state = Retreated;
            return;

        default:
            BackToBattlefield(state == Scouting ? Slow : Fast);
            return;
        }
    }
    switch (state)
    {
    case Scouting:
        WhenScouting();
        break;

    // find the target ==> then chase it
    case LookingForTarget:
        WhenLookingForTarget();
    case Chasing:
        WhenChasing();
        break;

    case Evading:
        WhenEvading();
        break;

    case Retreating:
        WhenRetreating();
        break;

    default:
        break;
    }
}

void Fighter::WhenEscorting()
{
    auto bomber_position = GetTarget().GetPosition();
    std::vector<int> new_position(bomber_position);
    int steps = 0;
    do
    {
        // keeping the z coordinate and wanting to fly next to it
        if (bomber_position[0] < position[0])
        {
            new_position[0] += 2;
        }
        else
        {
            new_position[0] -= 2;
        }
        new_position[2] = position[2];

        HeadTo(new_position);

        steps++;

        if (Distance::InRadiusFrom(position, bomber_position, 4))
            break;
    } while (steps < max_steps + 1);
}

void Fighter::WhenScouting()
{
    HeadTo(Distance::NewPointInDirection(direction, position));
}

void Fighter::WhenLookingForTarget()
{
    target_id = simulation->GetClosestUnattackedBomber(*this);
    ChaseBomber(target_id);
}

void Fighter::WhenChasing()
{
    Plane &plane = simulation->GetById(target_id);
    std::vector<int> future_pos = plane.GetPosAhead(1);

    for (int i = 0; i < max_steps; i++)
    {
        HeadTo(future_pos);
    }
}

void Fighter::WhenEvading()
{
}

void Fighter::WhenRetreating()
{
}

void Fighter::BackToBattlefield(Speed speed)
{
    int moves = (speed == Slow ? 2 : 3);
    for (int i = 0; i < moves; i++)
        HeadTo(PointSameHigh(simulation->GetCenter()));
}

void Fighter::Escort(const Bomber &bomber)
{
    SetState(Escorting);
    SetTarget(bomber);
}

void Fighter::ChaseBomber(int target_id)
{
    SetState(Chasing);
    simulation->UpdateBomberChaser(target_id, GetID());
}