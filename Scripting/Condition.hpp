
#pragma once

#include "Parser/PrdxParser.hpp"
#include "Simulator/ScriptedTrigger.hpp"

#include "Utils/StatusCode.hpp"

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
		enum struct Opcode : uint8_t
		{
			HAS_DLC = static_cast<uint8_t>(Control::Opcode::MAX_VALUE),
			CHARECTER_SCOPE,
			BLOODLINE_SCOPE,
			MAX_VALUE,
		};

		template <typename T>
		static constexpr Opcode GetScopeOpcode()
		{
			if constexpr (std::is_same_v<CharacterScope, T>) 
			{
				return  Opcode::CHARECTER_SCOPE;
			}
			else if (std::is_same_v<BloodlineScope, T>) 
			{
				return  Opcode::BLOODLINE_SCOPE;
			}
		}

		static std::unordered_map<std::string, Opcode> s_nameToOpcode;
		static std::unordered_map<Opcode, std::string> s_opcodeToName;
	};

	struct CharacterScope
	{
		enum struct Opcode : uint8_t
		{
			CONTROLS_RELIGION = static_cast<uint8_t>(AnyScope::Opcode::MAX_VALUE),
			RELIGION,
			CULTURE,
			RELIGION_GROUP,
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

		static std::unordered_map<std::string, Opcode> s_nameToOpcode;
		static std::unordered_map<Opcode, std::string> s_opcodeToName;
	};

	struct BloodlineScope
	{
		enum struct Opcode : uint8_t
		{
			BLOODLINE = static_cast<uint8_t>(AnyScope::Opcode::MAX_VALUE),
			BLOODLINE_IS_ACTIVE_FOR,
			HAD_BLOODLINE_FLAG,
			HAS_BLOODLINE_FLAG,
		};

		static std::unordered_map<std::string, Opcode> s_nameToOpcode;
	};

	ConditionBlock(AnyScope::Opcode initalScope);

	StatusCode execute();

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

	template <typename ScopeType>
	void Decompile(std::string& output, const bool isNested);

	std::string DecompileCharecterScope();

	StatusCode compile_had_flag(const Node& node);

	StatusCode compile_date(const Node& node);

	template <typename ScopeType, typename Opcode>
	StatusCode HandleOpcode(const Node& node, const Opcode op_code);

	template <typename Opcode>
	StatusCode DecompileOpcode(std::string& output ,const Opcode op_code);

	StatusCode handle_anyscope_opcode(const Node& node, const AnyScope::Opcode op);

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
		if (const auto iter = ScopeType::s_nameToOpcode.find(child.name); ScopeType::s_nameToOpcode.end() != iter)
		{
			this->instructions.push_back(static_cast<uint8_t>(iter->second));
			RETURN_RESULT_IF(StatusCode::SUCCESS, !=, HandleOpcode<ScopeType>(child, iter->second), child);
		}
		else if (const auto iter2 = Control::s_nameToOpcode.find(child.name); Control::s_nameToOpcode.end() != iter2)
		{
			this->instructions.push_back(static_cast<uint8_t>(iter2->second));
			if (child.children.size())
				RETURN_RESULT_IF(StatusCode::SUCCESS, !=, Compile<ScopeType>(child), child);
			else
				;
		}
		else if (const auto iter3 = AnyScope::s_nameToOpcode.find(child.name); AnyScope::s_nameToOpcode.end() != iter3)
		{
			this->instructions.push_back(static_cast<uint8_t>(iter3->second));
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

	this->instructions.push_back(static_cast<uint8_t>(Control::Opcode::RETURN));

	return StatusCode::SUCCESS;
}

template<typename Scope>
void ConditionBlock::Decompile(std::string& output, const bool isNested) 
{
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
		else if (Scope::s_opcodeToName.contains(static_cast<Scope::Opcode>(currentInstruction)))
		{
			output += Scope::s_opcodeToName.at(static_cast<Scope::Opcode>(currentInstruction));
			output += " || ";
			DecompileOpcode(output, static_cast<Scope::Opcode>(currentInstruction));
		}
		else if constexpr (std::is_same_v<AnyScope, Scope>)
		{
			switch (static_cast<AnyScope::Opcode>(currentInstruction))
			{
				case AnyScope::Opcode::CHARECTER_SCOPE:
					Decompile<CharacterScope>(output, true);
					break;
				default:
					break;
			}
		}
		else if constexpr (std::is_same_v<Scope, CharacterScope>)
		{
			switch (static_cast<CharacterScope::Opcode>(currentInstruction))
			{
				case CharacterScope::Opcode::ANY_OWNED_BLOODLINE:
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