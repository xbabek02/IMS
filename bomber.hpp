#ifndef __BOMBER_HPP__
#define __BOMBER_HPP__

#include "plane.hpp"

class Bomber : public Plane
{
private:
    int max_steps = 2;

public:
    Bomber(std::string name, int battles, Simulation *simulation);

    void Iterate(SimulationState sim_state);

    virtual ~Bomber() = default;
};

#endif