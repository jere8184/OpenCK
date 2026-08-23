
#include "Trait.hpp"
#include "Army.hpp"
#include "Map.hpp"
#include "Scripting/Condition.hpp"

#include <concepts>
#include <functional>
#include <ranges>
#include <print>

namespace openck::simulator
{

using openck::parser::Node;

Trait::Trait(std::string name) : Base(name)
{};

StatusCode Trait::set_potential(const Node &node)
{
	conditionBlock = scripting::ConditionBlock(scripting::ConditionBlock::AnyScope::Opcode::CHARECTER_SCOPE);
	return conditionBlock->Compile<scripting::ConditionBlock::CharacterScope>(node);
}

StatusCode Trait::set_is_visible(const Node &node)
{
	scripting::ConditionBlock block(scripting::ConditionBlock::AnyScope::Opcode::CHARECTER_SCOPE);
	return block.Compile<scripting::ConditionBlock::CharacterScope>(node);
}

StatusCode Trait::set_attribute(const Node& node)
{
	if (node.value == "intrigue")
		this->flags.attribute = AttributesType::INTRIGUE;
	else if (node.value == "martia")
		this->flags.attribute = AttributesType::MARTIAL;
	else if (node.value == "learning")
		this->flags.attribute = AttributesType::LEARNING;
	else if (node.value == "diplomacy")
		this->flags.attribute = AttributesType::DIPLOMACY;
	else if (node.value == "stewardship")
		this->flags.attribute = AttributesType::STEWARDSHIP;
	else
		return StatusCode::NOT_FOUND;
	return StatusCode::SUCCESS;
}

StatusCode Trait::set_attribute_modifer(const Node& node, const AttributesType attribute_type)
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
			return StatusCode::NOT_FOUND;
			break;
	}
	return StatusCode::SUCCESS;
}

		/*if (this->set_opinion_modifer(child_node, Opinon::From::DYNAMIC))
			continue;
		else
			return false;*/

StatusCode Trait::set_stat_modifer(const Node &node, const StatType stat_type)
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
		return StatusCode::NOT_FOUND;
		break;
	}
}

StatusCode Trait::set_attribute_penalty(const Node& node, const AttributesType attribute_type)
{
	switch (attribute_type)
	{
		case AttributesType::DIPLOMACY:
			return node.get_value(this->attribute_modifiers.diplomacy_penalty);
			break;

		case AttributesType::STEWARDSHIP:
			return node.get_value(this->attribute_modifiers.stewardship_penalty);
			break;

		case AttributesType::INTRIGUE:
			return node.get_value(this->attribute_modifiers.intrigue_penalty);
			break;

		case AttributesType::LEARNING:
			return node.get_value(this->attribute_modifiers.learning_penalty);
			break;

		case AttributesType::MARTIAL:
			return node.get_value(this->attribute_modifiers.martial_penalty);
			break;

		default:
			return StatusCode::NOT_HANDLED;
			break;
	}
}

StatusCode Trait::set_stat_penalty(const Node &node, const StatType stat_type)
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
		return StatusCode::NOT_FOUND;
		break;
	}

}

StatusCode Trait::set_opinion_modifer(const Node &node, const Opinon::From from)
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
		return StatusCode::NOT_FOUND;
		break;
	}
}

StatusCode Trait::set_opposites(const Node &node)
{
	for (const Node& child_node : node.children)
	{
		const Trait* opposite_trait;
		RETURN_RESULT_IF(StatusCode::SUCCESS, !=, this->get_by_name(opposite_trait, child_node.name));
		this->flags.opposites.push_back(opposite_trait);
	}
	return StatusCode::SUCCESS;
}

StatusCode Trait::set_command_modifier(const Node &node)
{
	for (const Node& child : node.children)
	{
		if(child.name == "terrain")
		{
			const Terrain* terrain;
			RETURN_RESULT_IF(StatusCode::SUCCESS, !=, Terrain::get_by_name(terrain, child.value), child);
			this->command_modifiers.terraine_specific_buffs.insert(terrain);
			return StatusCode::SUCCESS;
		}
		else
		{
			float val;
			RETURN_RESULT_IF(StatusCode::SUCCESS, !=, child.get_value(val), child);
			std::string setter_key = ("command_modifier.") + child.name;

			if (field_setters.contains(setter_key))
			{
				RETURN_RESULT_IF(StatusCode::SUCCESS, !=, Trait::field_setters.at(setter_key)(this, child), child);
			}
			else if (const UnitType* unit_type; StatusCode::SUCCESS == UnitType::get_by_name(unit_type, child.name))
			{
				this->command_modifiers.unit_specific_buffs[unit_type] = val;
			}
			else
			{
				return StatusCode::FAILURE;
			}
		}
	}
	return StatusCode::SUCCESS;
}

