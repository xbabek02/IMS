#include "bomber.hpp"

Bomber::Bomber(std::string name, int battles) : Plane(name, battles, Team::Attackers)
{
}

Bomber::~Bomber()
{
}