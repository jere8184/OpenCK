

#include "religion.hpp"

namespace openck::simulator
{
    ReligionGroup muslim;
    ReligionGroup christian;
    std::unordered_map<std::string, ReligionGroup*> ReligionGroup::religion_groups = {{"muslim", &muslim}, {"christian", &christian}};

    Religion jain;
    std::unordered_map<std::string, Religion*> Religion::religions = {{"jain", &jain}};
}