#include "simulation.hpp"

Simulation::Simulation(int battlefield_radius) : battlefield_radius(battlefield_radius)
{
    sim_length = sim_width = battlefield_radius * 2 + 50;
    center = {sim_length / 2, sim_width / 2, 0};
    target_pos.assign({sim_length / 2, sim_width / 2, 0});
    target_radius = (battlefield_radius * 2) / 15;
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

// TODO
// maximum podle ktereho se bude porovnavat muze byt 10 nebo 5
float ClashFunc(Plane chaser, Plane chasee)
{
    std::vector<int> pos1 = chaser.GetPosition();
    std::vector<int> pos2 = chasee.GetPosition();

    int expChaser = chaser.GetExperience();
    int expChasee = chasee.GetExperience();

    int direction = ((pos1[0] * pos2[0]) + (pos1[1] * pos2[1]));
    direction == 0 ? 1 : direction; // TODO
    int dist = Distance::CountDistance(pos1, pos2) * 250;

    // 2 is to normalize the result
    return (((expChaser + direction) - expChasee) / dist) + 2;
    // -2 do 2 -> 0 je 50%
    // po norm. je to 0 do 4
    // 2 je 50%
    // >= 3.8 sance 95%
    // <= 0 sance 0%

    // bombarder vs stihac -> volaji se pro oba, prvni ale bombarder
    // historicky posadky bombarderu nepresahovaly 25 misi
    // sance na zniceni bombarderu nebo stihace se pohybuje kolem 50% pokud jsou si rovni
    // bombarder je ale silny celkem a ma vysokou sanci na sestrel i pri nizke zkusenosti
    // bombarder se zmeni na chaser kdyz dojde na radu se branit, stale plati vyhoda
    // smeru

    //
}

void Simulation::Run(int speed)
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
                state = ZoneBreached;
            break;

        case ZoneBreached:
            DefendersDefend();
            Iterate();
            state = Combat;

        case Combat:
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
        usleep(speed);
        cout << "\033[2J\033[1;1H";
    }
}

