#pragma once

#include <string>

namespace openck::simulator
{
    struct Title;

    struct HoldingStats
    {
        int levies = 0;
        int gold = 0;  
    };

    struct HoldingModifier
    {
        std::string name = "default";
        int id = -1;
        float value = 0;
    };

    enum class HoldingType
    {
        CASTLE = 0,
        CHURCH = 1,
        CITY = 2,
        UNBUILT = 3
    };

    class Holding
    {
        HoldingType holding_type = HoldingType::UNBUILT;
        Title* barony = nullptr;
        std::string name = "default";
        
    };
} // namespace openck::simulator
