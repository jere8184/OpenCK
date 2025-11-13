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
        DEFAULT = 5
    };

    struct Title
    {
        struct Holder
        {
            Charecter* holder = nullptr;
            Date date_aquired = {0,0,0};
            Date date_released = {0,0,0};
        };

            /*bool adventurer();
            bool can_be_given_away();
            bool claimed_by(const Charecter* charecter);
            bool conquest_culture(const Charecter* charecter);
            bool had_title_flag(const TitleFlag* flag, Date for_time_delta);
            bool has_any_building();
            bool has_crown_law_title();
            bool has_de_jure_pretension(const Charecter* charecter);
            bool has_holder();
            bool has_holding_modifier(HoldingModifier modifier);
            bool has_instances_of_holding_modifier(HoldingModifier modifier, int count);
            bool has_mercenary_band(); 
            bool has_mercenary_maintenance_modifier();
            bool has_raised_standing_troops();
            bool has_title_flag(TitleFlag flag);
            bool holder(Charecter* charecter);
            bool is_conquered();
            bool is_contested();
            bool is_crown_law_title();
            bool is_holy_site();
            bool is_landless_type_title();
            bool is_located_in(Charecter* charecter);
            bool is_located_in(Title* title);
            bool is_preferred_holding_type(Charecter* charecter);
            bool is_primary_holder_title();
            bool is_primary_holder_title_tier();
            bool is_primary_type_title();
            bool is_recent_grant();
            bool is_titular();
            bool is_tribal_type_title();
            bool is_valid_viking_invasion_target();
            bool is_vice_royalty();
            bool mercenary_siphon_factor();
            int num_fitting_characters_for_title(Charecter* charecter);
            int num_of_buildings();
            bool temporary();
            bool would_be_heir_under_law(Charecter* Charecter /*, Law*//*); 
            bool controlled_by(Charecter* charecter);
            bool is_occupied();
            bool region(/*region*//*); 
            bool has_siege();
            bool is_allowed_holding_type(Charecter* charecter);
            bool holding_type(HoldingType type);
            bool is_connected_to(Province* target, bool sea_gap = false, bool land_gap = false, Charecter* sub_realm = nullptr, int naval_distance = 0);
            bool has_trade_post();
            int holy_site_distance(Religion* religion);
            int religion_distance(Religion* religion);*/
        
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
        TitleRank rank = TitleRank::DEFAULT;
        std::vector<Title*> defacto_sub_titles = {};
        Title* defacto_owner_title = nullptr;
        std::vector<Title*> dejure_sub_titles = {};
        Title* dejure_owner_title = nullptr;
        //Culture conquest_culture = nullptr;   
    };
}