void Simulation::InitAttackers()
{
    // PARAMETERS FOR FORMATION

    int offset_from_border = 12;
    int planes_in_formation = bombers.size();
    int boundary_y = 2; // 500m between planes
    int boundary_x = 4;

    int rows = planes_in_formation / 2;
    int escort_offset = 2;

    // center of the simulation width
    int x = center.at(0);
    // down the bottom, then point to the most front plane
    int y = sim_width - offset_from_border - (boundary_y * rows) + 1;

    size_t bomber_counter = 0;
    size_t escort_counter = 0;

    for (size_t i = 0; i < bombers.size(); i++)
    {
        int even = (i % 2 == 0 ? 1 : -1);
        int increment = even == 1 ? 0 : 1;
        int offset_y = ((i + increment) / 2) * boundary_y;
        int offset_x = ((i + increment) / 2) * boundary_x;

        Bomber &bomber = bombers.at(i);
        bomber.SetPosition({x + (offset_x * even), y + offset_y, rand() % 16 + 20});
        bomber.SetState(PlaneState::FlyingToTarget);

        // assign escort
        if (escort_counter < attackers.size())
        {
            auto position = bomber.GetPosition();
            position[0] -= escort_offset;
            position[1] += 7;
            position[2] += 2;

            attackers[escort_counter].SetPosition(position);
            attackers[escort_counter].Escort(bomber.GetID());
            escort_counter++;
        }
    }

    bomber_counter = 0;

    // place randomly excessive Fighters
    for (; escort_counter < attackers.size(); escort_counter++)
    {
        auto x = rnd::range(0, sim_width - 1);
        attackers[escort_counter].SetPosition({x, y, rnd::range(26, 33)});
        attackers[escort_counter].Escort(bombers[bomber_counter++].GetID());
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

void Simulation::DefendersDefend()
{
    for (Fighter &plane : defenders)
    {
        plane.SetState(LookingForTarget);
    }
}

int Simulation::GetClosestUnattackedBomber(const Plane &plane, bool orClosest)
{
    std::vector<Plane *> freeBombers;

    if (bombers.empty())
    {
        return -1;
    }

    for (auto it = targetedBombers.begin(); it != targetedBombers.end(); it++)
    {
        if (it->second.empty())
        {
            auto bomber = GetById(it->first);
            if (bomber.GetActive())
            {
                freeBombers.push_back(&bomber);
            }
        }
    }

    if (freeBombers.empty())
    {
        // return any closest bomber
        return orClosest ? GetClosestBomber(plane) : -1;
    }

    // getting the closest bomber out of free bombers
    int min = INT_MAX;
    int closest_bomber = -1;

    for (Plane *bomber : freeBombers)
    {
        auto distance = Distance::CountDistance(bomber->GetPosition(), plane.GetPosition());
        min = (min < distance ? min : distance);
        closest_bomber = bomber->GetID();
    }

    return closest_bomber;
}

int Simulation::GetClosestBomber(const Plane &plane)
{
    int min = INT_MAX;
    int closest_bomber = -1;

    for (auto &bomber : bombers)
    {
        auto distance = Distance::CountDistance(bomber.GetPosition(), plane.GetPosition());
        min = (min < distance ? min : distance);
        closest_bomber = bomber.GetID();
    }

    return closest_bomber;
}

void Simulation::UpdateBomberChaser(int bomberId, int chaserId)
{
    targetedBombers[bomberId].push_back(chaserId);
}

void Simulation::ExperienceSwapChase()
{
    for (auto &fighter : attackers)
    {
        if (fighter.GetState() == Evading)
        {
            fighter.SwapSkillCheck();
        }
    }

    for (auto &fighter : defenders)
    {
        if (fighter.GetState() == Evading)
        {
            fighter.SwapSkillCheck();
        }
    }
}

void Simulation::Iterate()
{
    std::vector<Fighter> newAttackers;
    std::vector<Fighter> newDefenders;
    std::vector<Bomber> newBombers;

    ExperienceSwapChase();

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

    // updating the targeting of bombers
    UpdateBomberTargeting();
    iteration++;
}

void Simulation::UpdateBomberTargeting()
{
    for (auto it = targetedBombers.begin(); it != targetedBombers.end(); it++)
    {
        auto &vec = it->second;
        for (auto inner_it = vec.begin(); inner_it != vec.end();)
        {
            auto plane = GetById(*inner_it);
            if (plane.GetTargetId() != it->first || plane.GetState() != Chasing)
            {
                inner_it = vec.erase(inner_it);
            }
            else
            {
                inner_it++;
            }
        }
    }

    // Get Duplicite Attackers
    std::vector<int> duplicates;

    for (auto it = targetedBombers.begin(); it != targetedBombers.end(); it++)
    {
        if (it->second.size() > 1)
        {
            for (auto inner_it = it->second.begin() + 1; inner_it != it->second.end(); inner_it++)
            {
                duplicates.push_back(*inner_it);
            }
        }
    }

    // Assign new attackers to non attacked bombers
    for (auto planeId : duplicates)
    {
        Fighter &plane = dynamic_cast<Fighter &>(GetById(planeId));
        int bomberId = GetClosestUnattackedBomber(plane, false);
        if (bomberId == -1)
        {
            return;
        }
        plane.ChaseBomber(bomberId);
    }
}

std::vector<Fighter> &Simulation::GetAllEnemyFighters(const Plane &plane)
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

std::vector<Bomber> &Simulation::GetAllBombers()
{
    return bombers;
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

int Simulation::GetTargetRadius() const
{
    return target_radius;
}

int Simulation::GetIteration() const
{
    return iteration;
}

void Simulation::DropBomb()
{
    bombs_dropped++;
}

int Simulation::GetBattlefieldRadius() const
{
    return battlefield_radius;
}

int Simulation::GetAttackerId(const Plane &plane)
{
    auto enemies = GetAllEnemyFighters(plane);
    for (auto &enemy : enemies)
    {
        if (enemy.GetTargetId() == plane.GetID() && enemy.GetState() == Chasing)
        {
            return enemy.GetID();
        }
    }
    return -1;
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