#pragma once

#include "date.hpp"
#include "province.hpp"
#include "culture.hpp"
#include "charecter.hpp"

#include <vector>
#include <string>
namespace openck::simulator 
{

    struct TitleFlag
    {
        std::string name = "Default";
        int id = -1;
        Date active_from{0,0,0};
        Date active_till{0,0,0};
    };

    enum class TitleRank 
    {
        BARONY = 0,
        COUNTY = 1,
        DUTCHY = 2,
        KINGDOM = 3,
        EMPIER = 4,
        NOT_SET = 5
    };

    struct Title
    {
        struct Holder
        {
            Charecter* holder = nullptr;
            Date date_aquired = {0,0,0};
            Date date_released = {0,0,0};
        };


        bool controls_religion() const { return false; };
        simulator::Religion* get_religion() const { return nullptr; };


        bool adventurer = false;
        bool can_be_given_away = false;
        Culture* conquest_culture = nullptr;
        Holder* current_holder = nullptr;
        bool is_contested = false;


        std::vector<Holder> history = {};
        std::vector<Charecter*> claimants = {};
        std::vector<TitleFlag> flags = {};
        std::vector<Province*> provinces = {};
        int id = -1;
        std::string name = "default";
        TitleRank rank = TitleRank::NOT_SET;
        std::vector<Title*> defacto_sub_titles = {};
        Title* defacto_owner_title = nullptr;
        std::vector<Title*> dejure_sub_titles = {};
        Title* dejure_owner_title = nullptr;
        //Culture conquest_culture = nullptr;   
    };
}