
#pragma once

#include "Parser/PrdxParser.hpp"
#include "Simulator/ScriptedTrigger.hpp"

#include "Utils/StatusCode.hpp"

#include <cstdint>
#include <vector>
#include <stack>
#include <cstddef>

namespace openck
{

namespace simulator
{
struct Character;
struct Title;
struct Province;
struct War;
struct Flank;
struct UnitType;
struct Trait;
struct Religion;
struct ReligionFeature;
struct Culture;
struct Society;
struct Artifact;
struct ReligionGroup;
struct BloodLine;
struct Dlc;
}

namespace scripting
{

struct ConditionBlock
{
	using Node = openck::parser::Node;



	struct Control
	{
		enum struct Opcode : uint8_t
		{
			RETURN = 0,
			OR,
			AND,
			NOR,
			NAND,
			NOT,
			LOAD_TRUE,
			LOAD_FALSE,
			LOAD_POINTER,
			LOAD_STRING,
			LOAD_FROM,
			LOAD_ROOT,
			LOAD_NUMBER,
			LOAD_RELIGION_ID,
			MAX_VALUE
		};

		static std::unordered_map<std::string, Opcode> s_nameToOpcode;
		static std::unordered_map<Opcode, std::string> s_opcodeToName;
	};

	enum struct DateOpcode
	{
		LOAD_DAY,
		LOAD_MONTH,
		LOAD_YEAR
	};

	enum struct BlockType : uint8_t
	{
		OR,
		AND,
		NOR,
		NAND,
		NOT
		//CALC_IF_TRUE
	};

	struct AnyScope
	{
		enum struct ConditionOpcode : uint8_t
		{
			HAS_DLC = static_cast<uint8_t>(Control::Opcode::MAX_VALUE),
			CHARECTER_SCOPE,
			BLOODLINE_SCOPE,
			MAX_VALUE,
		};

		enum struct CommandOpcode : uint8_t
		{};

		template <typename T>
		static constexpr ConditionOpcode GetScopeOpcode()
		{
			if constexpr (std::is_same_v<CharacterScope, T>) 
			{
				return  ConditionOpcode::CHARECTER_SCOPE;
			}
			else if (std::is_same_v<BloodlineScope, T>) 
			{
				return  ConditionOpcode::BLOODLINE_SCOPE;
			}
		}

		static std::unordered_map<std::string, ConditionOpcode> s_nameToConditionOpcode;
		static std::unordered_map<ConditionOpcode, std::string> s_conditionOpcodeToName;

		static std::unordered_map<std::string, CommandOpcode> s_nameToCommandOpcode;
		static std::unordered_map<CommandOpcode, std::string> s_commandOpcodeToName;
	};

	struct CharacterScope
	{
		enum struct ConditionOpcode : uint8_t
		{
			CONTROLS_RELIGION = static_cast<uint8_t>(AnyScope::ConditionOpcode::MAX_VALUE),
			RELIGION,
			RELIGION_GROUP,
			CULTURE,
			CULTURE_GROUP,
			IS_TRIBAL,
			TRAIT,
			IS_RULER,
			IS_FEMALE,
			CHARACTER,
			SOCIETY_MEMBER_OF,
			HAS_RELIGION_FEATURE,
			ANY_OWNED_BLOODLINE,
			AGE,
			IS_THEOCRACY,
			AI,
			PRISONER,
			RACE,
			HAS_FLAG,
		};

