
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

	ConditionBlock();

	enum struct LoadOpCode : uint8_t
	{
		UNDEFINED,
		LOAD_TRUE,
		LOAD_FALSE,
		LOAD_POINTER,
		LOAD_STRING,
		LOAD_FROM,
		LOAD_ROOT,
		LOAD_NUMBER,
		LOAD_RELIGION_ID,
	};

	enum struct DateOpCode
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
		NAND
	};

	struct Control
	{
		enum struct OpCode : uint8_t
		{
			RETURN = 200,
			OR,
			AND,
			NOR,
			NAND,
			NOT,
			MAX_VALUE
		};

		static std::unordered_map<std::string, OpCode> name_to_opcode;
	};
	struct AnyScope
	{
		enum struct OpCode : uint8_t
		{
			HAS_DLC,
			MAX_VALUE,
		};

		static std::unordered_map<std::string, OpCode> name_to_opcode;
	};

	struct CharacterScope
	{
		enum struct OpCode : uint8_t
		{
			CONTROLS_RELIGION = static_cast<uint8_t>(AnyScope::OpCode::MAX_VALUE),
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

		static std::unordered_map<std::string, OpCode> name_to_opcode;
	};

	struct BloodlineScope
	{
		enum struct OpCode : uint8_t
		{
			BLOODLINE = static_cast<uint8_t>(AnyScope::OpCode::MAX_VALUE),
			BLOODLINE_IS_ACTIVE_FOR,
			HAD_BLOODLINE_FLAG,
			HAS_BLOODLINE_FLAG,
		};

		static std::unordered_map<std::string, OpCode> name_to_opcode;
	};

	StatusCode execute();

	StatusCode execute_character();

	bool execute_bloodline();

	StatusCode any_owned_bloodline(simulator::Character *character);

	bool controls_religion(simulator::Character* character);

	bool religion(simulator::Character* character);

	const void* load_pointer();

	uint8_t store_pointer(const void* pointer);

	bool advance();

	template <typename ScopeType>
	StatusCode compile(const Node& node);

	StatusCode compile_had_flag(const Node& node);

	StatusCode compile_date(const Node& node);

	template <typename ScopeType, typename OpCode>
	StatusCode handle_opcode(const Node& node, const OpCode op_code);

	StatusCode handle_anyscope_opcode(const Node& node, const AnyScope::OpCode op);

	StatusCode evaluate_result(bool result);

	StatusCode append_bool_val(const Node& node);

	template <typename SimulatorType, ConditionBlock::LoadOpCode LOAD_OP_CODE = ConditionBlock::LoadOpCode::UNDEFINED>
	StatusCode append_pointer(const Node& node);

	template <typename SimulatorType, ConditionBlock::LoadOpCode LOAD_OP_CODE = ConditionBlock::LoadOpCode::UNDEFINED>
	StatusCode append_id(const Node& node);

	StatusCode append_register(const Node& node);

	template <bool SHOULD_APPEND_LOAD_OPCODE>
	StatusCode append_string(const Node& node);

	template <bool SHOULD_APPEND_LOAD_OPCODE>
	StatusCode append_number(const Node& node);

	template <ConditionBlock::LoadOpCode LOAD_OPOCDE>
	void append_immediate(auto& immediate_list, const auto& val);

	std::vector<uint8_t> instructions; ///< List of instructions to execute when evaluating this block.
	std::vector<uint8_t>::iterator ip; ///< Instruction pointer.
	
	std::vector<const void*> pointers; ///< List of pointers
	std::vector<std::string> strings; ///< List of strings
	std::vector<float> numbers; ///< List of strings

	std::stack<BlockType> stack;

	void* root_register;
	void* from_register;
};

struct Scope : ConditionBlock
{

};

struct characterScope : Scope
{
	enum struct OpCode
	{
		RETURN,
		CONTROLS_RELIGION,
	};

	simulator::Character& character; ///< The character to be evaluated.

	characterScope(simulator::Character& character, std::vector<int8_t>::iterator ip);

	bool execute();

	bool controls_religion() const;
};

template <typename ScopeType>
StatusCode ConditionBlock::compile(const Node& node)
{
	for (const Node& child : node.children)
	{
		if (const auto iter = ScopeType::name_to_opcode.find(child.name); ScopeType::name_to_opcode.end() != iter)
		{
			this->instructions.push_back(static_cast<uint8_t>(iter->second));
			RETURN_RESULT_IF(StatusCode::SUCCESS, !=, handle_opcode<ScopeType>(child, iter->second), child);
		}
		else if (const auto iter2 = Control::name_to_opcode.find(child.name); Control::name_to_opcode.end() != iter2)
		{
			this->instructions.push_back(static_cast<uint8_t>(iter2->second));
			if (child.children.size())
				RETURN_RESULT_IF(StatusCode::SUCCESS, !=, compile<ScopeType>(child), child);
			else
				;
		}
		else if (const auto iter3 = AnyScope::name_to_opcode.find(child.name); AnyScope::name_to_opcode.end() != iter3)
		{
			this->instructions.push_back(static_cast<uint8_t>(iter3->second));
			RETURN_RESULT_IF(StatusCode::SUCCESS, !=, handle_anyscope_opcode(child, iter3->second), child);
		}
		else if (const auto iter4 = simulator::ScriptedTrigger::map.find(child.name); simulator::ScriptedTrigger::map.end() != iter4)
		{
			RETURN_RESULT_IF(StatusCode::SUCCESS, !=, compile<ScopeType>(*iter4->second.condition_block_source), child);
		}
		else
		{
			return child.debugging.store_and_forward_result(StatusCode::NOT_FOUND);
		}
	}

	this->instructions.push_back(static_cast<uint8_t>(Control::OpCode::RETURN));

	return StatusCode::SUCCESS;
}

}

}