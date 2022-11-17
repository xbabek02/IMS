#ifndef __RANDOM_HPP__
#define __RANDOM_HPP__

#include <random>
#include "enums.hpp"
#include "plane.hpp"

namespace rnd
{
    int range(int n1, int n2);
    Directions direction();
    int ExperienceClash(int exp1, int exp2);
}

#endif