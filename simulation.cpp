#include "simulation.hpp"

Simulation::Simulation(int battlefield_radius) : battlefield_radius(battlefield_radius)
{
    sim_length = sim_width = battlefield_radius * 2 + 100;
    target_pos.assign({sim_length / 2, sim_width / 2, 0});
    target_radius = battlefield_radius * 2 / 15;
}

Simulation::~Simulation()
{
}

void Simulation::AddBomber(const Bomber &bomber)
{
    bombers.push_back(bomber);
}

void Simulation::AddDefender(const Fighter &fighter)
{
    defenders.push_back(fighter);
}

void Simulation::AddAttacker(const Fighter &fighter)
{
    attackers.push_back(fighter);
}

void Simulation::LogStatus()
{
    std::cerr << "Number of Bombers: " << bombers.size() << endl
              << "Number of Attackers: " << attackers.size() << endl
              << "Number of Defenders: " << defenders.size() << endl;
}

void Simulation::Run()
{
    /*
    while (true)
    {
        switch (state)
        {
        case PreStart:
            InitAttackers();
            state = NotDetected;
            break;

        default:
            break;
        }
    }
    */
    auto grid = ToGrid();
    grid.Display();
}

void Simulation::InitAttackers()
{
}

Grid Simulation::ToGrid()
{
    Grid grid(sim_length, sim_width);
    grid.DrawBattlefield(battlefield_radius, {sim_length / 2, sim_width / 2, 0});
    grid.DrawTargetZone(target_radius, target_pos);
    return grid;
}