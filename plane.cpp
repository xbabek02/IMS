#include "plane.hpp"

Plane::Plane(std::string name, int battles, Team team, Simulation *simulation) : WithId(), pilotName(name), number_of_battles(battles), team(team), simulation(simulation)
{
}

bool Plane::GetActive() const
{
    if (state == PlaneState::Destroyed || state == PlaneState::Retreated)
    {
        return false;
    }
    return false;
}

Team Plane::GetTeam() const
{
    return team;
}

std::vector<int> Plane::GetPosition() const
{
    return position;
}

Plane::~Plane()
{
}
