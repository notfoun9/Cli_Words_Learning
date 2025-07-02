#include <random>
#include <cassert>

class Random
{
public:
    static int Generate(int lowerBound, int upperBound)
    {
        std::random_device               rd;
        std::mt19937                     rng{rd()};
        std::uniform_int_distribution<>  dist{lowerBound, upperBound};

        assert(lowerBound <= upperBound);
        return dist(rng);
    }
};

