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

void Plane::SetDirection(Directions direction)
{
    this->direction = direction;
}

void Plane::HeadTo(std::vector<int> pos)
{
    // will rise if the target is higher
    if ((pos.at(2) > position.at(2)) && !last_rised)
    {
        position.at(2)++;
        last_rised = true;
        return;
    }

    last_rised = false;

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

    auto vec = Distance::DirectionToVector(best_direction);
    position.at(0) += vec.at(0);
    position.at(1) += vec.at(1);
    direction = best_direction;

    if (best_direction != direction)
    {
        last_turned = true;
    }
}

std::vector<int> Plane::PointSameHigh(std::vector<int> point)
{
    return std::vector<int>({point[0], point[1], position[2]});
}

Plane &Plane::GetTarget()
{
    return simulation->GetById(target_id);
}

Plane::~Plane()
{
}
