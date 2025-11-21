
#include "army.hpp"

namespace openck::simulator 
{
    UnitType light_infantry {"light_infantry"};
    UnitType heavy_infantry {"heavy_infantry"};
    UnitType pikemen {"pikemen"};
    UnitType light_cavalry {"light_cavalry"};
    UnitType knights {"knights"};
    UnitType archers {"archers"};
    UnitType special_troops {"special_troops"};

    std::vector<UnitType> unit_vector {light_infantry,
                                            heavy_infantry,
                                            pikemen,
                                            light_cavalry,
                                            knights,
                                            archers,
                                            special_troops};

    std::map<std::string, UnitType*> UnitType::unit_types = {{"light_infantry", &unit_vector[0]},
                                                                    {"heavy_infantry", &unit_vector[1]},
                                                                    {"pikemen", &unit_vector[2]},
                                                                    {"light_cavalry", &unit_vector[3]},
                                                                    {"knights", &unit_vector[4]},
                                                                    {"archers", &unit_vector[5]},
                                                                    {"special_troops", &unit_vector[6]}};
}