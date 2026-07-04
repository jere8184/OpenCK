#pragma once

#include "attributes.hpp"
#include "religion.hpp"
#include "base.hpp"

#include "parser/prdx_parser.h"
#include "scripting/condition.hpp"

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <optional>

namespace openck::simulator
{

struct Phase;
struct UnitType;
struct Terrain;

struct Trait : public Base<Trait>
{
	using Base = Base<Trait>;

	struct Flags
	{
		bool agnatic = false;
		bool enatic = false;
		unsigned short chance_on_birth = 0;
		bool blinding = false;
		bool cached = false;
		bool can_hold_titles = true;
		bool cannot_inherit = false;
		bool cannot_marry = false;
		unsigned int caste_tier = 0;
		bool customizer = true;
		bool education = false;
		AttributesType attribute = AttributesType::NOT_SET;
		bool hidden = false;
		bool immortal = false;
		bool in_hiding = false;
		bool inbred = false;
		bool incapacitating = false;
		unsigned short inherit_chance = 0;
		unsigned short both_parent_has_trait_inherit_chance = 0;
		bool is_epidemic = false;
		bool is_health = false;
		bool is_illness = false;
		bool leader = false;
		unsigned short leadership_traits = 0;
		bool childhood = false;
		bool lifestyle = false;
		bool personality = false;
		std::vector<const Trait*> opposites = {}; //List of traits that cannot be held simultaneously with this trait
		bool prevent_decadence = false;
		bool priest = false;
		bool pilgrimage = false;
		bool random = false;
		bool rebel_inherited = false;
		bool religious = false;
		const Religion* religious_branch = nullptr;
		unsigned int ruler_designer_cost = 0;
		std::vector<Religion*> tolerates_religion_group;
		bool same_trait_visibility = false;
		bool hidden_from_others = false;
		bool is_visible = false;
		bool vice = false;
		bool virtue = false;
		bool succession_gfx = false;
		bool is_symptom = false;
	};

	struct Greeting
	{
		enum struct Target
		{
			NOT_SET,
			MALE,
			FEMALE,
			CHILD
		};

		enum struct Type
		{
			NOT_SET,
			INSULT,
			COMPLIMENT
		};

		const std::string* male_insult_adjective = nullptr;
		const std::string* female_insult_adjective = nullptr;
		const std::string* child_insult_adjective = nullptr;
		const std::string* male_insult = nullptr;
		const std::string* female_insult = nullptr;
		const std::string* child_insult = nullptr;

		const std::string* male_compliment_adjective = nullptr;
		const std::string* female_compliment_adjective = nullptr;
		const std::string* child_compliment_adjective = nullptr;
		const std::string* male_compliment = nullptr;
		const std::string* female_compliment = nullptr;
		const std::string* child_compliment = nullptr;
	};

	struct Opinon
	{
		enum struct From
		{
			NOT_SET,
			GENERAL,
			CHURCH,
			CHRISTIAN_CHURCH,
			SPOUSE,
			SEX_APPEAL,
			DYNASTY,
			LIEGE,
			VASSAL,
			INFIDEL,
			OPPOSITE,
			SAME,
			SAME_PLUS_SAME_RELIGION,
			TWIN,
			AMBITION,
			SAME_RELIGION,
			TRIBAL,
			UNREFOMED_TRIBAL,
			DYNAMIC
		};

		int general_opinion = 0;
		int church_opinion = 0;
		int christian_church_opinion = 0;
		int spouse_opinion = 0;
		int sex_appeal_opinion = 0;
		int dynasty_opinion = 0;
		int liege_opinion = 0;
		int vassal_opinion = 0;
		int infidel_opinion = 0;
		int opposite_opinion = 0;
		int same_opinion = 0;
		int same_plus_if_same_religion_opinion = 0;
		int twin_opinion = 0;
		int ambition_opinion = 0;
		int same_religion_opinion = 0;
		int tribal_opinion = 0;
		int unreformed_tribal_opinion = 0;
		std::unordered_map<const Religion*, int> religion_opinions;
		std::unordered_map<const ReligionGroup*, int> religion_group_opinions;
	};

