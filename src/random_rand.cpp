#include <ctime>
#include "random_rand.hpp"

namespace game {

int random_rand::get_random_number(int min, int max) const {
    return (max - min + 1) * (static_cast<double>(rand()) / RAND_MAX) + min;
}

} // namespace game
