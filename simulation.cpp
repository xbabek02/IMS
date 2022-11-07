#include "simulation.hpp"

Simulation::Simulation(/* args */)
{
}

Simulation::~Simulation()
{
}

void Simulation::AddBomber(Bomber bomber)
{
    bombers.push_back(bomber);
}

void Simulation::AddDefender(Fighter fighter)
{
    defenders.push_back(fighter);
}

void Simulation::AddAttacker(Fighter fighter)
{
    attackers.push_back(fighter);
}

void Simulation::LogStatus()
{
    std::cerr << "Number of Bombers: " << bombers.size() << endl
              << "Number of Attackers: " << attackers.size() << endl
              << "Number of Defenders: " << defenders.size() << endl;
}