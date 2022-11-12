#include "simulation.hpp"

Simulation::Simulation(int battlefield_radius) : battlefield_radius(battlefield_radius)
{
    sim_length = sim_width = battlefield_radius * 2 + 50;
    center = {sim_length / 2, sim_width / 2, 0};
    target_pos.assign({sim_length / 2, sim_width / 2, 0});
    target_radius = battlefield_radius * 2 / 15;
    airport_pos = {center[0] + battlefield_radius / 5, center[1] + battlefield_radius / 3};
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
    while (true)
    {
        switch (state)
        {
        case SimulationState::PreStart:
            InitAttackers();
            state = NotDetected;
            break;

        case NotDetected:
            Iterate();
            /*
            if (AnyAttackerInsideBoundary())
            {
                state = Combat;
            }
            */
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

        ToGrid().Display();
        usleep(100000);
        cout << "\033[2J\033[1;1H";
    }
}

void Simulation::InitAttackers()
{
    // PARAMETERS FOR FORMATION

    int offset_from_border = 5;
    int planes_in_formation = bombers.size();
    int boundary_y = 2; // 500m between planes
    int boundary_x = 5;
    /*int number_of_formations = ((bombers.size() % planes_in_formation) == 0
                                    ? bombers.size() / planes_in_formation
                                    : bombers.size() / planes_in_formation + 1);*/
    int rows = planes_in_formation / 2;
    int escort_offset = 2;

    // center of the simulation width
    int x = center.at(0);
    // down the bottom, then point to the most front plane
    int y = sim_width - offset_from_border - boundary_y * rows + 1;

    size_t bomber_counter = 0;
    size_t escort_counter = 0;

    bombers.at(bomber_counter).SetPosition({x, y, rand() % 16 + 20});
    bombers.at(bomber_counter).SetState(PlaneState::FlyingToTarget);

    // assign escort
    if (escort_counter < attackers.size())
    {
        auto position = bombers.at(bomber_counter).GetPosition();
        position[0] -= escort_offset;
        position[2] += 2;

        attackers[escort_counter].SetPosition(position);
        attackers[escort_counter].Escort(bombers.at(bomber_counter));
        escort_counter++;
    }

    bomber_counter++;

    for (int i = 0; i < (planes_in_formation / 2); i++)
    {
        int offset_y = (i + 1) * boundary_y;
        int offset_x = (i + 1) * boundary_x;

        // left
        if (bomber_counter < bombers.size())
        {
            Bomber &bomber = bombers.at(bomber_counter);
            bomber.SetPosition({x - offset_x, y + offset_y, rand() % 16 + 20});
            bomber.SetState(PlaneState::FlyingToTarget);

            // assign escort
            if (escort_counter < attackers.size())
            {
                auto position = bomber.GetPosition();
                position[0] -= escort_offset;
                position[2] += 2;

                attackers[escort_counter].SetPosition(position);
                attackers[escort_counter].Escort(bomber);
                escort_counter++;
            }

            bomber_counter++;
        }

        // right
        if (bomber_counter < bombers.size())
        {
            Bomber &bomber = bombers.at(bomber_counter);
            bomber.SetPosition({x + offset_x, y + offset_y, rand() % 16 + 20});
            bomber.SetState(PlaneState::FlyingToTarget);

            // assign escort
            if (escort_counter < attackers.size())
            {
                auto position = bomber.GetPosition();
                position[0] += escort_offset;
                position[2] += 2;

                attackers[escort_counter].SetPosition(position);
                attackers[escort_counter].Escort(bomber);
                escort_counter++;
            }

            bomber_counter++;
        }
    }
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

    /*
    for (auto plane : attackers)
    {
        auto var = Fighter(plane);
        var.Iterate(state);

        if (var.GetActive())
        {
            newAttackers.push_back(var);
        }
        else
        {
            not_active.push_back(var);
        }
    }
    */

    if (state != NotDetected)
    {
        for (auto plane : defenders)
        {
            auto var = Fighter(plane);
            var.Iterate(state);

            if (var.GetActive())
            {
                newDefenders.push_back(var);
            }
            else
            {
                not_active.push_back(var);
            }

            defenders = newDefenders;
        }
    }

    for (auto plane : bombers)
    {
        auto var = Bomber(plane);
        var.Iterate(state);

        if (var.GetActive())
        {
            newBombers.push_back(var);
        }
        else
        {
            not_active.push_back(var);
        }
    }

    // attackers = newAttackers;
    bombers = newBombers;
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
    Grid grid(sim_length);
    grid.DrawBattlefield(battlefield_radius, {sim_length / 2, sim_width / 2, 0});
    grid.DrawTargetZone(target_radius, target_pos);
    grid.DrawAirport(airport_pos);

    for (auto bomber : bombers)
    {
        auto position = bomber.GetPosition();
        if (bomber.GetActive())
            grid.SetAt(position[0], position[1], BOMBER);
    }
    for (auto attacker : attackers)
    {
        auto position = attacker.GetPosition();
        if (attacker.GetActive())
            grid.SetAt(position[0], position[1], ATTACKER);
    }
    for (auto defender : defenders)
    {
        auto position = defender.GetPosition();
        if (defender.GetActive())
            grid.SetAt(position[0], position[1], DEFENDER);
    }

    return grid;
}

std::vector<int> Simulation::GetTarget()
{
    return target_pos;
}