		enum struct CommandOpcode : uint8_t
		{
			// Commands
			ABDICATE,
			ABDICATE_TO,
			ABDICATE_TO_MOST_LIKED_BY,
			ACTIVATE_PLOT,
			ADD_AGE,
			ADD_ALLIANCE,
			ADD_AMBITION,
			ADD_ARTIFACT,
			ADD_BETROTHAL,
			ADD_CHARACTER_MODIFIER,
			ADD_CONSORT,
			ADD_DYNASTY_MODIFIER,
			ADD_FAVOR,
			ADD_FRIEND,
			ADD_LOVER,
			ADD_OFFMAP_CURRENCY,
			ADD_POPULATION_SCALED,
			ADD_RANDOM_EDUCATION_TRAIT,
			ADD_RIVAL,
			ADD_SOCIETY_MODIFIER,
			ADD_SPECIAL_INTEREST,
			ADD_SPOUSE,
			ADD_SPOUSE_MATRILINEAL,
			ADD_TO_BLOODLINE,
			ADD_TRAIT,
			AMBITION_SUCCEEDS,
			BACK_PLOT,
			BANISH,
			BANISH_RELIGION,
			BECOME_SECRET_HERETIC,
			BREAK_ALLIANCE,
			BREAK_BETROTHAL,
			CANCEL_AMBITION,
			CANCEL_JOB_ACTION,
			CANCEL_OBJECTIVE,
			CANCEL_PLOT,
			CANCEL_PREGNANCY,
			CAPITAL,
			CHANGE_DIPLOMACY,
			CHANGE_INFAMY,
			CHANGE_INTRIGUE,
			CHANGE_LEARNING,
			CHANGE_MARTIAL,
			CHANGE_PLOT_POWER,
			CHANGE_SOCIETY_CURRENCY,
			CHANGE_STEWARDSHIP,
			CHARACTER_EVENT,
			CHRONICLE,
			CLEAR_BANISH_REASONS,
			CLEAR_EDUCATION_TRAIT,
			CLEAR_EXECUTE_REASONS,
			CLEAR_FOCUS,
			CLEAR_PRISON_REASONS,
			CLEAR_SECRET_RELIGION,
			CLEAR_REVOKE_REASONS,
			CLEAR_WEALTH,
			CLR_CHARACTER_FLAG,
			CLR_DISCOVERED_SOCIETY,
			CLR_DYNASTY_FLAG,
			CLR_QUEST,
			CLR_QUEST_TARGET,
			CONVERT_TO_SECRET_RELIGION,
			COPY_NAME,
			COPY_RANDOM_PERSONALITY_TRAIT,
			CREATE_BLOODLINE,
			CREATE_FAMILY_PALACE,
			CREATE_RETINUE,
			CULTURE_TECHPOINTS,
			CURE_ILLNESS,
			DEATH,
			DECADENCE,
			DECLINE_LAW,
			DIPLOMATIC_IMMUNITY,
			DIPLORESPONSE_EVENT,
			DISBAND_EVENT_FORCES,
			DYNASTY,
			ECONOMY_TECHPOINTS,
			EMBARGO,
			EXCOMMUNICATE,
			FATHER_OF_UNBORN_KNOWN,
			FERTILITY,
			FORCE_HOST,
			GAIN_ALL_OCCUPIED_TITLES,
			GAIN_SETTLEMENTS_UNDER_TITLE,
			GIVE_JOB_TITLE,
			GIVE_MINOR_TITLE,
			GIVE_NICKNAME,
			HEALTH,
			HOLD_ELECTION,
			IMPREGNATE,
			IMPREGNATE_CUCKOO,
			IMPRISON,
			INHERIT,
			JOIN_ATTACKER_WARS,
			JOIN_DEFENDER_WARS,
			JOIN_FACTION,
			JOIN_SOCIETY,
			LEAVE_FACTION,
			LEAVE_PLOT,
			LEAVE_SOCIETY,
			LIFT_EMBARGO,
			LETTER_EVENT,
			LONG_CHARACTER_EVENT,
			MAKE_PRIMARY_SPOUSE,
			MAKE_TRIBUTARY,
			MANPOWER,
			MILITARY_TECHPOINTS,
			MOVE_CHARACTER,
			NARRATIVE_EVENT,
			OBJECTIVE_SUCCEEDS,
			OCCUPY_MINORS_OF_OCCUPIED_SETTLEMENTS,
			OPINION,
			PARTICIPATION_SCALED_DECADENCE,
			PARTICIPATION_SCALED_PIETY,
			PARTICIPATION_SCALED_PRESTIGE,
			PIETY,
			PLEDGE_CRUSADE_DEFENSE,
			PLEDGE_CRUSADE_PARTICIPATION,
			PLOT_SUCCEEDS,
			POPULATION,
			RECALC_SUCCESSION,
			REMOVE_CHARACTER_MODIFIER,
			REMOVE_CHARACTER_MODIFIERS,
			REMOVE_CONSORT,
			REMOVE_DYNASTY_MODIFIER,
			REMOVE_FAVOR,
			REMOVE_FRIEND,
			REMOVE_FROM_BLOODLINE,
			REMOVE_GUARDIAN,
			REMOVE_LOVER,
			REMOVE_NICKNAME,
			REMOVE_OPINION,
			REMOVE_RIVAL,
			REMOVE_SPECIAL_CHARACTER_TITLE,
			REMOVE_SPECIAL_INTEREST,
			REMOVE_SPOUSE,
			REMOVE_TITLE,
			REMOVE_TRAIT,
			REMOVE_TRIBUTARY,
			REVEAL_PLOT,
			REVEAL_PLOT_W_MESSAGE,
			REVERSE_ADD_FAVOR,
			REVERSE_BACK_PLOT,
			REVERSE_BANISH,
			REVERSE_IMPRISON,
			REVERSE_LEAVE_PLOT,
			REVERSE_OPINION,
			REVERSE_REMOVE_FAVOR,
			REVERSE_REMOVE_OPINION,
			REVERSE_SET_OPINION_LEVY_RAISED_DAYS,
			REVERSE_UNSAFE_WAR,
			SCALED_PIETY,
			SCALED_PRESTIGE,
			SCALED_WEALTH,
			SEND_ASSASSIN,
			SET_CHARACTER_FLAG,
			SET_COUNCIL_MALCONTENT,
			SET_CRUSADE_BENEFICIARY,
			SET_CRUSADE_POT_MULTIPLIER,
			SET_DISCOVERED_SOCIETY,
			SET_DYNASTY_FLAG,
			SET_DYNASTY_NAME,
			SET_FATHER,
			SET_FOCUS,
			SET_GENDER,
			SET_GOVERNMENT_TYPE,
			SET_GRAPHICAL_CULTURE,
			SET_GUARDIAN,
			SET_IMMUNE_TO_PRUNING,
			SET_INTERESTED_SOCIETY,
			SET_JOB_ACTION,
			SET_LOOTER_HOSTILITY_DAYS,
			SET_MOTHER,
			SET_OFFICIAL_CRUSADE_RECIPIENT,
			SET_OFFMAP_CURRENCY,
			SET_OPINION_LEVY_RAISED_DAYS,
			SET_PACIFIED,
			SET_PLAYER_CHARACTER,
			SET_QUEST,
			SET_REAL_FATHER,
			SET_REINCARNATION,
			SET_SECRET_RELIGION,
			SET_SOCIETY_GRANDMASTER,
			SET_SPECIAL_CHARACTER_TITLE,
			SET_TRUCE,
			SOCIETY_QUEST_EVENT,
			SOCIETY_RANK_DOWN,
			SOCIETY_RANK_UP,
			SPAWN_FLEET,
			SPAWN_UNIT,
			START_FACTION,
			STEAL_POPULATION_SCALED,
			SUBJUGATE_OR_TAKE_UNDER_TITLE,
			TRANSFER_SCALED_WEALTH,
			TREASURY,
			UNIT_EVENT,
			UNSAFE_GIVE_MINOR_TITLE,
			UNSAFE_IMPREGNATE,
			UNSAFE_IMPREGNATE_CUCKOO,
			UNSAFE_WAR,
			VASSALIZE_OR_TAKE_UNDER_TITLE,
			VASSALIZE_OR_TAKE_UNDER_TITLE_DESTROY_DUCHIES,
			WAR,
			WEALTH,
			DESTROY_ARTIFACT,
			ABANDON_HERESY,
			BECOME_HERETIC,
			CLEAR_DELAYED_EVENT,
			CREATE_CHARACTER,
			CREATE_RANDOM_DIPLOMAT,
			CREATE_RANDOM_INTRIGUER,
			CREATE_RANDOM_PRIEST,
			CREATE_RANDOM_SOLDIER,
			CREATE_RANDOM_STEWARD,
			RELIGION_AUTHORITY,
			REPEAT_EVENT,
			REVERSE_CULTURE,
			CULTURE,
			RELIGION,
			CLEAR_FLAGS_WITH_PREFIX,
			PROMPT_NAME,
			CHANGE_VARIABLE,
			DIVIDE_VARIABLE,
			EXPORT_TO_VARIABLE,
			MODULO_VARIABLE,
			MULTIPLY_VARIABLE,
			SET_VARIABLE,
			SUBTRACT_VARIABLE,
			ADD_CLAIM,
			ADD_PRESSED_CLAIM,
			ADD_WEAK_CLAIM,
			ADD_WEAK_PRESSED_CLAIM,
			APPROVE_LAW,
			GAIN_TITLE_PLUS_BARONY_IF_UNLANDED,
			GRANT_KINGDOM_W_ADJUDICATION,
			GRANT_TITLE,
			GRANT_TITLE_NO_OPINION,
			PRESTIGE,
			REMOVE_CLAIM,
			REVERSE_WAR,
			SET_DEFACTO_LIEGE,
			SET_DEFACTO_VASSAL,
			UNSAFE_RELIGION,
			USURP_TITLE,
			USURP_TITLE_ONLY,
			USURP_TITLE_PLUS_BARONY_IF_UNLANDED,
			USURP_TITLE_PLUS_BARONY_IF_UNLANDED_AND_RETAIN_LIEGE,
			USURP_TITLE_PLUS_BARONY_IF_UNLANDED_AND_VASSALS,
			USURP_TITLE_PLUS_BARONY_IF_UNLANDED_AND_VASSALS_NO_ADJ,
			SET_QUEST_TARGET,
			SET_NAME
		};

