#include "simulation.hpp"

Simulation::Simulation(int battlefield_radius) : battlefield_radius(battlefield_radius)
{
    center = {sim_length / 2, sim_width / 2, 0};
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

//TODO
//maximum podle ktereho se bude porovnavat muze byt 10 nebo 5
float ClashFunc(Plane plane1, Plane plane2){
    std::vector<int> pos1 = plane1.GetPosition();
    std::vector<int> pos2 = plane2.GetPosition();

    //TODO
    int exp = 110;//max(plane1.number_of_battles, plane2.number_of_battles);

    float direction = (pos1[0] * pos2[0]) + (pos1[1] * pos2[1]) * 0.1;
    int dist = Distance::CountDistance(pos1, pos2) * 250;

    //TODO
    float chased = 1 / exp;

    return abs((exp / dist) + direction - chased); //chased?
}

void Simulation::Run()
{
    while (true)
    {
        switch (state)
        {
        case PreStart:
            InitAttackers();
            ToGrid().Display();
            exit(0);
            state = NotDetected;
            break;

        case NotDetected:
            Iterate();
            if (AnyAttackerInsideBoundary())
            {
                state = Combat;
            }
            break;

        case Combat:
            Iterate();
            if (bombs_dropped != 0)
            {
                state = TargetUnderAttack;
            }
            break;

        case TargetUnderAttack:
            Iterate();
            if (bombs_dropped >= bombs_goal)
            {
                state = AttackersWin;
            }

            break;

        default:
            break;
        }
    }

    auto grid = ToGrid();
    grid.Display();
}

void Simulation::InitAttackers()
{
    int offset_from_border = 5;
    int planes_in_formation = 9;
    int boundary = 4;
    int number_of_formations = bombers.size() % 0 ? bombers.size() / 9 : bombers.size() / 9 + 1;
    int rows = planes_in_formation /= 2;

    // center of simulation , down the bottom, then point to the most front plane
    int first_plane_y = center[1] + sim_width - offset_from_border - boundary * rows;
}

bool Simulation::InsideBoundary(const Plane &plane) const
{
    return Distance::InRadiusFrom(plane.GetPosition(), center, battlefield_radius);
}

bool Simulation::AnyAttackerInsideBoundary()
{
    for (auto bomber : bombers)
    {
        if (InsideBoundary(bomber))
        {
            return true;
        }
    }
    for (auto attacker : attackers)
    {
        if (InsideBoundary(attacker))
        {
            return true;
        }
    }
    return false;
}

void Simulation::Iterate()
{
    std::vector<Fighter> newAttackers;
    std::vector<Fighter> newDefenders;
    std::vector<Bomber> newBombers;

    for (auto plane : attackers)
    {
        auto var = plane.Iterate(state);
        if (var.GetActive())
        {
            newAttackers.push_back(var);
        }
        else
        {
            not_active.push_back(var);
        }
    }

    for (auto plane : defenders)
    {
        auto var = plane.Iterate(state);
        if (var.GetActive())
        {
            newDefenders.push_back(var);
        }
        else
        {
            not_active.push_back(var);
        }
    }

    for (auto plane : bombers)
    {
        auto var = plane.Iterate(state);
        if (var.GetActive())
        {
            newBombers.push_back(var);
        }
        else
        {
            not_active.push_back(var);
        }
    }
}

std::vector<Fighter> &Simulation::ReturnAllEnemyFighters(const Plane &plane)
{
    switch (plane.GetTeam())
    {
    case Defenders:
        return attackers;
        break;
    case Attackers:
        return defenders;
    default:
        return defenders;
        break;
    }
}

Grid Simulation::ToGrid()
{
    Grid grid(sim_length, sim_width);
    grid.DrawBattlefield(battlefield_radius, {sim_length / 2, sim_width / 2, 0});
    grid.DrawTargetZone(target_radius, target_pos);
    return grid;
}