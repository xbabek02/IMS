#ifndef __BOMBER_HPP__
#define __BOMBER_HPP__

#include "plane.hpp"

class Bomber : public Plane
{
private:
    /* data */
public:
    Bomber(std::string name, int battles, Simulation *simulation);

    Bomber Iterate(SimulationState state);

    ~Bomber();
};

#endif