		static std::unordered_map<std::string, ConditionOpcode> s_nameToConditionOpcode;
		static std::unordered_map<ConditionOpcode, std::string> s_conditionOpcodeToName;

		static std::unordered_map<std::string, CommandOpcode> s_nameToCommandOpcode;
		static std::unordered_map<CommandOpcode, std::string> s_commandOpcodeToName;
	};

	struct BloodlineScope
	{
		enum struct ConditionOpcode : uint8_t
		{
			BLOODLINE = static_cast<uint8_t>(AnyScope::ConditionOpcode::MAX_VALUE),
			BLOODLINE_IS_ACTIVE_FOR,
			HAD_BLOODLINE_FLAG,
			HAS_BLOODLINE_FLAG,
		};

		enum struct CommandOpcode : uint8_t {};

		static std::unordered_map<std::string, ConditionOpcode> s_nameToConditionOpcode;
		static std::unordered_map<ConditionOpcode, std::string> s_conditionOpcodeToName;

		static std::unordered_map<std::string, CommandOpcode> s_nameToCommandOpcode;
		static std::unordered_map<CommandOpcode, std::string> s_commandOpcodeToName;
	};

	ConditionBlock(AnyScope::ConditionOpcode initalScope);

	ConditionBlock() = default;

	StatusCode execute();

