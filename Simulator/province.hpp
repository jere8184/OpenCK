#pragma once

#include "title.hpp"
#include "holding.hpp"
#include "Religion.hpp"
#include <vector>
#include <unordered_map>

namespace openck 
{


    class Province
    {
        int id;
        Title* county;
        std::set<Holding*> holdings;
        Religion* religion;
        std::set<Trait*> traits;
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