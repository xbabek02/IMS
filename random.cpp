#include "random.hpp"

namespace rnd
{
    int range(int n1, int n2)
    {
        static std::random_device rd;                  // obtain a random number from hardware
        std::mt19937 gen(rd());                        // seed the generator
        std::uniform_int_distribution<> distr(n1, n2); // define the range

        return distr(gen);
    }

    Directions direction()
    {
        return static_cast<Directions>(rnd::range(0, 7));
    }
}
