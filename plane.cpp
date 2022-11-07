#include "plane.hpp"

Plane::Plane(std::string name, int battles, Team team) : WithId(), pilotName(name), number_of_battles(battles), team(team)
{
}

Plane::~Plane()
{
}
