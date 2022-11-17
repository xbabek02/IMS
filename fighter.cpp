#include "fighter.hpp"

Fighter::Fighter(std::string name, int battles, Team team, Simulation *simulation) : Plane(name, battles, team, simulation)
{
    if (team == Attackers)
    {
        insideZone = false;
    }
    else
    {
        insideZone = true;
    }
}

bool Fighter::IterateCommon()
{
    // Dealing with battlefield boundary
    if (insideZone && !simulation->InsideBoundary(*this))
    {
        switch (state)
        {
        case Retreating:
            state = Retreated;
            return false;

        default:
            BackToBattlefield(state == Scouting ? Slow : Fast);
            return false;
        }
    }
    else
    {
        if (simulation->InsideBoundary(*this))
        {
            insideZone = true;
        }
    }

    // Setting evading state if in danger
    int enemyId = IsAnyEnemyFighterDangerouslyBehind();
    if (enemyId != -1)
    {
        Evade(enemyId);
    }

    return true;
}

void Fighter::IterateAttacker(SimulationState sim_state)
{
    if (!IterateCommon())
    {
        return;
    }

    if (target_id != -1 && !IsTargetActive())
    {
        target_id = -1;
        state = FlyingToTarget;
    }

    switch (state)
    {
    case Escorting:
        WhenEscorting();
        break;

    case FlyingToTarget:
        WhenFlyingToTarget();
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
    if (!IterateCommon())
    {
        return;
    }

    if (target_id != -1 && !IsTargetActive())
    {
        target_id = -1;
        state = LookingForTarget;
    }

    switch (state)
    {
    case Scouting:
        WhenScouting();
        break;

    // find the target ==> then chase it
    case LookingForTarget:
        WhenLookingForTarget();
        break;
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

void Fighter::LookForDefenders()
{
    if (state == Escorting)
    {
        auto bomber = simulation->GetById(target_id);
        for (auto &defender : simulation->GetAllEnemyFighters(*this))
        {
            if (Distance::InRadiusFrom(bomber.GetPosition(), defender.GetPosition(), 50))
            {
                if (defender.GetTargetId() == bomber.GetID() && defender.GetState() == Chasing)
                {
                    ChaseFighter(defender.GetID());
                }
            }
        }
    }
    else if (state == FlyingToTarget)
    {
        for (auto &defender : simulation->GetAllEnemyFighters(*this))
        {
            if (Distance::InRadiusFrom(GetPosition(), defender.GetPosition(), 50))
            {
                ChaseFighter(defender.GetID());
            }
        }
    }
}

void Fighter::WhenEscorting()
{
    auto bomber_position = GetTarget().GetPosition();
    std::vector<int> new_position(bomber_position);
    int steps = 0;

    LookForDefenders();

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
    ChaseBomber(simulation->GetClosestUnattackedBomber(*this));
    WhenChasing();
}

void Fighter::WhenChasing()
{
    Plane &plane = simulation->GetById(target_id);

    if (team == Defenders && dynamic_cast<Bomber *>(&plane) != nullptr)
    {
        int attackerId = simulation->GetAttackerId(*this);
        if (attackerId != -1)
        {
            ChaseFighter(attackerId);
            WhenChasing();
            return;
        }
    }

    std::vector<int> future_pos;

    // chance to pick random direction if you are less experienced than your opponent
    if (rnd::ExperienceClash(GetExperience(), plane.GetExperience()) == 2)
    {
        Directions dir;
        if ((rand() % 2) == 1)
            dir = static_cast<Directions>(((plane.GetDirection() - 1) + 8) % 8);

        else
            dir = static_cast<Directions>(((plane.GetDirection() + 1) + 8) % 8);

        future_pos = Distance::NewPointInDirection(dir, plane.GetPosition(), 6);
    }

    else
    {
        future_pos = plane.GetPosAhead(3);
    }

    for (int i = 0; i < max_steps; i++)
    {
        HeadTo(future_pos);
    }
}

void Fighter::WhenFlyingToTarget()
{
    LookForDefenders();
    HeadTo(simulation->GetCenter());
}

void Fighter::WhenEvading()
{
    auto target = GetTarget();

    int distance = Distance::CountDistance2D(target.GetPosition(), position);

    // escaped or got behind
    if (target.IsDangerouslyBehind(*this) || distance > 10)
    {
        ChaseFighter(target.GetID());
        WhenChasing();
        return;
    }

    Directions left = static_cast<Directions>(((direction - 1) + 8) % 8);
    Directions right = static_cast<Directions>(((direction + 1) + 8) % 8);

    std::vector<std::vector<int>> possibilities;

    possibilities.push_back(Distance::NewPointInDirection(direction, position, 1));
    possibilities.push_back(Distance::NewPointInDirection(left, position, 1));
    possibilities.push_back(Distance::NewPointInDirection(right, position, 1));

    for (int i = 0; i < max_steps; i++)
    {
        if (last_turned)
        {
            HeadTo(possibilities[0]);
        }
        else
        {
            std::vector<int> *best;
            // std::vector<int> *second_best;
            int distance;
            int distance_max = INT_MIN;
            for (int i = 0; i < 3; i++)
            {
                distance = Distance::CountDistance2D(possibilities[i], target.GetPosition());
                if (distance > distance_max)
                {
                    distance_max = distance;
                    // second_best = best;
                    best = &(possibilities[i]);
                }
            }
            HeadTo(*best);
        }
    }
}
void Fighter::WhenRetreating()
{
    int steps = fuel == 0 ? 2 : 3;

    for (int i = 0; i < steps; i++)
        HeadTo(PointSameHigh(simulation->GetCenter()));
}

void Fighter::BackToBattlefield(Speed speed)
{
    int moves = (speed == Slow ? 2 : 3);
    for (int i = 0; i < moves; i++)
        HeadTo(PointSameHigh(simulation->GetCenter()));
}

void Fighter::SwapSkillCheck()
{
    auto &target = GetTarget();
    int distance = Distance::CountDistance2D(target.GetPosition(), position);

    // chance to get behind attacker
    if (rnd::range(1, 5) == 1 && distance < 6)
    {
        if (rnd::ExperienceClash(number_of_battles, target.GetExperience() == 1))
        {
            std::vector<int> aux(position);
            SetPosition(target.GetPosition());
            target.SetPosition(aux);

            SetState(Chasing);
            target.SetState(Evading);
        }
    }
}

void Fighter::Escort()
{
    target_id = assignedEscort;
    if (!IsTargetActive())
    {
        SetState(Escorting);
    }
    else
    {
        SetState(FlyingToTarget);
    }
}

void Fighter::Escort(int target_id)
{
    SetState(Escorting);
    this->target_id = target_id;
    assignedEscort = target_id;
}

void Fighter::ChaseBomber(int target_id)
{
    SetState(Chasing);
    simulation->UpdateBomberChaser(target_id, GetID());
    this->target_id = target_id;
}

void Fighter::ChaseFighter(int target_id)
{
    SetState(Chasing);
    this->target_id = target_id;
}

void Fighter::Evade(int target_id)
{
    SetState(Evading);
    this->target_id = target_id;
}