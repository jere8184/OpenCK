
#include "Condition.hpp"

#include "Simulator/Culture.hpp"
#include "Simulator/Religion.hpp"
#include "Simulator/Character.hpp"
#include "Simulator/Date.hpp"
#include "Simulator/Flag.hpp"
#include "Utils/StatusCode.hpp"
#include "Utils/Utils.hpp"


#include <cassert>
#include <exception>
#include <unordered_map>

namespace openck::scripting
{

std::unordered_map<std::string, ConditionBlock::AnyScope::ConditionOpcode> ConditionBlock::AnyScope::s_nameToConditionOpcode =
{
	{"has_dlc", ConditionOpcode::HAS_DLC},
	{"INTERNAL_CHARECTER_SCOPE", ConditionOpcode::CHARECTER_SCOPE}
};

std::unordered_map<ConditionBlock::AnyScope::ConditionOpcode, std::string> ConditionBlock::AnyScope::s_conditionOpcodeToName = Reverse(ConditionBlock::AnyScope::s_nameToConditionOpcode);

std::unordered_map<ConditionBlock::AnyScope::CommandOpcode, std::string> ConditionBlock::AnyScope::s_commandOpcodeToName = {};

std::unordered_map<std::string, ConditionBlock::Control::Opcode> ConditionBlock::Control::s_nameToOpcode =
{
	{"OR", Opcode::OR},
	{"RETURN", Opcode::RETURN},	
	{"AND", Opcode::AND},
	{"NOR", Opcode::NOR},
	{"NAND", Opcode::NAND},
	{"NOT", Opcode::NOT},
	{"ROOT", Opcode::LOAD_ROOT},
	{"FROM", Opcode::LOAD_FROM},
	{"LOAD_TRUE", Opcode::LOAD_TRUE},
	{"LOAD_FALSE", Opcode::LOAD_FALSE},
	{"LOAD_POINTER", Opcode::LOAD_POINTER},
	{"LOAD_STRING", Opcode::LOAD_STRING},
	{"LOAD_NUMBER", Opcode::LOAD_NUMBER},
	{"LOAD_RELIGION_ID", Opcode::LOAD_RELIGION_ID},
	{"MAX_VALUE", Opcode::MAX_VALUE},
};

std::unordered_map<ConditionBlock::Control::Opcode, std::string> ConditionBlock::Control::s_opcodeToName = Reverse(ConditionBlock::Control::s_nameToOpcode);

std::unordered_map<std::string, ConditionBlock::CharacterScope::ConditionOpcode> ConditionBlock::CharacterScope::s_nameToConditionOpcode
{
	{"controls_religion", ConditionOpcode::CONTROLS_RELIGION},
	{"culture", ConditionOpcode::CULTURE},
	{"culture_group", ConditionOpcode::CULTURE_GROUP},
	{"religion", ConditionOpcode::RELIGION},
	{"religion_group", ConditionOpcode::RELIGION_GROUP},
	{"is_tribal", ConditionOpcode::IS_TRIBAL},
	{"is_theocracy", ConditionOpcode::IS_THEOCRACY},
	{"trait", ConditionOpcode::TRAIT},
	{"is_ruler", ConditionOpcode::IS_RULER},
	{"is_female", ConditionOpcode::IS_FEMALE},
	{"character", ConditionOpcode::CHARACTER},
	{"society_member_of", ConditionOpcode::SOCIETY_MEMBER_OF},
	{"has_religion_feature", ConditionOpcode::HAS_RELIGION_FEATURE},
	{"age", ConditionOpcode::AGE},
	{"any_owned_bloodline",	ConditionOpcode::ANY_OWNED_BLOODLINE},
	{"ai", ConditionOpcode::AI},
	{"prisoner", ConditionOpcode::PRISONER},
	{"race", ConditionOpcode::RACE},
	{"has_character_flag", ConditionOpcode::HAS_FLAG},
	{"has_flag", ConditionOpcode::HAS_FLAG},
};

std::unordered_map<ConditionBlock::CharacterScope::ConditionOpcode, std::string> ConditionBlock::CharacterScope::s_conditionOpcodeToName = Reverse(ConditionBlock::CharacterScope::s_nameToConditionOpcode);

std::unordered_map<std::string, ConditionBlock::CharacterScope::CommandOpcode> ConditionBlock::CharacterScope::s_nameToCommandOpcode
{
	{"abdicate", CommandOpcode::ABDICATE},
	{"abdicate_to", CommandOpcode::ABDICATE_TO},
	{"abdicate_to_most_liked_by", CommandOpcode::ABDICATE_TO_MOST_LIKED_BY},
	{"activate_plot", CommandOpcode::ACTIVATE_PLOT},
	{"add_age", CommandOpcode::ADD_AGE},
	{"add_alliance", CommandOpcode::ADD_ALLIANCE},
	{"add_ambition", CommandOpcode::ADD_AMBITION},
	{"add_artifact", CommandOpcode::ADD_ARTIFACT},
	{"add_betrothal", CommandOpcode::ADD_BETROTHAL},
	{"add_character_modifier", CommandOpcode::ADD_CHARACTER_MODIFIER},
	{"add_consort", CommandOpcode::ADD_CONSORT},
	{"add_dynasty_modifier", CommandOpcode::ADD_DYNASTY_MODIFIER},
	{"add_favor", CommandOpcode::ADD_FAVOR},
	{"add_friend", CommandOpcode::ADD_FRIEND},
	{"add_lover", CommandOpcode::ADD_LOVER},
	{"add_offmap_currency", CommandOpcode::ADD_OFFMAP_CURRENCY},
	{"add_population_scaled", CommandOpcode::ADD_POPULATION_SCALED},
	{"add_random_education_trait", CommandOpcode::ADD_RANDOM_EDUCATION_TRAIT},
	{"add_rival", CommandOpcode::ADD_RIVAL},
	{"add_society_modifier", CommandOpcode::ADD_SOCIETY_MODIFIER},
	{"add_special_interest", CommandOpcode::ADD_SPECIAL_INTEREST},
	{"add_spouse", CommandOpcode::ADD_SPOUSE},
	{"add_spouse_matrilineal", CommandOpcode::ADD_SPOUSE_MATRILINEAL},
	{"add_to_bloodline", CommandOpcode::ADD_TO_BLOODLINE},
	{"add_trait", CommandOpcode::ADD_TRAIT},
	{"ambition_succeeds", CommandOpcode::AMBITION_SUCCEEDS},
	{"back_plot", CommandOpcode::BACK_PLOT},
	{"banish", CommandOpcode::BANISH},
	{"banish_religion", CommandOpcode::BANISH_RELIGION},
	{"become_secret_heretic", CommandOpcode::BECOME_SECRET_HERETIC},
	{"break_alliance", CommandOpcode::BREAK_ALLIANCE},
	{"break_betrothal", CommandOpcode::BREAK_BETROTHAL},
	{"cancel_ambition", CommandOpcode::CANCEL_AMBITION},
	{"cancel_job_action", CommandOpcode::CANCEL_JOB_ACTION},
	{"cancel_objective", CommandOpcode::CANCEL_OBJECTIVE},
	{"cancel_plot", CommandOpcode::CANCEL_PLOT},
	{"cancel_pregnancy", CommandOpcode::CANCEL_PREGNANCY},
	{"capital", CommandOpcode::CAPITAL},
	{"change_diplomacy", CommandOpcode::CHANGE_DIPLOMACY},
	{"change_infamy", CommandOpcode::CHANGE_INFAMY},
	{"change_intrigue", CommandOpcode::CHANGE_INTRIGUE},
	{"change_learning", CommandOpcode::CHANGE_LEARNING},
	{"change_martial", CommandOpcode::CHANGE_MARTIAL},
	{"change_plot_power", CommandOpcode::CHANGE_PLOT_POWER},
	{"change_society_currency", CommandOpcode::CHANGE_SOCIETY_CURRENCY},
	{"change_stewardship", CommandOpcode::CHANGE_STEWARDSHIP},
	{"character_event", CommandOpcode::CHARACTER_EVENT},
	{"chronicle", CommandOpcode::CHRONICLE},
	{"clear_banish_reasons", CommandOpcode::CLEAR_BANISH_REASONS},
	{"clear_education_trait", CommandOpcode::CLEAR_EDUCATION_TRAIT},
	{"clear_execute_reasons", CommandOpcode::CLEAR_EXECUTE_REASONS},
	{"clear_focus", CommandOpcode::CLEAR_FOCUS},
	{"clear_prison_reasons", CommandOpcode::CLEAR_PRISON_REASONS},
	{"clear_secret_religion", CommandOpcode::CLEAR_SECRET_RELIGION},
	{"clear_revoke_reasons", CommandOpcode::CLEAR_REVOKE_REASONS},
	{"clear_wealth", CommandOpcode::CLEAR_WEALTH},
	{"clr_character_flag", CommandOpcode::CLR_CHARACTER_FLAG},
	{"clr_discovered_society", CommandOpcode::CLR_DISCOVERED_SOCIETY},
	{"clr_dynasty_flag", CommandOpcode::CLR_DYNASTY_FLAG},
	{"clr_quest", CommandOpcode::CLR_QUEST},
	{"clr_quest_target", CommandOpcode::CLR_QUEST_TARGET},
	{"convert_to_secret_religion", CommandOpcode::CONVERT_TO_SECRET_RELIGION},
	{"copy_name", CommandOpcode::COPY_NAME},
	{"copy_random_personality_trait", CommandOpcode::COPY_RANDOM_PERSONALITY_TRAIT},
	{"create_bloodline", CommandOpcode::CREATE_BLOODLINE},
	{"create_family_palace", CommandOpcode::CREATE_FAMILY_PALACE},
	{"create_retinue", CommandOpcode::CREATE_RETINUE},
	{"culture_techpoints", CommandOpcode::CULTURE_TECHPOINTS},
	{"cure_illness", CommandOpcode::CURE_ILLNESS},
	{"death", CommandOpcode::DEATH},
	{"decadence", CommandOpcode::DECADENCE},
	{"decline_law", CommandOpcode::DECLINE_LAW},
	{"diplomatic_immunity", CommandOpcode::DIPLOMATIC_IMMUNITY},
	{"diploresponse_event", CommandOpcode::DIPLORESPONSE_EVENT},
	{"disband_event_forces", CommandOpcode::DISBAND_EVENT_FORCES},
	{"dynasty", CommandOpcode::DYNASTY},
	{"economy_techpoints", CommandOpcode::ECONOMY_TECHPOINTS},
	{"embargo", CommandOpcode::EMBARGO},
	{"excommunicate", CommandOpcode::EXCOMMUNICATE},
	{"father_of_unborn_known", CommandOpcode::FATHER_OF_UNBORN_KNOWN},
	{"fertility", CommandOpcode::FERTILITY},
	{"force_host", CommandOpcode::FORCE_HOST},
	{"gain_all_occupied_titles", CommandOpcode::GAIN_ALL_OCCUPIED_TITLES},
	{"gain_settlements_under_title", CommandOpcode::GAIN_SETTLEMENTS_UNDER_TITLE},
	{"give_job_title", CommandOpcode::GIVE_JOB_TITLE},
	{"give_minor_title", CommandOpcode::GIVE_MINOR_TITLE},
	{"give_nickname", CommandOpcode::GIVE_NICKNAME},
	{"health", CommandOpcode::HEALTH},
	{"hold_election", CommandOpcode::HOLD_ELECTION},
	{"impregnate", CommandOpcode::IMPREGNATE},
	{"impregnate_cuckoo", CommandOpcode::IMPREGNATE_CUCKOO},
	{"imprison", CommandOpcode::IMPRISON},
	{"inherit", CommandOpcode::INHERIT},
	{"join_attacker_wars", CommandOpcode::JOIN_ATTACKER_WARS},
	{"join_defender_wars", CommandOpcode::JOIN_DEFENDER_WARS},
	{"join_faction", CommandOpcode::JOIN_FACTION},
	{"join_society", CommandOpcode::JOIN_SOCIETY},
	{"leave_faction", CommandOpcode::LEAVE_FACTION},
	{"leave_plot", CommandOpcode::LEAVE_PLOT},
	{"leave_society", CommandOpcode::LEAVE_SOCIETY},
	{"lift_embargo", CommandOpcode::LIFT_EMBARGO},
	{"letter_event", CommandOpcode::LETTER_EVENT},
	{"long_character_event", CommandOpcode::LONG_CHARACTER_EVENT},
	{"make_primary_spouse", CommandOpcode::MAKE_PRIMARY_SPOUSE},
	{"make_tributary", CommandOpcode::MAKE_TRIBUTARY},
	{"manpower", CommandOpcode::MANPOWER},
	{"military_techpoints", CommandOpcode::MILITARY_TECHPOINTS},
	{"move_character", CommandOpcode::MOVE_CHARACTER},
	{"narrative_event", CommandOpcode::NARRATIVE_EVENT},
	{"objective_succeeds", CommandOpcode::OBJECTIVE_SUCCEEDS},
	{"occupy_minors_of_occupied_settlements", CommandOpcode::OCCUPY_MINORS_OF_OCCUPIED_SETTLEMENTS},
	{"opinion", CommandOpcode::OPINION},
	{"participation_scaled_decadence", CommandOpcode::PARTICIPATION_SCALED_DECADENCE},
	{"participation_scaled_piety", CommandOpcode::PARTICIPATION_SCALED_PIETY},
	{"participation_scaled_prestige", CommandOpcode::PARTICIPATION_SCALED_PRESTIGE},
	{"piety", CommandOpcode::PIETY},
	{"pledge_crusade_defense", CommandOpcode::PLEDGE_CRUSADE_DEFENSE},
	{"pledge_crusade_participation", CommandOpcode::PLEDGE_CRUSADE_PARTICIPATION},
	{"plot_succeeds", CommandOpcode::PLOT_SUCCEEDS},
	{"population", CommandOpcode::POPULATION},
	{"recalc_succession", CommandOpcode::RECALC_SUCCESSION},
	{"remove_character_modifier", CommandOpcode::REMOVE_CHARACTER_MODIFIER},
	{"remove_character_modifiers", CommandOpcode::REMOVE_CHARACTER_MODIFIERS},
	{"remove_consort", CommandOpcode::REMOVE_CONSORT},
	{"remove_dynasty_modifier", CommandOpcode::REMOVE_DYNASTY_MODIFIER},
	{"remove_favor", CommandOpcode::REMOVE_FAVOR},
	{"remove_friend", CommandOpcode::REMOVE_FRIEND},
	{"remove_from_bloodline", CommandOpcode::REMOVE_FROM_BLOODLINE},
	{"remove_guardian", CommandOpcode::REMOVE_GUARDIAN},
	{"remove_lover", CommandOpcode::REMOVE_LOVER},
	{"remove_nickname", CommandOpcode::REMOVE_NICKNAME},
	{"remove_opinion", CommandOpcode::REMOVE_OPINION},
	{"remove_rival", CommandOpcode::REMOVE_RIVAL},
	{"remove_special_character_title", CommandOpcode::REMOVE_SPECIAL_CHARACTER_TITLE},
	{"remove_special_interest", CommandOpcode::REMOVE_SPECIAL_INTEREST},
	{"remove_spouse", CommandOpcode::REMOVE_SPOUSE},
	{"remove_title", CommandOpcode::REMOVE_TITLE},
	{"remove_trait", CommandOpcode::REMOVE_TRAIT},
	{"remove_tributary", CommandOpcode::REMOVE_TRIBUTARY},
	{"reveal_plot", CommandOpcode::REVEAL_PLOT},
	{"reveal_plot_w_message", CommandOpcode::REVEAL_PLOT_W_MESSAGE},
	{"reverse_add_favor", CommandOpcode::REVERSE_ADD_FAVOR},
	{"reverse_back_plot", CommandOpcode::REVERSE_BACK_PLOT},
	{"reverse_banish", CommandOpcode::REVERSE_BANISH},
	{"reverse_imprison", CommandOpcode::REVERSE_IMPRISON},
	{"reverse_leave_plot", CommandOpcode::REVERSE_LEAVE_PLOT},
	{"reverse_opinion", CommandOpcode::REVERSE_OPINION},
	{"reverse_remove_favor", CommandOpcode::REVERSE_REMOVE_FAVOR},
	{"reverse_remove_opinion", CommandOpcode::REVERSE_REMOVE_OPINION},
	{"reverse_set_opinion_levy_raised_days", CommandOpcode::REVERSE_SET_OPINION_LEVY_RAISED_DAYS},
	{"reverse_unsafe_war", CommandOpcode::REVERSE_UNSAFE_WAR},
	{"scaled_piety", CommandOpcode::SCALED_PIETY},
	{"scaled_prestige", CommandOpcode::SCALED_PRESTIGE},
	{"scaled_wealth", CommandOpcode::SCALED_WEALTH},
	{"send_assassin", CommandOpcode::SEND_ASSASSIN},
	{"set_character_flag", CommandOpcode::SET_CHARACTER_FLAG},
	{"set_council_malcontent", CommandOpcode::SET_COUNCIL_MALCONTENT},
	{"set_crusade_beneficiary", CommandOpcode::SET_CRUSADE_BENEFICIARY},
	{"set_crusade_pot_multiplier", CommandOpcode::SET_CRUSADE_POT_MULTIPLIER},
	{"set_discovered_society", CommandOpcode::SET_DISCOVERED_SOCIETY},
	{"set_dynasty_flag", CommandOpcode::SET_DYNASTY_FLAG},
	{"set_dynasty_name", CommandOpcode::SET_DYNASTY_NAME},
	{"set_father", CommandOpcode::SET_FATHER},
	{"set_focus", CommandOpcode::SET_FOCUS},
	{"set_gender", CommandOpcode::SET_GENDER},
	{"set_government_type", CommandOpcode::SET_GOVERNMENT_TYPE},
	{"set_graphical_culture", CommandOpcode::SET_GRAPHICAL_CULTURE},
	{"set_guardian", CommandOpcode::SET_GUARDIAN},
	{"set_immune_to_pruning", CommandOpcode::SET_IMMUNE_TO_PRUNING},
	{"set_interested_society", CommandOpcode::SET_INTERESTED_SOCIETY},
	{"set_job_action", CommandOpcode::SET_JOB_ACTION},
	{"set_looter_hostility_days", CommandOpcode::SET_LOOTER_HOSTILITY_DAYS},
	{"set_mother", CommandOpcode::SET_MOTHER},
	{"set_official_crusade_recipient", CommandOpcode::SET_OFFICIAL_CRUSADE_RECIPIENT},
	{"set_offmap_currency", CommandOpcode::SET_OFFMAP_CURRENCY},
	{"set_opinion_levy_raised_days", CommandOpcode::SET_OPINION_LEVY_RAISED_DAYS},
	{"set_pacified", CommandOpcode::SET_PACIFIED},
	{"set_player_character", CommandOpcode::SET_PLAYER_CHARACTER},
	{"set_quest", CommandOpcode::SET_QUEST},
	{"set_real_father", CommandOpcode::SET_REAL_FATHER},
	{"set_reincarnation", CommandOpcode::SET_REINCARNATION},
	{"set_secret_religion", CommandOpcode::SET_SECRET_RELIGION},
	{"set_society_grandmaster", CommandOpcode::SET_SOCIETY_GRANDMASTER},
	{"set_special_character_title", CommandOpcode::SET_SPECIAL_CHARACTER_TITLE},
	{"set_truce", CommandOpcode::SET_TRUCE},
	{"society_quest_event", CommandOpcode::SOCIETY_QUEST_EVENT},
	{"society_rank_down", CommandOpcode::SOCIETY_RANK_DOWN},
	{"society_rank_up", CommandOpcode::SOCIETY_RANK_UP},
	{"spawn_fleet", CommandOpcode::SPAWN_FLEET},
	{"spawn_unit", CommandOpcode::SPAWN_UNIT},
	{"start_faction", CommandOpcode::START_FACTION},
	{"steal_population_scaled", CommandOpcode::STEAL_POPULATION_SCALED},
	{"subjugate_or_take_under_title", CommandOpcode::SUBJUGATE_OR_TAKE_UNDER_TITLE},
	{"transfer_scaled_wealth", CommandOpcode::TRANSFER_SCALED_WEALTH},
	{"treasury", CommandOpcode::TREASURY},
	{"unit_event", CommandOpcode::UNIT_EVENT},
	{"unsafe_give_minor_title", CommandOpcode::UNSAFE_GIVE_MINOR_TITLE},
	{"unsafe_impregnate", CommandOpcode::UNSAFE_IMPREGNATE},
	{"unsafe_impregnate_cuckoo", CommandOpcode::UNSAFE_IMPREGNATE_CUCKOO},
	{"unsafe_war", CommandOpcode::UNSAFE_WAR},
	{"vassalize_or_take_under_title", CommandOpcode::VASSALIZE_OR_TAKE_UNDER_TITLE},
	{"vassalize_or_take_under_title_destroy_duchies", CommandOpcode::VASSALIZE_OR_TAKE_UNDER_TITLE_DESTROY_DUCHIES},
	{"war", CommandOpcode::WAR},
	{"wealth", CommandOpcode::WEALTH},
	{"destroy_artifact", CommandOpcode::DESTROY_ARTIFACT},
	{"abandon_heresy", CommandOpcode::ABANDON_HERESY},
	{"become_heretic", CommandOpcode::BECOME_HERETIC},
	{"clear_delayed_event", CommandOpcode::CLEAR_DELAYED_EVENT},
	{"create_character", CommandOpcode::CREATE_CHARACTER},
	{"create_random_diplomat", CommandOpcode::CREATE_RANDOM_DIPLOMAT},
	{"create_random_intriguer", CommandOpcode::CREATE_RANDOM_INTRIGUER},
	{"create_random_priest", CommandOpcode::CREATE_RANDOM_PRIEST},
	{"create_random_soldier", CommandOpcode::CREATE_RANDOM_SOLDIER},
	{"create_random_steward", CommandOpcode::CREATE_RANDOM_STEWARD},
	{"religion_authority", CommandOpcode::RELIGION_AUTHORITY},
	{"repeat_event", CommandOpcode::REPEAT_EVENT},
	{"reverse_culture", CommandOpcode::REVERSE_CULTURE},
	{"culture", CommandOpcode::CULTURE},
	{"religion", CommandOpcode::RELIGION},
	{"clear_flags_with_prefix", CommandOpcode::CLEAR_FLAGS_WITH_PREFIX},
	{"prompt_name", CommandOpcode::PROMPT_NAME},
	{"change_variable", CommandOpcode::CHANGE_VARIABLE},
	{"divide_variable", CommandOpcode::DIVIDE_VARIABLE},
	{"export_to_variable", CommandOpcode::EXPORT_TO_VARIABLE},
	{"modulo_variable", CommandOpcode::MODULO_VARIABLE},
	{"multiply_variable", CommandOpcode::MULTIPLY_VARIABLE},
	{"set_variable", CommandOpcode::SET_VARIABLE},
	{"subtract_variable", CommandOpcode::SUBTRACT_VARIABLE},
	{"add_claim", CommandOpcode::ADD_CLAIM},
	{"add_pressed_claim", CommandOpcode::ADD_PRESSED_CLAIM},
	{"add_weak_claim", CommandOpcode::ADD_WEAK_CLAIM},
	{"add_weak_pressed_claim", CommandOpcode::ADD_WEAK_PRESSED_CLAIM},
	{"approve_law", CommandOpcode::APPROVE_LAW},
	{"gain_title_plus_barony_if_unlanded", CommandOpcode::GAIN_TITLE_PLUS_BARONY_IF_UNLANDED},
	{"grant_kingdom_w_adjudication", CommandOpcode::GRANT_KINGDOM_W_ADJUDICATION},
	{"grant_title", CommandOpcode::GRANT_TITLE},
	{"grant_title_no_opinion", CommandOpcode::GRANT_TITLE_NO_OPINION},
	{"prestige", CommandOpcode::PRESTIGE},
	{"remove_claim", CommandOpcode::REMOVE_CLAIM},
	{"reverse_war", CommandOpcode::REVERSE_WAR},
	{"set_defacto_liege", CommandOpcode::SET_DEFACTO_LIEGE},
	{"set_defacto_vassal", CommandOpcode::SET_DEFACTO_VASSAL},
	{"unsafe_religion", CommandOpcode::UNSAFE_RELIGION},
	{"usurp_title", CommandOpcode::USURP_TITLE},
	{"usurp_title_only", CommandOpcode::USURP_TITLE_ONLY},
	{"usurp_title_plus_barony_if_unlanded", CommandOpcode::USURP_TITLE_PLUS_BARONY_IF_UNLANDED},
	{"usurp_title_plus_barony_if_unlanded_and_retain_liege", CommandOpcode::USURP_TITLE_PLUS_BARONY_IF_UNLANDED_AND_RETAIN_LIEGE},
	{"usurp_title_plus_barony_if_unlanded_and_vassals", CommandOpcode::USURP_TITLE_PLUS_BARONY_IF_UNLANDED_AND_VASSALS},
	{"usurp_title_plus_barony_if_unlanded_and_vassals_no_adj", CommandOpcode::USURP_TITLE_PLUS_BARONY_IF_UNLANDED_AND_VASSALS_NO_ADJ},
	{"set_quest_target", CommandOpcode::SET_QUEST_TARGET},
	{"set_name", CommandOpcode::SET_NAME}
};

std::unordered_map<ConditionBlock::CharacterScope::CommandOpcode, std::string> ConditionBlock::CharacterScope::s_commandOpcodeToName;


std::unordered_map<std::string, ConditionBlock::BloodlineScope::ConditionOpcode> ConditionBlock::BloodlineScope::s_nameToConditionOpcode =
{
	{"bloodline", ConditionOpcode::BLOODLINE},
	{"bloodline_is_active_for", ConditionOpcode::BLOODLINE_IS_ACTIVE_FOR},
	{"had_bloodline_flag", ConditionOpcode::HAD_BLOODLINE_FLAG},
	{"has_bloodline_flag", ConditionOpcode::HAS_BLOODLINE_FLAG},
};

std::unordered_map<std::string, ConditionBlock::BloodlineScope::CommandOpcode> ConditionBlock::BloodlineScope::s_nameToCommandOpcode = {};


ConditionBlock::ConditionBlock(ConditionBlock::AnyScope::ConditionOpcode initalScopeOpcode)
{
	AppendInstruction(initalScopeOpcode);
}

StatusCode ConditionBlock::execute()
{
	while(Advance())
	{
		switch (static_cast<AnyScope::ConditionOpcode>(*(this->ip)))
		{
			case AnyScope::ConditionOpcode::CHARECTER_SCOPE:
				//RETURN_RESULT_IF(StatusCode::SUCCESS, !=, ExecuteCharacter());
				break;

			case AnyScope::ConditionOpcode::BLOODLINE_SCOPE:
				
				break;
		}
	}
	return StatusCode::NOT_IMPLIMENTED;
}

bool ConditionBlock::ExecuteCharacter(simulator::Character& charecter)
{
	bool result;

	while (Advance())
	{
		switch (static_cast<CharacterScope::ConditionOpcode>(*(this->ip)))
		{
		case CharacterScope::ConditionOpcode::CONTROLS_RELIGION:
			result = ControlsReligion(charecter);
			break;

		case CharacterScope::ConditionOpcode::RELIGION:
			result = Religion(charecter);
			break;

		case CharacterScope::ConditionOpcode::ANY_OWNED_BLOODLINE:
			//result = AnyOwnedBloodline(charecter);
			break;

		default:
			break;
		}

		switch (static_cast<Control::Opcode>(*(this->ip))) 
		{
			case Control::Opcode::NOT:
			{
				stack.push(BlockType::NOT);
				continue;
			}
			default:
				break;
		}

		// Todo: needs to be switch
		StatusCode statusCode = EvaluateResult(result);
		if (statusCode == StatusCode::SUCCESS)
			return true;
		else if (statusCode == StatusCode::FAILURE)
			return false;
		else if (statusCode == StatusCode::CONTINUE)
			continue;
		else
			throw(std::exception());
	}
	return false;
}

bool ConditionBlock::execute_bloodline()
{
	return false;
}

bool ConditionBlock::Advance()
{
	this->ip++;
	if (this->ip == this->instructions.end())
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool ConditionBlock::ControlsReligion(const simulator::Character& character)
{
	//const simulator::Religion* religion = static_cast<const simulator::Religion*>(this->load_pointer());
	return character.ControlsReligion(); 
}

bool ConditionBlock::Religion(const simulator::Character &character)
{
	const simulator::Religion* religion = static_cast<const simulator::Religion*>(this->load_pointer());
	return religion == character.religion;
}

const void* ConditionBlock::load_pointer()
{
	this->Advance();
	return this->pointers[*(this->ip)];
}

uint8_t ConditionBlock::store_pointer(const void* pointer)
{
	uint8_t indx = this->pointers.size();
	assert(std::numeric_limits<uint8_t>::max() > indx);
	this->pointers.push_back(pointer);
	return indx;
}

StatusCode ConditionBlock::EvaluateResult(bool result)
{
	switch (this->stack.top())
	{
	case BlockType::AND:
		return result ? StatusCode::CONTINUE : StatusCode::FAILURE;
		break;
	case BlockType::OR:
		return result ? StatusCode::SUCCESS : StatusCode::CONTINUE;
		break;
	case BlockType::NOR:
		return result ? StatusCode::FAILURE : StatusCode::CONTINUE;
		break;
	case BlockType::NAND:
		return result ? StatusCode::CONTINUE : StatusCode::SUCCESS;
		break;
	case BlockType::NOT:
		return result ? StatusCode::FAILURE : StatusCode::SUCCESS;
	default:
		return StatusCode::NOT_HANDLED;
		break;
	}
}

template <>
StatusCode ConditionBlock::append_pointer<simulator::Society, ConditionBlock::Control::Opcode::MAX_VALUE>(const Node& node)
{
	return StatusCode::NOT_IMPLIMENTED;
}

template <>
StatusCode ConditionBlock::append_pointer<simulator::Culture, ConditionBlock::Control::Opcode::MAX_VALUE>(const Node& node)
{
	return StatusCode::NOT_IMPLIMENTED;
}

template <>
StatusCode ConditionBlock::append_pointer<simulator::ReligionFeature, ConditionBlock::Control::Opcode::MAX_VALUE>(const Node& node)
{
	return StatusCode::NOT_IMPLIMENTED;
}

template <>
StatusCode ConditionBlock::append_pointer<simulator::Dlc, ConditionBlock::Control::Opcode::MAX_VALUE>(const Node& node)
{
	return StatusCode::NOT_IMPLIMENTED;
}

template <>
StatusCode ConditionBlock::append_pointer<simulator::BloodLine, ConditionBlock::Control::Opcode::MAX_VALUE>(const Node& node)
{
	return StatusCode::NOT_IMPLIMENTED;
}

template <>
StatusCode ConditionBlock::append_id<simulator::Society, ConditionBlock::Control::Opcode::MAX_VALUE>(const Node& node)
{
	return StatusCode::NOT_IMPLIMENTED;
}

template <>
StatusCode ConditionBlock::append_id<simulator::Culture, ConditionBlock::Control::Opcode::MAX_VALUE>(const Node& node)
{
	return StatusCode::NOT_IMPLIMENTED;
}

template <>
StatusCode ConditionBlock::append_id<simulator::CultureGroup, ConditionBlock::Control::Opcode::MAX_VALUE>(const Node& node)
{
	return StatusCode::NOT_IMPLIMENTED;
}

template <>
StatusCode ConditionBlock::append_id<simulator::ReligionFeature, ConditionBlock::Control::Opcode::MAX_VALUE>(const Node& node)
{
	return StatusCode::NOT_IMPLIMENTED;
}

template <>
StatusCode ConditionBlock::append_id<simulator::Dlc, ConditionBlock::Control::Opcode::MAX_VALUE>(const Node& node)
{
	return StatusCode::NOT_IMPLIMENTED;
}

template <>
StatusCode ConditionBlock::append_id<simulator::BloodLine, ConditionBlock::Control::Opcode::MAX_VALUE>(const Node& node)
{
	return StatusCode::NOT_IMPLIMENTED;
}

StatusCode ConditionBlock::compile_had_flag(const Node& node)
{
	const Node* flag;
	RETURN_RESULT_IF(StatusCode::SUCCESS, !=, node.get_child_by_name(flag, "flag"));
	RETURN_RESULT_IF(StatusCode::SUCCESS, !=, append_id<simulator::Flag>(*flag));
	return compile_date(node);
}

template <>
StatusCode ConditionBlock::HandleOpcode<ConditionBlock::CharacterScope>(const Node& node, const ConditionBlock::CharacterScope::ConditionOpcode op)
{
	switch (op)
	{
		case CharacterScope::ConditionOpcode::RELIGION:
		{
			return append_id<simulator::Religion, Control::Opcode::LOAD_RELIGION_ID>(node);
			//others...
			break;
		}
		case CharacterScope::ConditionOpcode::CONTROLS_RELIGION:
		{
			RETURN_RESULT_IF(StatusCode::SUCCESS, ==, append_bool_val(node));
			StatusCode result = append_id<simulator::Religion, Control::Opcode::LOAD_RELIGION_ID>(node);
			RETURN_RESULT_IF(StatusCode::SUCCESS, ==, result);
			break;
		}
		case CharacterScope::ConditionOpcode::RELIGION_GROUP:
		{
			return append_id<simulator::ReligionGroup>(node);
			//others
			break;
		}
		case CharacterScope::ConditionOpcode::HAS_RELIGION_FEATURE:
		{
			return append_id<simulator::ReligionFeature>(node);
			break;
		}
		case CharacterScope::ConditionOpcode::TRAIT:
		{
			return append_id<simulator::Trait>(node);
			break;
		}
		case CharacterScope::ConditionOpcode::SOCIETY_MEMBER_OF:
		{
			return append_id<simulator::Society>(node);
			break;
		}
		case CharacterScope::ConditionOpcode::CULTURE:
		{
			return append_id<simulator::Culture>(node);
			break;
		}
		case CharacterScope::ConditionOpcode::CULTURE_GROUP:
		{
			return append_id<simulator::CultureGroup>(node);
			break;
		}
		case CharacterScope::ConditionOpcode::HAS_FLAG:
		{
			return append_string<false>(node);
			break;
		}
		case CharacterScope::ConditionOpcode::IS_FEMALE:
		case CharacterScope::ConditionOpcode::IS_RULER:
		case CharacterScope::ConditionOpcode::IS_TRIBAL:
		case CharacterScope::ConditionOpcode::IS_THEOCRACY:
		case CharacterScope::ConditionOpcode::AI:
		case CharacterScope::ConditionOpcode::PRISONER:
		{
			return append_bool_val(node);
			break;
		}
		case CharacterScope::ConditionOpcode::CHARACTER:
		{
			RETURN_RESULT_IF(StatusCode::SUCCESS, ==, append_register(node));
			return append_pointer<simulator::Character>(node);
			break;
		}
		case CharacterScope::ConditionOpcode::ANY_OWNED_BLOODLINE:
		{
			AppendInstruction(AnyScope::ConditionOpcode::BLOODLINE_SCOPE);
			return Compile<BloodlineScope>(node);
		}
		case CharacterScope::ConditionOpcode::AGE:
		case CharacterScope::ConditionOpcode::RACE:
		{
			return StatusCode::NOT_IMPLIMENTED;
			break;
		}
		default:
		{
			return StatusCode::NOT_HANDLED;
			break;
		}
	}

	return StatusCode::FAILURE;
}

template <>
StatusCode ConditionBlock::HandleOpcode<ConditionBlock::BloodlineScope>(const Node& node, const ConditionBlock::BloodlineScope::ConditionOpcode op)
{
	switch (op)
	{
		case BloodlineScope::ConditionOpcode::BLOODLINE:
		{
			RETURN_RESULT_IF(StatusCode::SUCCESS, ==, append_pointer<simulator::BloodLine>(node));
			break;
		}
		case BloodlineScope::ConditionOpcode::BLOODLINE_IS_ACTIVE_FOR:
		{
			RETURN_RESULT_IF(StatusCode::SUCCESS, ==, append_register(node));
			return append_pointer<simulator::Character>(node);
			break;
		}
		case BloodlineScope::ConditionOpcode::HAD_BLOODLINE_FLAG:
		{
			return compile_had_flag(node);
			break;
		}
		case BloodlineScope::ConditionOpcode::HAS_BLOODLINE_FLAG:
		{
			return append_string<false>(node);
			break;
		}
	}

	return StatusCode::FAILURE;
}

StatusCode ConditionBlock::handle_anyscope_opcode(const Node& node, const AnyScope::ConditionOpcode op)
{
	switch (op)
	{
		case AnyScope::ConditionOpcode::HAS_DLC:
		{
			return append_id<simulator::Dlc>(node);
			break;
		}
		default:
		{
			return StatusCode::NOT_HANDLED;
			break;
		}
	}
	return StatusCode::FAILURE;
}

StatusCode ConditionBlock::append_bool_val(const Node& node)
{
	bool condition;
	RETURN_RESULT_IF(StatusCode::SUCCESS, !=, node.GetValue(condition));
	ConditionBlock::Control::Opcode condition_op =  condition ? ConditionBlock::Control::Opcode::LOAD_TRUE : ConditionBlock::Control::Opcode::LOAD_FALSE;
	AppendInstruction(condition_op);
	return StatusCode::SUCCESS;
}

template <typename SimulatorType, ConditionBlock::Control::Opcode LOAD_OP_CODE>
StatusCode ConditionBlock::append_pointer(const Node& node)
{
	const SimulatorType* ptr;
	RETURN_RESULT_IF(StatusCode::SUCCESS, !=, SimulatorType::GetByName(ptr, node.value));
	
	AppendImmediate<LOAD_OP_CODE>(this->pointers, ptr);

	return StatusCode::SUCCESS;
}

template <typename SimulatorType, ConditionBlock::Control::Opcode LOAD_OP_CODE>
StatusCode ConditionBlock::append_id(const Node& node)
{
	const SimulatorType* ptr;
	RETURN_RESULT_IF(StatusCode::SUCCESS, !=, SimulatorType::GetByName(ptr, node.value));
	
	//Appending id as a float?
	AppendImmediate<LOAD_OP_CODE>(this->numbers, ptr->id);

	return StatusCode::SUCCESS;
}

template <typename SimulatorType>
StatusCode ConditionBlock::DecompileId(std::string& output)
{
	Advance();
	SimulatorType* pObj;
	const typename SimulatorType::Id id = numbers.at(*ip);

	RETURN_RESULT_IF(StatusCode::SUCCESS, !=, SimulatorType::GetById(id, pObj));
	
	output += pObj->name;
	output += " || ";

	return StatusCode::SUCCESS;
}

StatusCode ConditionBlock::DecompileBoolVal(std::string& output)
{
	Advance();
	if (Control::Opcode::LOAD_TRUE == static_cast<Control::Opcode>(*ip))
	{
		output += "LOAD_TRUE";
		output += " || ";
		return StatusCode::SUCCESS;
	}
	else if (Control::Opcode::LOAD_FALSE == static_cast<Control::Opcode>(*ip))
	{
		output += "LOAD_FALSE";
		output += " || ";
		return StatusCode::SUCCESS;
	}
	else
	{
		return StatusCode::FAILURE;
	}
}

template <>
StatusCode ConditionBlock::DecompileId<simulator::ReligionFeature>(std::string& output)
{
	return StatusCode::NOT_IMPLIMENTED;
}

template <>
StatusCode ConditionBlock::DecompileId<simulator::Society>(std::string& output)
{
	return StatusCode::NOT_IMPLIMENTED;
}


StatusCode ConditionBlock::append_register(const Node& node)
{
	if ("ROOT" == node.value)
		AppendInstruction(ConditionBlock::Control::Opcode::LOAD_ROOT);
	else if ("FROM" == node.value)
		AppendInstruction(ConditionBlock::Control::Opcode::LOAD_FROM);
	else
		return StatusCode::NOT_FOUND;

	return StatusCode::SUCCESS;
}

template <bool SHOULD_APPEND_LOAD_OPCODE>
StatusCode ConditionBlock::append_string(const Node& node)
{
	if constexpr (SHOULD_APPEND_LOAD_OPCODE)
		AppendImmediate<Control::Opcode::LOAD_STRING>(this->strings, node.value);
	else
		AppendImmediate<Control::Opcode::MAX_VALUE>(this->strings, node.value);
	return StatusCode::SUCCESS;
}

template <bool SHOULD_APPEND_LOAD_OPCODE>
StatusCode ConditionBlock::append_number(const Node& node)
{
	typename decltype(this->numbers)::value_type val;
	RETURN_RESULT_IF(StatusCode::SUCCESS, !=, node.GetValue(val));

	if constexpr (SHOULD_APPEND_LOAD_OPCODE)
		AppendImmediate<Control::Opcode::LOAD_NUMBER>(this->numbers, val);
	else
		AppendImmediate<Control::Opcode::MAX_VALUE>(this->numbers, val);

	return StatusCode::SUCCESS;
}

template <ConditionBlock::Control::Opcode LOAD_OPOCDE>
void ConditionBlock::AppendImmediate(auto& immediate_list, const auto& val)
{
	uint8_t indx;
	auto iter = std::ranges::find(immediate_list, val);
	if (immediate_list.end() != iter)
		indx = std::distance(immediate_list.begin(), iter);
	else
		indx = immediate_list.size();

	assert(std::numeric_limits<typename decltype(this->instructions)::value_type>::max() > indx);

	immediate_list.push_back(val);
	if constexpr(Control::Opcode::MAX_VALUE != LOAD_OPOCDE)
		AppendInstruction(static_cast<uint8_t>(LOAD_OPOCDE));
	AppendInstruction(indx);
}


StatusCode ConditionBlock::compile_date(const Node& node)
{
	const Node* child;
	simulator::Date date;
	bool found = false;
	if (StatusCode::SUCCESS == node.get_child_by_name(child, "days"))
	{
		found = true;
		RETURN_RESULT_IF(StatusCode::SUCCESS, !=, append_number<false>(node));
		return StatusCode::NOT_IMPLIMENTED;
	}
	
	if (StatusCode::SUCCESS == node.get_child_by_name(child, "months"))
	{
		found = true;
		RETURN_RESULT_IF(StatusCode::SUCCESS, !=, append_number<false>(node));
		return StatusCode::NOT_IMPLIMENTED;
	}
	
	if (StatusCode::SUCCESS == node.get_child_by_name(child, "years"))
	{
		found = true;
		RETURN_RESULT_IF(StatusCode::SUCCESS, !=, append_number<false>(node));
		return StatusCode::NOT_IMPLIMENTED;
	}

	return found ? StatusCode::SUCCESS : StatusCode::NOT_FOUND;
}

template <>
StatusCode ConditionBlock::DecompileOpcode<ConditionBlock::CharacterScope::ConditionOpcode>(std::string& output ,const ConditionBlock::CharacterScope::ConditionOpcode op)
{
	switch (op)
	{
		case CharacterScope::ConditionOpcode::RELIGION:
		{
			Advance();
			if (static_cast<Control::Opcode>(*ip) == Control::Opcode::LOAD_RELIGION_ID)
			{
				return DecompileId<simulator::Religion>(output);
			}
			//others...
			break;
		}
		case CharacterScope::ConditionOpcode::CONTROLS_RELIGION:
		{
			RETURN_RESULT_IF(StatusCode::SUCCESS, ==, DecompileBoolVal(output));
			if (static_cast<Control::Opcode>(*ip) == Control::Opcode::LOAD_RELIGION_ID)
				RETURN_RESULT_IF(StatusCode::SUCCESS, ==, DecompileId<simulator::Religion>(output));
			break;
		}
		case CharacterScope::ConditionOpcode::RELIGION_GROUP:
		{
			return DecompileId<simulator::ReligionGroup>(output);
			//others
			break;
		}
		case CharacterScope::ConditionOpcode::HAS_RELIGION_FEATURE:
		{
			return DecompileId<simulator::ReligionFeature>(output);
			break;
		}
		case CharacterScope::ConditionOpcode::TRAIT:
		{
			return DecompileId<simulator::Trait>(output);
			break;
		}
		case CharacterScope::ConditionOpcode::SOCIETY_MEMBER_OF:
		{
			return DecompileId<simulator::Society>(output);
			break;
		}
		case CharacterScope::ConditionOpcode::CULTURE:
		{
			return DecompileId<simulator::Culture>(output);
			break;
		}
		case CharacterScope::ConditionOpcode::HAS_FLAG:
		{
			return DecompileString(output);
			break;
		}
		case CharacterScope::ConditionOpcode::IS_FEMALE:
		case CharacterScope::ConditionOpcode::IS_RULER:
		case CharacterScope::ConditionOpcode::IS_TRIBAL:
		case CharacterScope::ConditionOpcode::IS_THEOCRACY:
		case CharacterScope::ConditionOpcode::AI:
		case CharacterScope::ConditionOpcode::PRISONER:
		{
			return DecompileBoolVal(output);
			break;
		}
		case CharacterScope::ConditionOpcode::CHARACTER:
		{
			//RETURN_RESULT_IF(StatusCode::SUCCESS, ==, decompile_register(output));
			//return decompile_pointer<simulator::Character>(output);
			break;
		}
		case CharacterScope::ConditionOpcode::AGE:
		case CharacterScope::ConditionOpcode::RACE:
		{
			return StatusCode::NOT_IMPLIMENTED;
			break;
		}
		default:
		{
			return StatusCode::NOT_HANDLED;
			break;
		}
	}

	return StatusCode::FAILURE;
}

template <>
StatusCode ConditionBlock::DecompileOpcode<ConditionBlock::AnyScope::ConditionOpcode>(std::string& output ,const ConditionBlock::AnyScope::ConditionOpcode op)
{
	return StatusCode::SUCCESS;
}

StatusCode ConditionBlock::DecompileString(std::string& output)
{
	Advance();
	output += strings.at(*ip);
	output += " || ";

	return StatusCode::SUCCESS;
}


}