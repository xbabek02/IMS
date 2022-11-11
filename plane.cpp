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

void Plane::SetTarget(Plane *plane)
{
    target = plane;
}

Plane::~Plane()
{
}
