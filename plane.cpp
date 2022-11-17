#include "plane.hpp"

Plane::Plane(std::string name, int battles, Team team, Simulation *simulation) : WithId(), pilotName(name), number_of_battles(battles), team(team), simulation(simulation)
{
}

bool Plane::GetActive() const
{
    if (state == PlaneState::Destroyed || state == PlaneState::Retreated || state == PlaneState::PreLauch)
    {
        return false;
    }
    return true;
}

Team Plane::GetTeam() const
{
    return team;
}

void Plane::SetPosition(std::vector<int> pos)
{
    position = pos;
}

std::vector<int> Plane::GetPosition() const
{
    return position;
}

void Plane::SetState(PlaneState state)
{
    this->state = state;
}

void Plane::SetTarget(const Plane &plane)
{
    target_id = plane.GetID();
}

void Plane::SetInsideZone()
{
    insideZone = true;
}

bool Plane::GetInsideZone()
{
    return insideZone;
}

void Plane::SetDirection(Directions direction)
{
    this->direction = direction;
}

void Plane::HeadTo(std::vector<int> pos)
{
    /*// will rise if the target is higher
    if ((pos.at(2) > position.at(2)) && !last_rised)
    {
        position.at(2)++;
        last_rised = true;
        return;
    }

    last_rised = false;
    */
    // must head forward if it turned last iteration
    if (last_turned)
    {
        auto vec = Distance::DirectionToVector(direction);
        position.at(0) += vec.at(0);
        position.at(1) += vec.at(1);
        last_turned = false;
        return;
    }

    // turning, counting the best direction to choose
    Directions best_direction = Distance::GetBestDirection(position, pos);
    best_direction = Distance::BestPossibleFromCurrentDirection(direction, best_direction);

    if (state != Retreating && IsBoundaryInDirection(best_direction) && insideZone)
    {
        best_direction = DirectionEvadeBoundary();
    }

    auto vec = Distance::DirectionToVector(best_direction);
    position.at(0) += vec.at(0);
    position.at(1) += vec.at(1);

    if (best_direction != direction)
    {
        last_turned = true;
    }

    fuel--;
    direction = best_direction;
}

bool Plane::IsBoundaryInDirection(Directions d)
{
    if (!Distance::InRadiusFrom2D(Distance::NewPointInDirection(d, position, 5),
                                  simulation->GetCenter(), simulation->GetBattlefieldRadius()))
    {
        return true;
    }
    return false;
}

Directions Plane::DirectionEvadeBoundary()
{
    std::vector<Directions> directions({static_cast<Directions>(((direction - 1) + 8) % 8),
                                        static_cast<Directions>(((direction + 1) + 8) % 8),
                                        direction});

    int max = INT_MIN;
    Directions best;
    int battlefield_radius = simulation->GetBattlefieldRadius();
    auto battlefield_center = simulation->GetCenter();
    int steps = 0;

    for (int i = 0; i < 3; i++)
    {
        // iterate until hitting the boundary
        for (steps = 0;
             Distance::InRadiusFrom2D(Distance::NewPointInDirection(directions[i], position, steps),
                                      battlefield_center, battlefield_radius);
             steps++)
            ;
        if (steps > max)
        {
            best = directions[i];
            max = steps;
        }
    }
    return best;
}

int Plane::IsAnyEnemyFighterDangerouslyBehind()
{
    auto enemies = simulation->GetAllEnemyFighters(*this);
    for (auto &enemy : enemies)
    {
        if (IsDangerouslyBehind(enemy))
        {
            return enemy.GetID();
        }
    }
    return -1;
}

bool Plane::IsDangerouslyBehind(const Plane &plane, int distance) const
{
    if (plane.GetTargetId() != GetID() || plane.GetState() != Chasing)
    {
        return false;
    }

    if (Distance::CountDistance2D(position, plane.GetPosition()) > distance)
    {
        return false;
    }

    Directions left = static_cast<Directions>(((plane.GetDirection() - 1) + 8) % 8);
    Directions right = static_cast<Directions>(((plane.GetDirection() + 1) + 8) % 8);

    if (left == direction || right == direction || plane.GetDirection() == direction)
    {
        return true;
    }
    return false;
}

void Plane::Destroy(int byId)
{
    auto &attacker = simulation->GetById(byId);
    SetState(Destroyed);
    attacker.IncrementTakedowns();
}

void Plane::IncrementTakedowns()
{
    takedowns++;
}

std::vector<int> Plane::GetPosAhead(int steps)
{
    auto vec = Distance::DirectionToVector(direction);
    std::vector<int> new_pos(position);
    new_pos[0] += vec[0] * steps;
    new_pos[1] += vec[1] * steps;

    return new_pos;
}

std::vector<int> Plane::PointSameHigh(std::vector<int> point)
{
    return std::vector<int>({point[0], point[1], position[2]});
}

Plane &Plane::GetTarget()
{
    return simulation->GetById(target_id);
}

Directions Plane::GetDirection() const
{
    return direction;
}

bool Plane::IsTargetActive()
{
    for (auto plane : simulation->GetNotActive())
    {
        if (plane.GetID() == target_id)
        {
            return false;
        }
    }
    return true;
}

int Plane::GetTargetId() const
{
    return target_id;
}

PlaneState Plane::GetState() const
{
    return state;
}

int Plane::GetExperience() const
{
    return number_of_battles;
}