	template <typename OpcodeType>
	void AppendInstruction(OpcodeType opcode)
	{
		instructions.push_back(static_cast<uint8_t>(opcode));
	}

	bool ExecuteCharacter(simulator::Character& charecter);

	bool execute_bloodline();

	bool AnyOwnedBloodline(const simulator::Character& character);

	bool ControlsReligion(const simulator::Character& character);

	bool Religion(const simulator::Character& character);

	const void* load_pointer();

	uint8_t store_pointer(const void* pointer);

	bool Advance();

	template <typename ScopeType>
	StatusCode Compile(const Node& node);

	StatusCode Compile(const Node& node, AnyScope::ConditionOpcode startingScope)
	{
		if (AnyScope::ConditionOpcode::CHARECTER_SCOPE == startingScope)
		{
			AppendInstruction(AnyScope::ConditionOpcode::CHARECTER_SCOPE);
			return Compile<CharacterScope>(node);
		}
		else
		{
			return StatusCode::NOT_IMPLIMENTED;
		}
	}

	template <typename ScopeType>
	void Decompile(std::string& output, const bool isNested = false);

	std::string DecompileCharecterScope();

	StatusCode compile_had_flag(const Node& node);

	StatusCode compile_date(const Node& node);

	template <typename ScopeType, typename Opcode>
	StatusCode HandleOpcode(const Node& node, const Opcode op_code);

	template <typename Opcode>
	StatusCode DecompileOpcode(std::string& output ,const Opcode op_code);

	StatusCode handle_anyscope_opcode(const Node& node, const AnyScope::ConditionOpcode op);

	StatusCode EvaluateResult(bool result);

	StatusCode append_bool_val(const Node& node);

	StatusCode DecompileBoolVal(std::string& output);

	template <typename SimulatorType, ConditionBlock::Control::Opcode LOAD_OP_CODE = ConditionBlock::Control::Opcode::MAX_VALUE>
	StatusCode append_pointer(const Node& node);

	template <typename SimulatorType, ConditionBlock::Control::Opcode LOAD_OP_CODE = ConditionBlock::Control::Opcode::MAX_VALUE>
	StatusCode append_id(const Node& node);

	template <typename SimulatorType>
	StatusCode DecompileId(std::string& output);

	StatusCode append_register(const Node& node);

	template <bool SHOULD_APPEND_LOAD_OPCODE>
	StatusCode append_string(const Node& node);

	StatusCode DecompileString(std::string& output);

	template <bool SHOULD_APPEND_LOAD_OPCODE>
	StatusCode append_number(const Node& node);

	template <ConditionBlock::Control::Opcode LOAD_OPOCDE>
	void AppendImmediate(auto& immediate_list, const auto& val);

	std::vector<uint8_t> instructions; ///< List of instructions to execute when evaluating this block.
	std::vector<uint8_t>::iterator ip; ///< Instruction pointer.
	
	std::vector<const void*> pointers; ///< List of pointers
	std::vector<std::string> strings; ///< List of strings
	std::vector<float> numbers; ///< List of strings

	std::stack<BlockType> stack;

