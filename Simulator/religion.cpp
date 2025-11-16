

#include "religion.hpp"

namespace openck::simulator
{
    ReligionGroup muslim;
    std::unordered_map<std::string, ReligionGroup*> ReligionGroup::religion_groups = {{"muslim", &muslim}};

    Religion jain;
    std::unordered_map<std::string, Religion*> Religion::religion = {{"jain", &jain}};
}