StatusCode Trait::set_opinion_modifer_dynamic(const Node& node)
{
	int val;
	RETURN_RESULT_IF(StatusCode::SUCCESS, !=, node.get_value(val));

	std::string name = node.name.substr(0, node.name.size() - std::string_view("_opinion").size());

	const Religion* religion;
	const ReligionGroup* religionGroup;
	StatusCode statusCode = Religion::get_by_name(religion, name);
	if (StatusCode::SUCCESS == statusCode)
	{
		this->opinion.religion_opinions[religion] = val;
		return StatusCode::SUCCESS;
	}
	else if (StatusCode::SUCCESS == ReligionGroup::get_by_name(religionGroup, name))
	{
		this->opinion.religion_group_opinions[religionGroup] = val;
		return StatusCode::SUCCESS;
	}
	else
	{
		return StatusCode::FAILURE;
	}
}

const StatusCode get_localisation(const std::string* localisation, const std::string& key) {static std::string dummy_val(""); localisation = &dummy_val; return StatusCode::SUCCESS; /*Dummy function*/}

StatusCode Trait::set_greeting_adjective(const Node& node, const Greeting::Target target, const Greeting::Type type)
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
				return StatusCode::NOT_FOUND;
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
				return StatusCode::NOT_FOUND;
				break;
			}
			break;

		default :
			return StatusCode::NOT_FOUND;
			break;
	}
	return get_localisation(*value_ptr, node.value);
}

StatusCode Trait::set_greeting(const Node& node, const Greeting::Target target, const Greeting::Type type)
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
				return StatusCode::NOT_FOUND;
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
				return StatusCode::NOT_FOUND;
				break;
			}
			break;

		default :
			return StatusCode::NOT_FOUND;
			break;
		}
		return get_localisation(*value_ptr, node.value);
}

StatusCode Trait::set_tolerance(const Node& node)
{
	const ReligionGroup* religion_group;
	RETURN_RESULT_IF(StatusCode::SUCCESS, !=, ReligionGroup::get_by_name(religion_group, node.name.substr(std::string_view("tolerates_").size())));
	tolerated_religion_groups.insert(religion_group);
	return StatusCode::SUCCESS;

}

template<>
enum struct Base<Trait>::DynamicFieldType
{
	NOT_SET,
	OPINION,
	TOLERANCE
};

Base<Trait>::DynamicFieldType Trait::get_dynamic_field_type(const Node &node)
{
	int index = node.name.find("_opinion");
	if ((index != std::string::npos) && (node.name.size() == index + std::string("_opinion").size()))
	{
		return DynamicFieldType::OPINION;
	}

	index = node.name.find("tolerates_");
	if (index == 0)
	{
		return DynamicFieldType::TOLERANCE;
	}

	return DynamicFieldType::NOT_SET;
}

StatusCode Trait::set_dynamic_field(const Node& node, DynamicFieldType type)
{
	switch (type)
	{
	case DynamicFieldType::OPINION :
		return this->set_opinion_modifer(node, Opinon::From::DYNAMIC);
		break;

	case DynamicFieldType::TOLERANCE :
		return this->set_tolerance(node);

	default:
		return StatusCode::NOT_HANDLED;
		break;
	}
}