	void* root_register;
	void* from_register;
};

/*
struct Scope : ConditionBlock
{

};

struct characterScope : Scope
{
	enum struct Opcode
	{
		RETURN,
		ControlsReligion,
	};

	simulator::Character& character; ///< The character to be evaluated.

	characterScope(simulator::Character& character, std::vector<int8_t>::iterator ip);

	bool execute();

	bool ControlsReligion() const;
};*/



template <typename ScopeType>
StatusCode ConditionBlock::Compile(const Node& node)
{
	for (const Node& child : node.children)
	{
		if (const auto iter = ScopeType::s_nameToConditionOpcode.find(child.name); ScopeType::s_nameToConditionOpcode.end() != iter)
		{
			AppendInstruction(iter->second);
			RETURN_RESULT_IF(StatusCode::SUCCESS, !=, HandleOpcode<ScopeType>(child, iter->second), child);
		}
		else if (const auto iter = ScopeType::s_nameToCommandOpcode.find(child.name); ScopeType::s_nameToCommandOpcode.end() != iter)
		{
			AppendInstruction(iter->second);
			// HandleOpcode.
		}
		else if (const auto iter2 = Control::s_nameToOpcode.find(child.name); Control::s_nameToOpcode.end() != iter2)
		{
			AppendInstruction(iter2->second);
			if (child.children.size())
				RETURN_RESULT_IF(StatusCode::SUCCESS, !=, Compile<ScopeType>(child), child);
			else
				;
		}
		else if (const auto iter3 = AnyScope::s_nameToConditionOpcode.find(child.name); AnyScope::s_nameToConditionOpcode.end() != iter3)
		{
			AppendInstruction(iter3->second);
			RETURN_RESULT_IF(StatusCode::SUCCESS, !=, handle_anyscope_opcode(child, iter3->second), child);
		}
		else if (const auto iter4 = simulator::ScriptedTrigger::map.find(child.name); simulator::ScriptedTrigger::map.end() != iter4)
		{
			RETURN_RESULT_IF(StatusCode::SUCCESS, !=, Compile<ScopeType>(*iter4->second.condition_block_source), child);
		}
		else
		{
			return child.debugging.store_and_forward_result(StatusCode::NOT_FOUND);
		}
	}

	AppendInstruction(Control::Opcode::RETURN);

	return StatusCode::SUCCESS;
}

template<typename Scope>
void ConditionBlock::Decompile(std::string& output, const bool isNested) 
{
	if (!isNested)
	{
		if (instructions.empty())
		{
			output += "EMPTY";
			return;
		}
		else
		{
			ip = instructions.begin();
		}
	}

	do  
	{
		std::uint8_t currentInstruction = *ip;

		if (Control::s_opcodeToName.contains(static_cast<Control::Opcode>(currentInstruction)))
		{
			output += Control::s_opcodeToName.at(static_cast<Control::Opcode>(currentInstruction));
			output += " || ";
			if (Control::Opcode::RETURN == static_cast<Control::Opcode>(currentInstruction) && isNested)
				return;
		}
		else if (Scope::s_conditionOpcodeToName.contains(static_cast<Scope::ConditionOpcode>(currentInstruction)))
		{
			output += Scope::s_conditionOpcodeToName.at(static_cast<Scope::ConditionOpcode>(currentInstruction));
			output += " || ";
			DecompileOpcode(output, static_cast<Scope::ConditionOpcode>(currentInstruction));
		}
		else if (Scope::s_commandOpcodeToName.contains(static_cast<Scope::CommandOpcode>(currentInstruction)))
		{
			output += Scope::s_commandOpcodeToName.at(static_cast<Scope::CommandOpcode>(currentInstruction));
			output += " || ";
			//DecompileOpcode(output, static_cast<Scope::CommandOpcode>(currentInstruction));
		}
		else if constexpr (std::is_same_v<AnyScope, Scope>)
		{
			switch (static_cast<AnyScope::ConditionOpcode>(currentInstruction))
			{
				case AnyScope::ConditionOpcode::CHARECTER_SCOPE:
					Decompile<CharacterScope>(output, true);
					break;
				default:
					break;
			}
		}
		else if constexpr (std::is_same_v<Scope, CharacterScope>)
		{
			switch (static_cast<CharacterScope::ConditionOpcode>(currentInstruction))
			{
				case CharacterScope::ConditionOpcode::ANY_OWNED_BLOODLINE:
					//Decompile<BloodlineScope>(output, true);
					break;
				default:
					break;
			}
		}
		else
		{
			output += "MISSING OPCODE || ";
		}
	} while (Advance());
}

}

}