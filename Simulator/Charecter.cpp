#include "charecter.hpp"


namespace openck
{
    bool Ruler::among_most_powerful_vassals(int number_of_vassals)
    {
        return vassal_power_rank < number_of_vassals;
    }


}