void Trait::init_field_setters()
{
	field_setters =
	{
		{"attribute", &Trait::set_attribute},
		{"martia", std::bind(&Trait::set_attribute_modifer, std::placeholders::_1, std::placeholders::_2, AttributesType::MARTIAL)},
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

		{"male_insult_adj", std::bind(&Trait::set_greeting_adjective, std::placeholders::_1, std::placeholders::_2, Trait::Greeting::Target::MALE, Trait::Greeting::Type::INSULT)},
		{"female_insult_adj", std::bind(&Trait::set_greeting_adjective, std::placeholders::_1, std::placeholders::_2, Trait::Greeting::Target::FEMALE, Trait::Greeting::Type::INSULT)},
		{"child_insult_adj", std::bind(&Trait::set_greeting_adjective, std::placeholders::_1, std::placeholders::_2, Trait::Greeting::Target::CHILD, Trait::Greeting::Type::INSULT)},
		{"male_compliment_adj", std::bind(&Trait::set_greeting_adjective, std::placeholders::_1, std::placeholders::_2, Trait::Greeting::Target::MALE, Trait::Greeting::Type::COMPLIMENT)},
		{"female_compliment_adj", std::bind(&Trait::set_greeting_adjective, std::placeholders::_1, std::placeholders::_2, Trait::Greeting::Target::FEMALE, Trait::Greeting::Type::COMPLIMENT)},
		{"child_compliment_adj", std::bind(&Trait::set_greeting_adjective, std::placeholders::_1, std::placeholders::_2, Trait::Greeting::Target::CHILD, Trait::Greeting::Type::COMPLIMENT)},

//		{"male_insult", std::bind(&Trait::set_greeting, std::placeholders::_1, std::placeholders::_2, Trait::Greeting::Target::MALE, Trait::Greeting::Type::INSULT)},
		{"female_insult", std::bind(&Trait::set_greeting, std::placeholders::_1, std::placeholders::_2, Trait::Greeting::Target::FEMALE, Trait::Greeting::Type::INSULT)},
		{"child_insult", std::bind(&Trait::set_greeting, std::placeholders::_1, std::placeholders::_2, Trait::Greeting::Target::CHILD, Trait::Greeting::Type::INSULT)},
		{"male_compliment", std::bind(&Trait::set_greeting, std::placeholders::_1, std::placeholders::_2, Trait::Greeting::Target::MALE, Trait::Greeting::Type::COMPLIMENT)},
		{"female_compliment", std::bind(&Trait::set_greeting, std::placeholders::_1, std::placeholders::_2, Trait::Greeting::Target::FEMALE, Trait::Greeting::Type::COMPLIMENT)},
		{"child_compliment", std::bind(&Trait::set_greeting, std::placeholders::_1, std::placeholders::_2, Trait::Greeting::Target::CHILD, Trait::Greeting::Type::COMPLIMENT)},

		{"fertility", std::bind(&Trait::set_stat_modifer, std::placeholders::_1, std::placeholders::_2, StatType::FERTILITY)},
		{"health", std::bind(&Trait::set_stat_modifer, std::placeholders::_1, std::placeholders::_2, StatType::HEALTH)},
		{"fertility_penalty", std::bind(&Trait::set_stat_penalty, std::placeholders::_1, std::placeholders::_2, StatType::FERTILITY)},
		{"health_penalty", std::bind(&Trait::set_stat_penalty, std::placeholders::_1, std::placeholders::_2, StatType::HEALTH)},

		{"ai_zea", std::bind(&Trait::set_stat_modifer, std::placeholders::_1, std::placeholders::_2, StatType::ZEAL)},
		{"ai_greed", std::bind(&Trait::set_stat_modifer, std::placeholders::_1, std::placeholders::_2, StatType::GREED)},
		{"ai_ambition", std::bind(&Trait::set_stat_modifer, std::placeholders::_1, std::placeholders::_2, StatType::AMBITION)},
		{"ai_rationality", std::bind(&Trait::set_stat_modifer, std::placeholders::_1, std::placeholders::_2, StatType::RATIONALITY)},
		{"ai_honor", std::bind(&Trait::set_stat_modifer, std::placeholders::_1, std::placeholders::_2, StatType::HONOR)},

		{"general_opinion", std::bind(&Trait::set_opinion_modifer, std::placeholders::_1, std::placeholders::_2, Trait::Opinon::From::GENERAL)},
		{"church_opinion", std::bind(&Trait::set_opinion_modifer, std::placeholders::_1, std::placeholders::_2, Trait::Opinon::From::CHURCH)},
		{"christian_church_opinion", std::bind(&Trait::set_opinion_modifer, std::placeholders::_1, std::placeholders::_2, Trait::Opinon::From::CHRISTIAN_CHURCH)},
		{"spouse_opinion", std::bind(&Trait::set_opinion_modifer, std::placeholders::_1, std::placeholders::_2, Trait::Opinon::From::SPOUSE)},
		{"sex_appeal_opinion", std::bind(&Trait::set_opinion_modifer, std::placeholders::_1, std::placeholders::_2, Trait::Opinon::From::SEX_APPEAL)},
		{"dynasty_opinion", std::bind(&Trait::set_opinion_modifer, std::placeholders::_1, std::placeholders::_2, Trait::Opinon::From::DYNASTY)},
		{"liege_opinion", std::bind(&Trait::set_opinion_modifer, std::placeholders::_1, std::placeholders::_2, Trait::Opinon::From::LIEGE)},
		{"vassal_opinion", std::bind(&Trait::set_opinion_modifer, std::placeholders::_1, std::placeholders::_2, Trait::Opinon::From::VASSAL)},
		{"infidel_opinion", std::bind(&Trait::set_opinion_modifer, std::placeholders::_1, std::placeholders::_2, Trait::Opinon::From::INFIDEL)},
		{"opposite_opinion", std::bind(&Trait::set_opinion_modifer, std::placeholders::_1, std::placeholders::_2, Trait::Opinon::From::OPPOSITE)},
		{"same_opinion", std::bind(&Trait::set_opinion_modifer, std::placeholders::_1, std::placeholders::_2, Trait::Opinon::From::SAME)},
		{"same_opinion_if_same_religion", std::bind(&Trait::set_opinion_modifer, std::placeholders::_1, std::placeholders::_2, Trait::Opinon::From::SAME_PLUS_SAME_RELIGION)},
		{"twin_opinion", std::bind(&Trait::set_opinion_modifer, std::placeholders::_1, std::placeholders::_2, Trait::Opinon::From::TWIN)},
		{"ambition_opinion", std::bind(&Trait::set_opinion_modifer, std::placeholders::_1, std::placeholders::_2, Trait::Opinon::From::AMBITION)},
		{"same_religion_opinion", std::bind(&Trait::set_opinion_modifer, std::placeholders::_1, std::placeholders::_2, Trait::Opinon::From::SAME_RELIGION)},
		{"tribal_opinion", std::bind(&Trait::set_opinion_modifer, std::placeholders::_1, std::placeholders::_2, Trait::Opinon::From::UNREFOMED_TRIBAL)},
		{"unreformed_tribal_opinion", std::bind(&Trait::set_opinion_modifer, std::placeholders::_1, std::placeholders::_2, Trait::Opinon::From::UNREFOMED_TRIBAL)},

		{"opposites", &Trait::set_opposites},

		{"education", [](Trait* trait, const Node& node){return node.get_value(trait->flags.education);}},
		{"leadership_traits", [](Trait* trait, const Node& node){return node.get_value(trait->flags.leadership_traits);}},
		{"customizer", [](Trait* trait, const Node& node){return node.get_value(trait->flags.customizer);}},
		{"religious", [](Trait* trait, const Node& node){return node.get_value(trait->flags.religious);}},
		{"is_health", [](Trait* trait, const Node& node){return node.get_value(trait->flags.is_health);}},
		{"is_illness", [](Trait* trait, const Node& node){return node.get_value(trait->flags.is_illness);}},
		{"is_symptom", [](Trait* trait, const Node& node){return node.get_value(trait->flags.is_symptom);}},
		{"is_epidemic", [](Trait* trait, const Node& node){return node.get_value(trait->flags.is_epidemic);}},
		{"inherit_chance", [](Trait* trait, const Node& node){return node.get_value(trait->flags.inherit_chance);}},
		{"succession_gfx", [](Trait* trait, const Node& node){return node.get_value(trait->flags.succession_gfx);}},
		{"incapacitating", [](Trait* trait, const Node& node){return node.get_value(trait->flags.incapacitating);}},
		{"ruler_designer_cost", [](Trait* trait, const Node& node){return node.get_value(trait->flags.ruler_designer_cost);}},
		{"birth", [](Trait* trait, const Node& node){return node.get_value(trait->flags.chance_on_birth);}},
		{"both_parent_has_trait_inherit_chance", [](Trait* trait, const Node& node){return node.get_value(trait->flags.both_parent_has_trait_inherit_chance);}},
		{"inbred", [](Trait* trait, const Node& node){return node.get_value(trait->flags.inbred);}},
		{"hidden", [](Trait* trait, const Node& node){return node.get_value(trait->flags.hidden);}},
		{"immorta", [](Trait* trait, const Node& node){return node.get_value(trait->flags.immortal);}},
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
		{"cached", [](Trait* trait, const Node& node){return node.get_value(trait->flags.cached);}},
		{"blinding", [](Trait* trait, const Node& node){return node.get_value(trait->flags.blinding);}},
		{"rebel_inherited", [](Trait* trait, const Node& node){return node.get_value(trait->flags.rebel_inherited);}},
		{"caste_tier", [](Trait* trait, const Node& node){return node.get_value(trait->flags.caste_tier);}},
		{"in_hiding", [](Trait* trait, const Node& node){return node.get_value(trait->flags.in_hiding);}},
		{"can_hold_titles", [](Trait* trait, const Node& node){return node.get_value(trait->flags.can_hold_titles);}},
		{"hidden_from_others", [](Trait* trait, const Node& node){return node.get_value(trait->flags.hidden_from_others);}},

		{"religious_branch", [](Trait* trait, const Node& node){return simulator::Religion::get_by_name(trait->flags.religious_branch, node.value);}},

		// modifiers
		{"diplomacy", [](Trait* trait, const Node& node){return node.get_value(trait->modifiers.diplomacy);}},
		{"diplomacy_penalty", [](Trait* trait, const Node& node){return node.get_value(trait->modifiers.diplomacy_penalty);}},
		{"stewardship", [](Trait* trait, const Node& node){return node.get_value(trait->modifiers.stewardship);}},
		{"stewardship_penalty", [](Trait* trait, const Node& node){return node.get_value(trait->modifiers.stewardship_penalty);}},
		{"martia", [](Trait* trait, const Node& node){return node.get_value(trait->modifiers.martial);}},
		{"martial_penalty", [](Trait* trait, const Node& node){return node.get_value(trait->modifiers.martial_penalty);}},
		{"intrigue", [](Trait* trait, const Node& node){return node.get_value(trait->modifiers.intrigue);}},
		{"intrigue_penalty", [](Trait* trait, const Node& node){return node.get_value(trait->modifiers.intrigue_penalty);}},
		{"learning", [](Trait* trait, const Node& node){return node.get_value(trait->modifiers.learning);}},
		{"learning_penalty", [](Trait* trait, const Node& node){return node.get_value(trait->modifiers.learning_penalty);}},
		{"fertility", [](Trait* trait, const Node& node){return node.get_value(trait->modifiers.fertility);}},
		{"fertility_penalty", [](Trait* trait, const Node& node){return node.get_value(trait->modifiers.fertility_penalty);}},
		{"health", [](Trait* trait, const Node& node){return node.get_value(trait->modifiers.health);}},
		{"health_penalty", [](Trait* trait, const Node& node){return node.get_value(trait->modifiers.health_penalty);}},
		{"combat_rating", [](Trait* trait, const Node& node){return node.get_value(trait->modifiers.combat_rating);}},
		{"threat_decay_speed", [](Trait* trait, const Node& node){return node.get_value(trait->modifiers.threat_decay_speed);}},
		{"demesne_size", [](Trait* trait, const Node& node){return node.get_value(trait->modifiers.demesne_size);}},
		{"global_revolt_risk", [](Trait* trait, const Node& node){return node.get_value(trait->modifiers.global_revolt_risk);}},
		{"culture_flex", [](Trait* trait, const Node& node){return node.get_value(trait->modifiers.culture_flex);}},
		{"religion_flex", [](Trait* trait, const Node& node){return node.get_value(trait->modifiers.religion_flex);}},
		{"assassinate_chance_modifier", [](Trait* trait, const Node& node){return node.get_value(trait->modifiers.assassinate_chance_modifier);}},
		{"arrest_chance_modifier", [](Trait* trait, const Node& node){return node.get_value(trait->modifiers.arrest_chance_modifier);}},
		{"plot_power_modifier", [](Trait* trait, const Node& node){return node.get_value(trait->modifiers.plot_power_modifier);}},
		{"tax_income", [](Trait* trait, const Node& node){return node.get_value(trait->modifiers.tax_income);}},
		{"global_tax_modifier", [](Trait* trait, const Node& node){return node.get_value(trait->modifiers.global_tax_modifier);}},
		{"global_levy_size", [](Trait* trait, const Node& node){return node.get_value(trait->modifiers.global_tax_modifier);}},
		{"wonder_build_time_modifier", [](Trait* trait, const Node& node){return node.get_value(trait->modifiers.wonder_build_time_modifier);}},
		{"wonder_build_cost_modifier", [](Trait* trait, const Node& node){return node.get_value(trait->modifiers.wonder_build_cost_modifier);}},
		{"days_of_supply", [](Trait* trait, const Node& node){return node.get_value(trait->modifiers.days_of_supply);}},
		{"attrition", [](Trait* trait, const Node& node){return node.get_value(trait->modifiers.attrition);}},
		{"local_tax_modifier", [](Trait* trait, const Node& node){return node.get_value(trait->modifiers.local_tax_modifier);}},
		{"monthly_character_prestige", [](Trait* trait, const Node& node){return node.get_value(trait->modifiers.monthly_character_prestige);}},
		{"monthly_character_piety", [](Trait* trait, const Node& node){return node.get_value(trait->modifiers.monthly_character_piety);}},
		{"monthly_character_wealth", [](Trait* trait, const Node& node){return node.get_value(trait->modifiers.monthly_character_wealth);}},
		{"monthly_grace", [](Trait* trait, const Node& node){return node.get_value(trait->modifiers.monthly_grace);}},
		{"ai_rationality", [](Trait* trait, const Node& node){return node.get_value(trait->modifiers.ai_rationality);}},
		{"ai_zea", [](Trait* trait, const Node& node){return node.get_value(trait->modifiers.ai_zeal);}},
		{"ai_greed", [](Trait* trait, const Node& node){return node.get_value(trait->modifiers.ai_greed);}},
		{"ai_honor", [](Trait* trait, const Node& node){return node.get_value(trait->modifiers.ai_honor);}},
		{"ai_ambition", [](Trait* trait, const Node& node){return node.get_value(trait->modifiers.ai_ambition);}},
		{"build_cost_modifier", [](Trait* trait, const Node& node){return node.get_value(trait->modifiers.build_cost_modifier);}},
		{"build_time_modifier", [](Trait* trait, const Node& node){return node.get_value(trait->modifiers.build_time_modifier);}},
		{"local_build_cost_modifier", [](Trait* trait, const Node& node){return node.get_value(trait->modifiers.local_build_cost_modifier);}},
		{"local_build_time_modifier", [](Trait* trait, const Node& node){return node.get_value(trait->modifiers.local_build_time_modifier);}},
		{"vassal_opinion", [](Trait* trait, const Node& node){return node.get_value(trait->modifiers.vassal_opinion);}},
		{"sex_appeal_opinion", [](Trait* trait, const Node& node){return node.get_value(trait->modifiers.sex_appeal_opinion);}},
		{"same_opinion", [](Trait* trait, const Node& node){return node.get_value(trait->modifiers.same_opinion);}},
		{"same_opinion_if_same_religion", [](Trait* trait, const Node& node){return node.get_value(trait->modifiers.same_opinion_if_same_religion);}},
		{"opposite_opinion", [](Trait* trait, const Node& node){return node.get_value(trait->modifiers.opposite_opinion);}},
		{"liege_opinion", [](Trait* trait, const Node& node){return node.get_value(trait->modifiers.liege_opinion);}},
		{"general_opinion", [](Trait* trait, const Node& node){return node.get_value(trait->modifiers.general_opinion);}},
		{"twin_opinion", [](Trait* trait, const Node& node){return node.get_value(trait->modifiers.twin_opinion);}},
		{"dynasty_opinion", [](Trait* trait, const Node& node){return node.get_value(trait->modifiers.dynasty_opinion);}},
		{"spouse_opinion", [](Trait* trait, const Node& node){return node.get_value(trait->modifiers.spouse_opinion);}},
		//{"<religion>_opinion", [](Trait* trait, const Node& node){return node.get_value(trait->modifiers.<religion>_opinion);}},
		//{"<religion_group>_opinion", [](Trait* trait, const Node& node){return node.get_value(trait->modifiers.<religion_group>_opinion);}},
		{"same_religion_opinion", [](Trait* trait, const Node& node){return node.get_value(trait->modifiers.same_religion_opinion);}},
		{"infidel_opinion", [](Trait* trait, const Node& node){return node.get_value(trait->modifiers.infidel_opinion);}},
		//{"<religion_group>_church_opinion", [](Trait* trait, const Node& node){return node.get_value(trait->modifiers.<religion_group>_church_opinion);}},
		{"church_opinion", [](Trait* trait, const Node& node){return node.get_value(trait->modifiers.church_opinion);}},
		{"town_opinion", [](Trait* trait, const Node& node){return node.get_value(trait->modifiers.town_opinion);}},
		{"tribal_opinion", [](Trait* trait, const Node& node){return node.get_value(trait->modifiers.tribal_opinion);}},
		{"unreformed_tribal_opinion", [](Trait* trait, const Node& node){return node.get_value(trait->modifiers.unreformed_tribal_opinion);}},
		{"levy_size", [](Trait* trait, const Node& node){return node.get_value(trait->modifiers.levy_size);}},
		{"levy_reinforce_rate", [](Trait* trait, const Node& node){return node.get_value(trait->modifiers.levy_reinforce_rate);}},
		{"land_morale", [](Trait* trait, const Node& node){return node.get_value(trait->modifiers.land_morale);}},
		{"global_supply_limit", [](Trait* trait, const Node& node){return node.get_value(trait->modifiers.global_supply_limit);}},
		{"supply_limit", [](Trait* trait, const Node& node){return node.get_value(trait->modifiers.supply_limit);}},
		{"max_attrition", [](Trait* trait, const Node& node){return node.get_value(trait->modifiers.max_attrition);}},
		{"siege_speed", [](Trait* trait, const Node& node){return node.get_value(trait->modifiers.siege_speed);}},
		{"tradevalue", [](Trait* trait, const Node& node){return node.get_value(trait->modifiers.tradevalue);}},
		{"max_manpower_mult", [](Trait* trait, const Node& node){return node.get_value(trait->modifiers.max_manpower_mult);}},
		{"murder_plot_power_modifier", [](Trait* trait, const Node& node){return node.get_value(trait->modifiers.murder_plot_power_modifier);}},
		{"defensive_plot_power_modifier", [](Trait* trait, const Node& node){return node.get_value(trait->modifiers.defensive_plot_power_modifier);}},
		{"build_cost_temple_modifier", [](Trait* trait, const Node& node){return node.get_value(trait->modifiers.build_cost_temple_modifier);}},
		{"local_build_cost_temple_modifier", [](Trait* trait, const Node& node){return node.get_value(trait->modifiers.local_build_cost_temple_modifier);}},

		{"command_modifier.cavalry", [](Trait* trait, const Node& node) { return node.get_value(trait->command_modifiers.cavalry); }},
		{"command_modifier.center", [](Trait* trait, const Node& node) { return node.get_value(trait->command_modifiers.center); }},
		{"command_modifier.damage", [](Trait* trait, const Node& node) { return node.get_value(trait->command_modifiers.damage); }},
		{"command_modifier.defence", [](Trait* trait, const Node& node) { return node.get_value(trait->command_modifiers.defence); }},
		{"command_modifier.flank", [](Trait* trait, const Node& node) { return node.get_value(trait->command_modifiers.flank); }},
		{"command_modifier.morale_defence", [](Trait* trait, const Node& node) { return node.get_value(trait->command_modifiers.morale_defence); }},
		{"command_modifier.morale_offence", [](Trait* trait, const Node& node) { return node.get_value(trait->command_modifiers.morale_offence); }},
		{"command_modifier.narrow_flank", [](Trait* trait, const Node& node) { return node.get_value(trait->command_modifiers.narrow_flank);}},
		{"command_modifier.pursue", [](Trait* trait, const Node& node) { return node.get_value(trait->command_modifiers.pursue); }},
		{"command_modifier.random", [](Trait* trait, const Node& node) { return node.get_value(trait->command_modifiers.random); }},
		{"command_modifier.religious_enemy", [](Trait* trait, const Node& node) { return node.get_value(trait->command_modifiers.religious_enemy);}},
		{"command_modifier.retreat", [](Trait* trait, const Node& node) { return node.get_value(trait->command_modifiers.retreat); }},
		{"command_modifier.siege", [](Trait* trait, const Node& node) { return node.get_value(trait->command_modifiers.siege); }},
		{"command_modifier.speed", [](Trait* trait, const Node& node) { return node.get_value(trait->command_modifiers.speed); }},
		{"command_modifier.winter_combat", [](Trait* trait, const Node& node) { return node.get_value(trait->command_modifiers.winter_combat); }},
		{"command_modifier.winter_supply", [](Trait* trait, const Node& node) { return node.get_value(trait->command_modifiers.winter_supply); }},
		{"command_modifier", std::bind(&Trait::set_command_modifier, std::placeholders::_1, std::placeholders::_2)},

		{"potential", &Trait::set_potential},
		{"is_visible", &Trait::set_is_visible},
		{"random", [](Trait* trait, const Node& node){return node.get_value(trait->flags.random);}},
	};
}

}