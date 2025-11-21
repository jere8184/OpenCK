
#include "traits.hpp"
#include "army.hpp"

#include <concepts>
#include <functional>
#include <ranges>
#include <print>

namespace openck::simulator 
{

bool Trait::init(const parser::Node& trait_node)
{
    this->name = trait_node.name;

    for (const auto& [child_name, child_node] : trait_node.children_map)
    {
        if (trait_field_setters.contains(child_name))
        {
            if (trait_field_setters.at(child_name)(this, child_node))
            {
                continue;
            }
            else
            {
                std::print(stderr, "Failed to set trait field: {} for trait: {}\n", child_name, this->name);
                return false;
            }
        }
        else
        {
            int index;
            if (index = child_name.find("_opinion"); (index != std::string::npos) && (child_name.size() == index + std::string("_opinion").size()))
            {
                return this->set_opinion_modifer(child_node, Opinon::From::DYNAMIC);
            }
            else
            {
                std::print(stderr, "Unknown trait field: {}\n", child_name);
                return false;
            }
        }
    }
    return true;
}

bool Trait::set_potential(const Node &node)
{
    bool is_success = true;
    scripting::ConditionBlock<scripting::CharacterScope> block = scripting::ConditionBlock<scripting::CharacterScope>(node, is_success);
    
    if (is_success)
    {
        this->condition_block.emplace(std::move(block));
        return true;
    }
    else
    {
        return false;
    }
}

std::unordered_map<std::string, std::function<bool(Trait*, const Node&)>> Trait::trait_field_setters =
{
    {"attribute", &Trait::set_attribute},
    {"education", &Trait::set_education},
    {"martial", std::bind(&Trait::set_attribute_modifer, std::placeholders::_1, std::placeholders::_2, AttributesType::MARTIAL)},
    {"diplomacy", std::bind(&Trait::set_attribute_modifer, std::placeholders::_1, std::placeholders::_2, AttributesType::DIPLOMACY)},
    {"stewardship", std::bind(&Trait::set_attribute_modifer, std::placeholders::_1, std::placeholders::_2, AttributesType::STEWARDSHIP)},
    {"intrigue", std::bind(&Trait::set_attribute_modifer, std::placeholders::_1, std::placeholders::_2, AttributesType::INTRIGUE)},
    {"learning", std::bind(&Trait::set_attribute_modifer, std::placeholders::_1, std::placeholders::_2, AttributesType::LEARNING)},
    {"combat_rating", std::bind(&Trait::set_attribute_modifer, std::placeholders::_1, std::placeholders::_2, AttributesType::COMBAT_RATING)},

    {"martial_penalty", std::bind(&Trait::set_attribute_penalty, std::placeholders::_1, std::placeholders::_2, AttributesType::MARTIAL)},
    {"diplomacy_penalty", std::bind(&Trait::set_attribute_penalty, std::placeholders::_1, std::placeholders::_2, AttributesType::DIPLOMACY)},
    {"stewardship_penalty", std::bind(&Trait::set_attribute_penalty, std::placeholders::_1, std::placeholders::_2, AttributesType::STEWARDSHIP)},
    {"intrigue_penalty", std::bind(&Trait::set_attribute_penalty, std::placeholders::_1, std::placeholders::_2, AttributesType::INTRIGUE)},
    {"learning_penalty", std::bind(&Trait::set_attribute_penalty, std::placeholders::_1, std::placeholders::_2, AttributesType::LEARNING)},
    
    {"male_insult_adj", std::bind(&Trait::set_greeting_adjective, std::placeholders::_1, std::placeholders::_2, Greeting::Target::MALE, Greeting::Type::INSULT)},
    {"female_insult_adj", std::bind(&Trait::set_greeting_adjective, std::placeholders::_1, std::placeholders::_2, Greeting::Target::FEMALE, Greeting::Type::INSULT)},
    {"child_insult_adj", std::bind(&Trait::set_greeting_adjective, std::placeholders::_1, std::placeholders::_2, Greeting::Target::CHILD, Greeting::Type::INSULT)},
    {"male_compliment_adj", std::bind(&Trait::set_greeting_adjective, std::placeholders::_1, std::placeholders::_2, Greeting::Target::MALE, Greeting::Type::COMPLIMENT)},
    {"female_compliment_adj", std::bind(&Trait::set_greeting_adjective, std::placeholders::_1, std::placeholders::_2, Greeting::Target::FEMALE, Greeting::Type::COMPLIMENT)},
    {"child_compliment_adj", std::bind(&Trait::set_greeting_adjective, std::placeholders::_1, std::placeholders::_2, Greeting::Target::CHILD, Greeting::Type::COMPLIMENT)},
    
    {"male_insult", std::bind(&Trait::set_greeting, std::placeholders::_1, std::placeholders::_2, Greeting::Target::MALE, Greeting::Type::INSULT)},
    {"female_insult", std::bind(&Trait::set_greeting, std::placeholders::_1, std::placeholders::_2, Greeting::Target::FEMALE, Greeting::Type::INSULT)},
    {"child_insult", std::bind(&Trait::set_greeting, std::placeholders::_1, std::placeholders::_2, Greeting::Target::CHILD, Greeting::Type::INSULT)},
    {"male_compliment", std::bind(&Trait::set_greeting, std::placeholders::_1, std::placeholders::_2, Greeting::Target::MALE, Greeting::Type::COMPLIMENT)},
    {"female_compliment", std::bind(&Trait::set_greeting, std::placeholders::_1, std::placeholders::_2, Greeting::Target::FEMALE, Greeting::Type::COMPLIMENT)},
    {"child_compliment", std::bind(&Trait::set_greeting, std::placeholders::_1, std::placeholders::_2, Greeting::Target::CHILD, Greeting::Type::COMPLIMENT)},
    
    {"fertility", std::bind(&Trait::set_stat_modifer, std::placeholders::_1, std::placeholders::_2, StatType::FERTILITY)},
    {"health", std::bind(&Trait::set_stat_modifer, std::placeholders::_1, std::placeholders::_2, StatType::HEALTH)},
    {"fertility_penalty", std::bind(&Trait::set_stat_penalty, std::placeholders::_1, std::placeholders::_2, StatType::FERTILITY)},
    {"health_penalty", std::bind(&Trait::set_stat_penalty, std::placeholders::_1, std::placeholders::_2, StatType::HEALTH)},
    
    {"ai_zeal", std::bind(&Trait::set_stat_modifer, std::placeholders::_1, std::placeholders::_2, StatType::ZEAL)},
    {"ai_greed", std::bind(&Trait::set_stat_modifer, std::placeholders::_1, std::placeholders::_2, StatType::GREED)},
    {"ai_ambition", std::bind(&Trait::set_stat_modifer, std::placeholders::_1, std::placeholders::_2, StatType::AMBITION)},
    {"ai_rationality", std::bind(&Trait::set_stat_modifer, std::placeholders::_1, std::placeholders::_2, StatType::RATIONALITY)},
    {"ai_honor", std::bind(&Trait::set_stat_modifer, std::placeholders::_1, std::placeholders::_2, StatType::HONOR)},

    {"general_opinion", std::bind(&Trait::set_opinion_modifer, std::placeholders::_1, std::placeholders::_2, Opinon::From::GENERAL)},
    {"church_opinion", std::bind(&Trait::set_opinion_modifer, std::placeholders::_1, std::placeholders::_2, Opinon::From::CHURCH)},
    {"christian_church_opinion", std::bind(&Trait::set_opinion_modifer, std::placeholders::_1, std::placeholders::_2, Opinon::From::CHRISTIAN_CHURCH)},
    {"spouse_opinion", std::bind(&Trait::set_opinion_modifer, std::placeholders::_1, std::placeholders::_2, Opinon::From::SPOUSE)},
    {"sex_appeal_opinion", std::bind(&Trait::set_opinion_modifer, std::placeholders::_1, std::placeholders::_2, Opinon::From::SEX_APPEAL)},
    {"dynasty_opinion", std::bind(&Trait::set_opinion_modifer, std::placeholders::_1, std::placeholders::_2, Opinon::From::DYNASTY)},
    {"liege_opinion", std::bind(&Trait::set_opinion_modifer, std::placeholders::_1, std::placeholders::_2, Opinon::From::LIEGE)},
    {"vassal_opinion", std::bind(&Trait::set_opinion_modifer, std::placeholders::_1, std::placeholders::_2, Opinon::From::VASSAL)},
    {"infidel_opinion", std::bind(&Trait::set_opinion_modifer, std::placeholders::_1, std::placeholders::_2, Opinon::From::INFIDEL)},
    {"opposite_opinion", std::bind(&Trait::set_opinion_modifer, std::placeholders::_1, std::placeholders::_2, Opinon::From::OPPOSITE)},
    {"same_opinion", std::bind(&Trait::set_opinion_modifer, std::placeholders::_1, std::placeholders::_2, Opinon::From::SAME)},
    {"same_opinion_if_same_religion", std::bind(&Trait::set_opinion_modifer, std::placeholders::_1, std::placeholders::_2, Opinon::From::SAME_PLUS_SAME_RELIGION)},
    {"twin_opinion", std::bind(&Trait::set_opinion_modifer, std::placeholders::_1, std::placeholders::_2, Opinon::From::TWIN)},
    {"ambition_opinion", std::bind(&Trait::set_opinion_modifer, std::placeholders::_1, std::placeholders::_2, Opinon::From::AMBITION)},
    {"same_religion_opinion", std::bind(&Trait::set_opinion_modifer, std::placeholders::_1, std::placeholders::_2, Opinon::From::SAME_RELIGION)},
    {"tribal_opinion", std::bind(&Trait::set_opinion_modifer, std::placeholders::_1, std::placeholders::_2, Opinon::From::UNREFOMED_TRIBAL)},
    {"unreformed_tribal_opinion", std::bind(&Trait::set_opinion_modifer, std::placeholders::_1, std::placeholders::_2, Opinon::From::UNREFOMED_TRIBAL)},

    {"opposites", &Trait::set_opposites},

    {"leadership_traits", [](Trait* trait, const Node& node){return node.get_value(trait->flags.leadership_traits);}},
    {"customizer", [](Trait* trait, const Node& node){return node.get_value(trait->flags.customizer);}},
    {"religious", [](Trait* trait, const Node& node){return node.get_value(trait->flags.religious);}},
    {"is_health", [](Trait* trait, const Node& node){return node.get_value(trait->flags.is_health);}},
    {"is_illness", [](Trait* trait, const Node& node){return node.get_value(trait->flags.is_illness);}},
    {"is_epidemic", [](Trait* trait, const Node& node){return node.get_value(trait->flags.is_epidemic);}},
    {"inherit_chance", [](Trait* trait, const Node& node){return node.get_value(trait->flags.inherit_chance);}},
    {"succession_gfx", [](Trait* trait, const Node& node){return node.get_value(trait->flags.succession_gfx);}},
    {"incapacitating", [](Trait* trait, const Node& node){return node.get_value(trait->flags.incapacitating);}},
    {"ruler_designer_cost", [](Trait* trait, const Node& node){return node.get_value(trait->flags.ruler_designer_cost);}},
    {"birth", [](Trait* trait, const Node& node){return node.get_value(trait->flags.chance_on_birth);}},
    {"both_parent_has_trait_inherit_chance", [](Trait* trait, const Node& node){return node.get_value(trait->flags.both_parent_has_trait_inherit_chance);}},
    {"inbred", [](Trait* trait, const Node& node){return node.get_value(trait->flags.inbred);}},
    {"hidden", [](Trait* trait, const Node& node){return node.get_value(trait->flags.hidden);}},
    {"immortal", [](Trait* trait, const Node& node){return node.get_value(trait->flags.immortal);}},
    {"cannot_inherit", [](Trait* trait, const Node& node){return node.get_value(trait->flags.cannot_inherit);}},
    {"cannot_marry", [](Trait* trait, const Node& node){return node.get_value(trait->flags.cannot_marry);}},
    {"agnatic", [](Trait* trait, const Node& node){return node.get_value(trait->flags.agnatic);}},
    {"enatic", [](Trait* trait, const Node& node){return node.get_value(trait->flags.enatic);}},
    {"childhood", [](Trait* trait, const Node& node){return node.get_value(trait->flags.childhood);}},
    {"lifestyle", [](Trait* trait, const Node& node){return node.get_value(trait->flags.lifestyle);}},
    {"personality", [](Trait* trait, const Node& node){return node.get_value(trait->flags.personality);}},
    {"prevent_decadence", [](Trait* trait, const Node& node){return node.get_value(trait->flags.prevent_decadence);}},
    {"priest", [](Trait* trait, const Node& node){return node.get_value(trait->flags.priest);}},
    {"pilgrimage", [](Trait* trait, const Node& node){return node.get_value(trait->flags.pilgrimage);}},
    {"vice", [](Trait* trait, const Node& node){return node.get_value(trait->flags.vice);}},
    {"virtue", [](Trait* trait, const Node& node){return node.get_value(trait->flags.virtue);}},
    {"leader", [](Trait* trait, const Node& node){return node.get_value(trait->flags.leader);}},
    {"monthly_character_piety", [](Trait* trait, const Node& node){return node.get_value(trait->buffs.monthly_character_piety);}},
    {"monthly_character_prestige", [](Trait* trait, const Node& node){return node.get_value(trait->buffs.monthly_character_prestige);}},
    {"global_tax_modifier", [](Trait* trait, const Node& node){return node.get_value(trait->buffs.global_tax_modifier);}},
    {"command_modifier", std::bind(&Trait::set_command_modifier, std::placeholders::_1, std::placeholders::_2)},
    {"command_modifier.random", [](Trait* trait, const Node& node) { return node.get_value(trait->command_modifiers.random); }},
    {"command_modifier.speed", [](Trait* trait, const Node& node) { return node.get_value(trait->command_modifiers.speed); }},
    {"command_modifier.retreat", [](Trait* trait, const Node& node) { return node.get_value(trait->command_modifiers.retreat); }},
    {"command_modifier.defence", [](Trait* trait, const Node& node) { return node.get_value(trait->command_modifiers.defence); }},
    {"command_modifier.damage", [](Trait* trait, const Node& node) { return node.get_value(trait->command_modifiers.damage); }},
    {"command_modifier.center", [](Trait* trait, const Node& node) { return node.get_value(trait->command_modifiers.center); }},
    {"command_modifier.flank", [](Trait* trait, const Node& node) { return node.get_value(trait->command_modifiers.flank); }},
    {"command_modifier.pursue", [](Trait* trait, const Node& node) { return node.get_value(trait->command_modifiers.pursue); }},
    {"command_modifier.siege", [](Trait* trait, const Node& node) { return node.get_value(trait->command_modifiers.siege); }},
    {"command_modifier.morale_offence", [](Trait* trait, const Node& node) { return node.get_value(trait->command_modifiers.morale_offence); }},
    {"command_modifier.morale_defence", [](Trait* trait, const Node& node) { return node.get_value(trait->command_modifiers.morale_defence); }},
    {"command_modifier.cavalry", [](Trait* trait, const Node& node) { return node.get_value(trait->command_modifiers.cavalry); }},


    {"potential", std::bind(&Trait::set_potential, std::placeholders::_1, std::placeholders::_2)},
    {"random", [](Trait* trait, const Node& node){return node.get_value(trait->flags.random);}}
};

std::unordered_map<std::string, Trait*> Trait::traits;

Trait::Trait(const parser::Node& trait_node, scripting::CharacterScope& charecter_scope) : charecter_scope(charecter_scope), name(trait_node.name)
{
    for (const UnitType& unit_type : unit_vector)
    {
        this->command_modifiers.unit_specific_buffs[&unit_type] = 0;
    }

    if (init(trait_node))
        Trait::traits[this->name] = this; 
}

bool Trait::set_attribute(const Node& node)
{
    if (node.value == "intrigue")
        this->flags.attribute = AttributesType::INTRIGUE;
    else if (node.value == "martial")
        this->flags.attribute = AttributesType::MARTIAL;
    else if (node.value == "learning")
        this->flags.attribute = AttributesType::LEARNING;
    else if (node.value == "diplomacy")
        this->flags.attribute = AttributesType::DIPLOMACY;
    else if (node.value == "stewardship")
        this->flags.attribute = AttributesType::STEWARDSHIP;
    else
        return false;
    return true;
}

bool Trait::set_attribute_modifer(const Node& node, const AttributesType attribute_type)
{
    switch (attribute_type)
    {
        case AttributesType::DIPLOMACY :
            return node.get_value(this->attribute_modifiers.diplomacy);
            break;
        
        case AttributesType::STEWARDSHIP :
            return node.get_value(this->attribute_modifiers.stewardship);
            break;
        
        case AttributesType::INTRIGUE :
            return node.get_value(this->attribute_modifiers.intrigue);
            break;
        
        case AttributesType::LEARNING :
            return node.get_value(this->attribute_modifiers.learning);
            break;
        
        case AttributesType::MARTIAL :
            return node.get_value(this->attribute_modifiers.martial);
            break;
        
        case AttributesType::COMBAT_RATING :
            return node.get_value(this->attribute_modifiers.combat);
            break;

        default : 
            return false;
            break;
    }
    return true;
}

bool Trait::set_stat_modifer(const Node &node, const StatType stat_type)
{
    switch (stat_type)
    {
    case StatType::AMBITION :
        return node.get_value(this->stat_modifiers.amabition);
        break;
    
    case StatType::FERTILITY :
        return node.get_value(this->stat_modifiers.fertility);
        break;

    case StatType::HEALTH :
        return node.get_value(this->stat_modifiers.health);
        break;

    case StatType::GREED :
        return node.get_value(this->stat_modifiers.greed);
        break;

    case StatType::RATIONALITY :
        return node.get_value(this->stat_modifiers.rationality);
        break;

    case StatType::ZEAL :
        return node.get_value(this->stat_modifiers.zeal);
        break;

    case StatType::HONOR :
        return node.get_value(this->stat_modifiers.honor);
        break;

    case StatType::THREAT_DECAY_SPEED :
        return node.get_value(this->stat_modifiers.threat_decay_speed);
        break;
        
    default:
        break;
    }
    return false;
}

bool Trait::set_attribute_penalty(const Node& node, const AttributesType attribute_type)
{
    switch (attribute_type)
    {
        case AttributesType::DIPLOMACY :
            return node.get_value(this->attribute_modifiers.diplomacy_penalty);
            break;
        
        case AttributesType::STEWARDSHIP :
            node.get_value(this->attribute_modifiers.stewardship_penalty);
            break;
        
        case AttributesType::INTRIGUE :
            node.get_value(this->attribute_modifiers.intrigue_penalty);
            break;
        
        case AttributesType::LEARNING :
            node.get_value(this->attribute_modifiers.learning_penalty);
            break;
        
        case AttributesType::MARTIAL :
            node.get_value(this->attribute_modifiers.martial_penalty);
            break;

        default : 
            return false;
            break;
    }
    return true;
}


bool Trait::set_stat_penalty(const Node &node, const StatType stat_type)
{
    switch (stat_type)
    {
    case StatType::FERTILITY :
        return node.get_value(this->stat_modifiers.fertility_penalty);
        break;
    
    case StatType::HEALTH :
        return node.get_value(this->stat_modifiers.health_penalty);
        break;
    
    default:
        return false;
        break;
    }

}


bool Trait::set_opinion_modifer(const Node &node, const Opinon::From from)
{
    switch (from)
    {
    case Opinon::From::GENERAL :
        return node.get_value(this->opinion.general_opinion);
        break;
        
    case Opinon::From::CHURCH :
        return node.get_value(this->opinion.church_opinion);
        break;

    case Opinon::From::CHRISTIAN_CHURCH :
        return node.get_value(this->opinion.christian_church_opinion);
        break;
    
    case Opinon::From::SPOUSE :
        return node.get_value(this->opinion.spouse_opinion);
        break;
    
    case Opinon::From::SEX_APPEAL :
        return node.get_value(this->opinion.sex_appeal_opinion);;
        break;

    case Opinon::From::DYNASTY :
        return node.get_value(this->opinion.dynasty_opinion);
        break;
    
    case Opinon::From::LIEGE :
        return node.get_value(this->opinion.liege_opinion);
        break;
    
    case Opinon::From::VASSAL :
        return node.get_value(this->opinion.vassal_opinion); 
        break;

    case Opinon::From::INFIDEL :
        return node.get_value(this->opinion.infidel_opinion);
        break;

    case Opinon::From::OPPOSITE :
        return node.get_value(this->opinion.opposite_opinion);
        break;

    case Opinon::From::SAME :   
        return node.get_value(this->opinion.same_opinion);
        break;

    case Opinon::From::SAME_PLUS_SAME_RELIGION :
        return node.get_value(this->opinion.same_plus_if_same_religion_opinion);
        break;

    case Opinon::From::TWIN :
        return node.get_value(this->opinion.twin_opinion);
        break;
    
    case Opinon::From::AMBITION :
        return node.get_value(this->opinion.ambition_opinion);
        break;

    case Opinon::From::SAME_RELIGION :
        return node.get_value(this->opinion.same_religion_opinion);
        break;

    case Opinon::From::UNREFOMED_TRIBAL :
        return node.get_value(this->opinion.unreformed_tribal_opinion);
        break;

    case Opinon::From::DYNAMIC :
        return this->set_opinion_modifer_dynamic(node);
        break;

    default:
        return false;
        break;
    }
}

bool Trait::set_opposites(const Node &node)
{
    for (const std::string& child_name : node.children_map | std::views::keys)
    {
        const Trait* opposite_trait = get_trait_by_name(child_name);
        if (opposite_trait)
            this->flags.opposites.push_back(opposite_trait);
        else
            return false;
    }
    return true;
}

bool Trait::set_command_modifier(const Node &node)
{
    bool is_success = true;
    for (const auto& [child_name, child_node] : node.children_map)
    {
        std::string setter_key = ("command_modifier.") + child_name;
        if (trait_field_setters.contains(setter_key))
        {
            trait_field_setters.at(setter_key)(this, child_node);
        }
        else if(UnitType::unit_type_map.contains(child_name))
        {
            UnitType* unit = UnitType::unit_type_map.at(child_name);
            float val;

            if (child_node.get_value(val))
            {
                this->command_modifiers.unit_specific_buffs.at(unit) = val;
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            is_success = false;
        }
    }
    return is_success;
}

bool Trait::set_opinion_modifer_dynamic(const Node & node)
{
    int val;
    if (!node.get_value(val))
        return false;

    std::string from_name = node.name.substr(0, node.name.size() - std::string_view("_opinion").size());

    if (Religion* religion = Religion::get_religion_by_name(from_name))
    {
        this->opinion.religion_opinions[religion] = val;
        return true;
    }
    else
    {
        return false;
    }
}

bool Trait::set_education(const Node& node)
{
    if (node.value == "yes")
    {
        this->flags.education = true;
        return true;
    }
    else if (node.value == "no")
    {
        this->flags.education = false;
        return true;
    }
    return false;
}

const std::string* get_localisation(const std::string& key) {static std::string dummy_val(""); return &dummy_val; /*Dummy function*/}

bool Trait::set_greeting_adjective(const Node& node, const Greeting::Target target, const Greeting::Type type)
{
    const std::string** value_ptr = nullptr;
    switch (type)
    {
        case Greeting::Type::INSULT :
            switch (target)
            {
            case Greeting::Target::MALE :
                value_ptr = &this->adjectives.male_insult_adjective;
                break;

            case Greeting::Target::FEMALE :
                value_ptr = &this->adjectives.female_insult_adjective;
                break;

            case Greeting::Target::CHILD :
                value_ptr = &this->adjectives.child_insult_adjective;
                break;

            default :
                return false;
                break;
            }
            break;
        
        case Greeting::Type::COMPLIMENT :
            switch (target)
            {
            case Greeting::Target::MALE :
                value_ptr = &this->adjectives.male_compliment_adjective;
                break;

            case Greeting::Target::FEMALE :
                value_ptr = &this->adjectives.female_compliment_adjective;
                break;

            case Greeting::Target::CHILD :
                value_ptr = &this->adjectives.child_compliment_adjective;
                break;

            default :
                return false;
                break;
            }
            break;
        
        default : 
            return false;
            break;
        }
    return value_ptr != nullptr && (*value_ptr = get_localisation(node.value)) != nullptr;
}

bool Trait::set_greeting(const Node& node, const Greeting::Target target, const Greeting::Type type)
{
    const std::string** value_ptr = nullptr;
    switch (type)
    {
        case Greeting::Type::INSULT :
            switch (target)
            {
            case Greeting::Target::MALE :
                value_ptr = &this->adjectives.male_insult;
                break;

            case Greeting::Target::FEMALE :
                value_ptr = &this->adjectives.female_insult;
                break;

            case Greeting::Target::CHILD :
                value_ptr = &this->adjectives.child_insult;
                break;

            default :
                return false;
                break;
            }
            break;
        
        case Greeting::Type::COMPLIMENT :
            switch (target)
            {
            case Greeting::Target::MALE :
                value_ptr = &this->adjectives.male_compliment;
                break;

            case Greeting::Target::FEMALE :
                value_ptr = &this->adjectives.female_compliment;
                break;

            case Greeting::Target::CHILD :
                value_ptr = &this->adjectives.child_compliment;
                break;

            default :
                return false;
                break;
            }
            break;
        
        default : 
            return false;
            break;
        }
    return value_ptr != nullptr && (*value_ptr = get_localisation(node.value)) != nullptr;
}


std::vector<Trait> generate_traits_from_nodes(const std::vector<parser::Node>& root_nodes)
{
    std::vector<Trait> traits;

    //auto a = std::bind(set_attribute_modifer, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    scripting::CharacterScope charecter_scope;
    charecter_scope.type = scripting::Scope::Type::CHARACTER;

    for (const parser::Node& trait_node : root_nodes) //
    {
        Trait trait(trait_node, charecter_scope);
        traits.push_back(std::move(trait));
    }

    return traits;
}

}