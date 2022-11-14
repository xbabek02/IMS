#include "simulation.hpp"

Simulation::Simulation(int battlefield_radius) : battlefield_radius(battlefield_radius)
{
    sim_length = sim_width = battlefield_radius * 2 + 50;
    center = {sim_length / 2, sim_width / 2, 0};
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
    while (true)
    {
        switch (state)
        {
        case SimulationState::PreStart:
            InitAttackers();
            InitDefenders();
            state = NotDetected;
            break;

        case NotDetected:
            Iterate();
            if (AnyAttackerInsideBoundary())
                state = Combat;

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

    int rows = planes_in_formation / 2;
    int escort_offset = 2;

    // center of the simulation width
    int x = center.at(0);
    // down the bottom, then point to the most front plane
    int y = sim_width - offset_from_border - boundary_y * rows + 1;

    size_t bomber_counter = 0;
    size_t escort_counter = 0;

    if (bombers.size() != 0)
    {
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
    }

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
    return Distance::InRadiusFrom2D(plane.GetPosition(), center, battlefield_radius);
}

const std::vector<int> &Simulation::GetCenter() const
{
    return center;
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
        plane.IterateAttacker(state);

        if (plane.GetActive())
        {
            newAttackers.push_back(plane);
        }
        else
        {
            not_active.push_back(plane);
        }
    }

    for (auto plane : defenders)
    {
        plane.IterateDefender(state);

        if (plane.GetActive())
        {
            newDefenders.push_back(plane);
        }
        else
        {
            not_active.push_back(plane);
        }
    }

    for (auto plane : bombers)
    {
        plane.Iterate(state);

        if (plane.GetActive())
        {
            newBombers.push_back(plane);
        }
        else
        {
            not_active.push_back(plane);
        }
    }

    attackers = newAttackers;
    bombers = newBombers;
    defenders = newDefenders;
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
    grid.DrawBattlefield(battlefield_radius, center);
    grid.DrawTargetZone(target_radius, target_pos);

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

Plane &Simulation::GetById(int id)
{
    for (Bomber &bomber : bombers)
    {
        if (bomber.GetID() == id)
        {
            return bomber;
        }
    }
    for (Fighter &attacker : attackers)
    {
        if (attacker.GetID() == id)
        {
            return attacker;
        }
    }
    for (Fighter &defender : defenders)
    {
        if (defender.GetID() == id)
        {
            return defender;
        }
    }

    // something went wrong
    throw;
}

void Simulation::InitDefenders()
{
    for (Fighter &plane : defenders)
    {
        plane.SetState(Scouting);
        auto planepos = RandomBattlefieldPoint();
        planepos[2] = rnd::range(26, 33);
        plane.SetPosition(planepos);
        plane.SetDirection(rnd::direction());
    }
}

std::vector<int> Simulation::GetTarget()
{
    return target_pos;
}

std::vector<int> Simulation::RandomBattlefieldPoint()
{
    int angle = rnd::range(0, 360);
    int size = rnd::range(0, battlefield_radius);
    double x_size = sin(angle * (M_PI / 180)) * size;
    double y_size = cos(angle * (M_PI / 180)) * size;
    std::vector<int> Y({static_cast<int>(y_size + center[0]), center[1], 0});

    const int quadrant = rnd::range(1, 4);

    std::vector<int> point({Y[0], Y[1] + static_cast<int>(x_size), 0});

    switch (quadrant)
    {
    case 1:
        break;
    case 2:
        point.at(0) = 2 * center[0] - point[0];
        break;
    case 3:
        point.at(0) = 2 * center[0] - point[0];
        point.at(1) = 2 * center[1] - point[1];
        break;
    case 4:
        point.at(1) = 2 * center[1] - point[1];
        break;
    default:
        throw;
        break;
    }

    return point;
}