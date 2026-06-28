
#pragma once

#include "parser/prdx_parser.h"

#include "utils/status_code.hpp"

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
	ConditionBlock();

	enum struct ValueOpCode
	{
		LOAD_TRUE,
		LOAD_FALSE,
		LOAD_IMEDIATE,
		LOAD_FROM,
		LOAD_ROOT
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
			SCOPE_CHARACTER,
			SCOPE_OR,
			SCOPE_AND,
			SCOPE_NOR,
			SCOPE_NAND,
			SCOPE_NOT,
			CONTROL_OP_CODE_MAX
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
			RELIGION_GROUP,
			SCOPE_ANY_OWNED_BLOODLINE,
			IS_TRIBAL,
			TRAIT,
			IS_RULER,
			IS_FEMALE,
			CHARACTER,
			SOCIETY_MEMBER_OF,
			HAS_RELIGION_FEATURE,
			ANY_OWNED_BLOODLINE
		};

		static std::unordered_map<std::string, OpCode> name_to_opcode;
	};

	std::vector<uint8_t> instructions; ///< List of instructions to execute when evaluating this block.
	std::vector<uint8_t>::iterator ip; ///< Instruction pointer.
	std::vector<const void*> pointers; ///< List of pointers
	std::stack<BlockType> stack;

	void* root_register;
	void* from_register;

	StatusCode execute();

	StatusCode execute_character();

	bool execute_bloodline();

	StatusCode any_owned_bloodline(simulator::Character *character);

	bool controls_religion(simulator::Character* character);

	bool religion(simulator::Character* character);

	const void* load_pointer();

	uint8_t store_pointer(const void* pointer);

	bool advance();

	StatusCode compile_tree(const openck::parser::Node& node);

	template <typename T>
	StatusCode compile(const openck::parser::Node& node);

	template <typename T>
	StatusCode handle_opcode(const openck::parser::Node& node, const CharacterScope::OpCode op_code);

	StatusCode handle_anyscope_opcode(const openck::parser::Node& node, const AnyScope::OpCode op);

	StatusCode evaluate_result(bool result);

	StatusCode append_bool_val(const parser::Node& node);

	template <typename SimulatorType>
	StatusCode append_imdieate(const parser::Node& node);

	StatusCode append_register(const parser::Node& node);
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


template <typename T>
StatusCode ConditionBlock::compile(const openck::parser::Node& node)
{
	for (const openck::parser::Node& child : node.children)
	{
		if (const auto iter = CharacterScope::name_to_opcode.find(child.name); CharacterScope::name_to_opcode.end() != iter)
		{
			this->instructions.push_back(static_cast<uint8_t>(iter->second));
			RETURN_RESULT_IF(StatusCode::SUCCESS, !=, handle_opcode<T>(child, iter->second));
		}
		else if (const auto iter2 = Control::name_to_opcode.find(child.name); Control::name_to_opcode.end() != iter2)
		{
			this->instructions.push_back(static_cast<uint8_t>(iter2->second));
			if (node.children.size())
				RETURN_RESULT_IF(StatusCode::SUCCESS, !=, compile<T>(child));
		}
		else if (const auto iter3 = AnyScope::name_to_opcode.find(child.name); AnyScope::name_to_opcode.end() != iter3)
		{
			this->instructions.push_back(static_cast<uint8_t>(iter3->second));
			RETURN_RESULT_IF(StatusCode::SUCCESS, !=, handle_anyscope_opcode(child, iter3->second));
		}
		else
		{
			return StatusCode::NOT_FOUND;
		}
	}

	this->instructions.push_back(static_cast<uint8_t>(Control::OpCode::RETURN));

	return StatusCode::SUCCESS;
}

}

}