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

    fuel = 200;
}

bool Fighter::IterateCommon()
{
    if (ammo_cnt == 0 || fuel == 0)
    {
        state = Retreating;
    }

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
    if (enemyId != -1 && state != Retreating)
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
        Escort();
    }

    switch (state)
    {
        /*
         * Stay close to your assigned escort, attack possible threat to it
         */
    case Escorting:
        WhenEscorting();
        break;

        /*
         * Fly to the center of target and attack anyone you see close
         */
    case FlyingToTarget:
        WhenFlyingToTarget();
        break;

        /*
         * Try to find most distant point from the attacker chasing you
         * with a chance of getting behind him in the process
         */
    case Evading:
        WhenEvading();
        break;

        /*
         * Stick close behind enemy and try to shoot it down
         * when its most probable to hit
         */
    case Chasing:
        WhenChasing();
        break;

        /*
         * Get out of the zone and retreat from fight if you are out of
         * ammonition or fuel
         */
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
        /*
         * Fly in medium speed to random point in the battlefield
         */
    case Scouting:
        WhenScouting();
        break;

        /*
         *  Attack unnatacked or closest bomber
         */
    case LookingForTarget:
        WhenLookingForTarget();
        break;

        /*
         * Try to find most distant point from the attacker chasing you
         * with a chance of getting behind him in the process
         */
    case Evading:
        WhenEvading();
        break;

        /*
         * Stick close behind enemy and try to shoot it down
         * when its most probable to hit
         */
    case Chasing:
        WhenChasing();
        break;

        /*
         * Get out of the zone and retreat from fight if you are out of
         * ammonition or fuel
         */
    case Retreating:
        WhenRetreating();
        break;

    default:
        break;
    }
}

void Fighter::LookForDefenders()
{
    // attack any defender that gets close to your bomber
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
    // attack any defender that gets close to you
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
    // if bomber was destroyed or retreated
    if (!IsTargetActive())
    {
        target_id = -1;
        SetState(FlyingToTarget);
        WhenFlyingToTarget();
        return;
    }

    auto bomber_position = GetTarget().GetPosition();
    std::vector<int> new_position(bomber_position);
    int steps = 0;

    LookForDefenders();

    do
    {
        // try to fly next to the bomber
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
    if (simulation->GetAllBombers().size() == 0)
    {
        SetState(Scouting);
        WhenScouting();
        return;
    }

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

    // SHOOTING
    float clash = simulation->ClashFunc(*this, plane);
    // cout << clash << endl;
    if (ShouldTryShooting(clash))
    {
        Shoot(clash);
    }

    // MOVING
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
    if (!IsTargetActive())
    {
        if (team == Attackers)
        {
            target_id = -1;
            SetState(FlyingToTarget);
            WhenFlyingToTarget();
            return;
        }
        else
        {
            target_id = -1;
            SetState(LookingForTarget);
            WhenLookingForTarget();
            return;
        }
    }
    auto target = GetTarget();

    int distance = Distance::CountDistance2D(target.GetPosition(), position);

    // escaped or got behind
    if (target.IsDangerouslyBehind(*this) || distance > 10)
    {
        ChaseFighter(target.GetID());
        WhenChasing();
        return;
    }

    // count the most distant position to move form attacker

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
            int distance;
            int distance_max = INT_MIN;
            for (int i = 0; i < 3; i++)
            {
                distance = Distance::CountDistance2D(possibilities[i], target.GetPosAhead(1));
                if (distance > distance_max)
                {
                    distance_max = distance;
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

    auto center = simulation->GetCenter();

    std::vector<int> new_point(
        Distance::NewPointInDirection(Distance::GetBestDirection(center, position), position, 5));

    for (int i = 0; i < steps; i++)
        HeadTo(PointSameHigh(new_point));
}

void Fighter::BackToBattlefield(Speed speed)
{
    int moves = (speed == Slow ? 2 : 3);
    for (int i = 0; i < moves; i++)
        HeadTo(PointSameHigh(simulation->GetCenter()));
}

void Fighter::SwapSkillCheck()
{
    if (!IsTargetActive())
    {
        return;
    }

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

bool Fighter::ShouldTryShooting(float clash)
{
    if (number_of_battles > 50)
    {
        if (clash > 0.4)
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

void Fighter::Shoot(float clash)
{
    int clash_int = round(clash * 100);
    if (rnd::range(0, 100) < clash_int)
    {
        simulation->AddEnemyToDestruction(GetID(), target_id);
    }

    // how many rounds of ammonition were wasted
    if (number_of_battles > 50)
    {
        ammo_cnt -= rounds_per_second;
    }
    else
    {
        if (rnd::range(0, 1))
        {
            ammo_cnt -= rounds_per_second * 2;
        }
        else
        {
            ammo_cnt -= rounds_per_second;
        }
    }
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