	struct modifiers
	{
		float diplomacy = 0.0;
		float diplomacy_penalty = 0.0;
		float stewardship = 0.0;
		float stewardship_penalty = 0.0;
		float martial = 0.0;
		float martial_penalty = 0.0;
		float intrigue = 0.0;
		float intrigue_penalty = 0.0;
		float learning = 0.0;
		float learning_penalty = 0.0;
		float fertility = 0.0;
		float fertility_penalty = 0.0;
		float health = 0.0;
		float health_penalty = 0.0;
		float combat_rating = 0.0;
		float threat_decay_speed = 0.0;
		float demesne_size = 0.0;
		float global_revolt_risk = 0.0;
		float culture_flex = 0.0;
		float religion_flex = 0.0;
		float assassinate_chance_modifier = 0.0;
		float arrest_chance_modifier = 0.0;
		float plot_power_modifier = 0.0;
		float tax_income = 0.0;
		float global_tax_modifier = 0.0;
		float local_tax_modifier = 0.0;
		float monthly_character_prestige = 0.0;
		float monthly_character_piety = 0.0;
		float monthly_character_wealth = 0.0;
		float ai_rationality = 0.0;
		float ai_zeal = 0.0;
		float ai_greed = 0.0;
		float ai_honor = 0.0;
		float ai_ambition = 0.0;
		float build_cost_modifier = 0.0;
		float build_time_modifier = 0.0;
		float local_build_cost_modifier = 0.0;
		float local_build_time_modifier = 0.0;
		float vassal_opinion = 0.0;
		float sex_appeal_opinion = 0.0;
		float same_opinion = 0.0;
		float same_opinion_if_same_religion = 0.0;
		float opposite_opinion = 0.0;
		float liege_opinion = 0.0;
		float general_opinion = 0.0;
		float global_levy_size = 0.0;
		float wonder_build_time_modifier = 0.0;
		float wonder_build_cost_modifier = 0.0;
		float twin_opinion = 0.0;
		float dynasty_opinion = 0.0;
		float spouse_opinion = 0.0;
		float days_of_supply = 0.0;
		float attrition = 0.0;
		//float <religion>_opinion = 0.0;
		//float <religion_group>_opinion = 0.0;
		float same_religion_opinion = 0.0;
		float infidel_opinion = 0.0;
		//float <religion_group>_church_opinion = 0.0;
		float church_opinion = 0.0;
		float town_opinion = 0.0;
		float tribal_opinion = 0.0;
		float unreformed_tribal_opinion = 0.0;
		float levy_size = 0.0;
		float levy_reinforce_rate = 0.0;
		float land_morale = 0.0;
		float global_supply_limit = 0.0;
		float supply_limit = 0.0;
		float max_attrition = 0.0;
		float siege_speed = 0.0;
		float tradevalue = 0.0;
		float max_manpower_mult = 0.0;
		float murder_plot_power_modifier = 0.0;
		float defensive_plot_power_modifier = 0.0;
		float build_cost_temple_modifier = 0.0;
		float local_build_cost_temple_modifier = 0.0;
	};

	struct Commandmodifiers
	{
		float cavalry = 0;
		float center = 0;
		float damage = 0;
		float defence = 0;
		float flank = 0;
		float morale_defence = 0;
		float morale_offence = 0;
		float narrow_flank = 0;
		float pursue = 0;
		float random = 0;
		float religious_enemy = 0;
		float retreat = 0;
		float siege = 0;
		float speed = 0;
		float winter_combat = 0;
		float winter_supply = 0;

		std::unordered_map<const UnitType*, float> unit_specific_buffs;
		std::unordered_set<const Terrain*> terraine_specific_buffs;
		std::unordered_map<const Phase*, float> phase_specific_buffs;
	};

	std::unordered_set<const ReligionGroup*> tolerated_religion_groups;

	Flags flags;
	Attributes attribute_modifiers;
	modifiers modifiers;
	Commandmodifiers command_modifiers;
	Stats stat_modifiers;
	Greeting adjectives;
	Opinon opinion;

	//std::optional<openck::scripting::ConditionBlock> condition_block;

	//Trait(const Node& trait_node, bool& was_success);
	//Trait& operator=(Trait&& trait);


	Trait(std::string name);

	//Trait(Trait&&) = delete;
	//Trait& operator=(Trait&&) = delete;

	StatusCode set_potential(const Node& node);

	StatusCode set_is_visible(const Node &node);

	StatusCode set_attribute(const Node& node);

	StatusCode set_martial_modifier(const Node& node);

	StatusCode set_diplomacy_modifier(const Node& node);

	StatusCode set_stewardship_modifier(const Node& node);

	StatusCode set_intrigue_modifier(const Node& node);

	StatusCode set_learning_modifier(const Node& node);

	StatusCode set_greeting(const Node &node, const Greeting::Target target, const Greeting::Type type);

	StatusCode set_greeting_adjective(const Node& node, const Greeting::Target target, const Greeting::Type type);

	StatusCode set_attribute_modifer(const Node& node, const AttributesType attribute_type);

	StatusCode set_attribute_penalty(const Node& node, const AttributesType attribute_type);

	StatusCode set_stat_modifer(const Node& node, const StatType stat_type);

	StatusCode set_stat_penalty(const Node &node, const StatType stat_type);

	StatusCode set_opinion_modifer(const Node& node, const Opinon::From from);

	StatusCode set_opposites(const Node& node);

	StatusCode set_command_modifier(const Node& node);

	StatusCode set_opinion_modifer_dynamic(const Node& node);

	StatusCode set_tolerance(const Node & node);

	DynamicFieldType get_dynamic_field_type(const Node& node);

	StatusCode set_dynamic_field(const Node& node, DynamicFieldType type);

	static void init_field_setters();
	static void init_static_objects() {};
};

} // namespace openck::simulator
