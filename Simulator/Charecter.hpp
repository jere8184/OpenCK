#pragma once


#include "title.hpp"
#include "date.hpp"
#include "attributes.hpp"
#include "traits.hpp"
#include "holding.hpp"
#include "religion.hpp"

#include <set>
#include <unordered_map>

namespace openck::simulator
{
    struct Abilities  
    {
        bool can_change_religion = true;
        bool can_call_crusade = false;
        bool can_copy_personality_trait_from = false;
        bool can_grant_title = false;
        bool can_have_more_leadership_traits = false;
        bool can_hold_title = false;
        bool can_join_society = false;
    };

    struct Family
    {
        Charecter* farther = nullptr;
        Charecter* real_farther = nullptr;
        Charecter* mother = nullptr;
        std::set<Charecter*> children = {};
    };

    struct Relations
    { 
        std::set<Charecter*> friends = {};
        std::set<Charecter*> rivals = {};
        std::set<Charecter*> lovers = {};
        std::set<Charecter*> kills = {};
    };
    
    struct Mana
    {
        int gold = 0;
        int prestige = 0;
        int piety = 0;
    };

    enum class Gender
    {
        DEFAULT = -1,
        MALE = 0,
        FEMALE = 1
    };

    struct Charecter 
    {
        Mana mana = {};
        Stats stats = {};
        Family family = {};
        Title* primary_title = nullptr;
        std::vector<Title*> titles = {};
        int id = -1;
        std::string first_name = "";
        std::string middle_name = "";
        std::string last_name = "";
        std::string nickname = "";
        Gender gender = Gender::DEFAULT;
        int age = -1;
        Date dateOfBirth ={};
        Date dateOfDeath = {};
        Holding* location = nullptr;
        Religion* religion = nullptr;
        Religion* secret_religion = nullptr;
        Attributes attributes = {};
        bool ai = false;

        bool can_copy_personality_trait_from(Charecter* someone_elese);
        bool can_grant_title(Charecter* someone_else);
        Religion* get_religion() {return nullptr;};
        bool controls_religion() const {return false;} 
        
        // house
        // culture
        // religion
        // bloodline
        // artifacts
        // genes
    };

    class Ruler : Charecter
    {
        
    };
}
