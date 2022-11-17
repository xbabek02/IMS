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

    int ExperienceClash(int exp1, int exp2)
    {
        if (rnd::range(0, 1) == 0)
        {
            return 0;
        }

        int sum = exp1 + exp2;

        int random = rnd::range(1, sum);
        if (random <= exp1)
        {
            return 1;
        }
        else
        {
            return 2;
        }
    }
}
