#pragma once

#include "holding.hpp"
#include "religion.hpp"
#include "traits.hpp"

#include <vector>
#include <unordered_map>
#include <set>

namespace openck::simulator 
{

struct Title;

class Province
{
    int id;
    Title* county;
    std::set<Holding*> holdings;
    Religion* religion;
    std::set<Province*> neigbours;

    bool controlled_by(Charecter* charecter);
    bool is_occupied();
    bool region(/*region*/);
    bool holding_type(HoldingType type);
    bool has_trade_post();
    bool has_started_building_wonder_upgrade();



    //culture
    //effects
    //supply
};
}