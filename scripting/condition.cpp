
#include "condition.hpp"

#include "simulator/religion.hpp"
#include "simulator/character.hpp"
#include "simulator/date.hpp"

#include <cassert>

namespace openck::scripting
{

std::unordered_map<std::string, ConditionBlock::Control::OpCode> ConditionBlock::Control::name_to_opcode =
{
	{"NOT", Control::OpCode::NOT},
	{"OR", Control::OpCode::OR},
	{"AND", Control::OpCode::AND},
};

std::unordered_map<std::string, ConditionBlock::CharacterScope::OpCode> ConditionBlock::CharacterScope::name_to_opcode =
{
	{"controls_religion", OpCode::CONTROLS_RELIGION},
	{"religion", OpCode::RELIGION},
	{"culture", OpCode::CULTURE},
	{"religion_group", OpCode::RELIGION_GROUP},
	{"is_tribal", OpCode::IS_TRIBAL},
	{"is_theocracy", OpCode::IS_THEOCRACY},
	{"trait", OpCode::TRAIT},
	{"is_ruler", OpCode::IS_RULER},
	{"is_female", OpCode::IS_FEMALE},
	{"character", OpCode::CHARACTER},
	{"society_member_of", OpCode::SOCIETY_MEMBER_OF},
	{"has_religion_feature", OpCode::HAS_RELIGION_FEATURE},
	{"age", OpCode::AGE},
	{"any_owned_bloodline",	OpCode::ANY_OWNED_BLOODLINE},
	{"ai", OpCode::AI},
	{"prisoner", OpCode::PRISONER},
	{"race", OpCode::RACE},
	{"has_character_flag", OpCode::HAS_FLAG},
	{"has_flag", OpCode::HAS_FLAG},
};

std::unordered_map<std::string, ConditionBlock::AnyScope::OpCode> ConditionBlock::AnyScope::name_to_opcode =
{
	{"has_dlc", OpCode::HAS_DLC}
};

std::unordered_map<std::string, ConditionBlock::BloodlineScope::OpCode> ConditionBlock::BloodlineScope::name_to_opcode =
{
	{"bloodline", OpCode::BLOODLINE},
	{"bloodline_is_active_for", OpCode::BLOODLINE_IS_ACTIVE_FOR},
	{"had_bloodline_flag", OpCode::HAD_BLOODLINE_FLAG},
	{"has_bloodline_flag", OpCode::HAS_BLOODLINE_FLAG},
};

ConditionBlock::ConditionBlock()
{
}


StatusCode ConditionBlock::execute()
{
	/*while(advance())
	{
		switch (static_cast<OpCode>(*(this->ip)))
		{
			case OpCode::NOT:
				break;

			case OpCode::SCOPE_character:
				execute_character();
				break;
		}
	}*/
	return StatusCode::NOT_IMPLIMENTED;
}

StatusCode ConditionBlock::execute_character()
{
	/*simulator::character* root = static_cast<simulator::character*>(this->root);
	StatusCode result;

	while (advance())
	{
		switch (static_cast<CharacterScope::OpCode>(*(this->ip)))
		{
		case CharacterScope::OpCode::CONTROLS_RELIGION:
			result = controls_religion(root);
			break;

		case CharacterScope::OpCode::RELIGION:
			result = religion(root);

		case CharacterScope::OpCode::SCOPE_ANY_OWNED_BLOODLINE:
			result = any_owned_bloodline(root);

		default:
			break;
		}

		// Todo: needs to be switch
		StatusCode status_code = evaluate_result(result);
		if (status_code == StatusCode::SUCCESS)
			return true;
		else if (status_code == StatusCode::FAILURE)
			return false;
		else
			continue;
	}*/
	return StatusCode::NOT_IMPLIMENTED;
}

bool ConditionBlock::execute_bloodline()
{
	return false;
}

bool ConditionBlock::advance()
{
	return ++this->ip != this->instructions.end();
}

bool ConditionBlock::controls_religion(simulator::Character *character)
{
	const simulator::Religion* religion = static_cast<const simulator::Religion*>(this->load_pointer());
	return religion->get_head() == character;
}

bool ConditionBlock::religion(simulator::Character *character)
{
	const simulator::Religion* religion = static_cast<const simulator::Religion*>(this->load_pointer());
	return religion == character->religion;
}

StatusCode ConditionBlock::any_owned_bloodline(simulator::Character *character)
{
	/*for (simulator::BloodLine* bloodline : character->bloodlines)
		execute_bloodline();*/

	return StatusCode::NOT_IMPLIMENTED;
}

const void* ConditionBlock::load_pointer()
{
	this->advance();
	return this->pointers[*(this->ip)];
}

uint8_t ConditionBlock::store_pointer(const void* pointer)
{
	uint8_t indx = this->pointers.size();
	assert(std::numeric_limits<uint8_t>::max() > indx);
	this->pointers.push_back(pointer);
	return indx;
}

StatusCode ConditionBlock::evaluate_result(bool result)
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
	default:
		return StatusCode::NOT_HANDLED;
		break;
	}
}

template <>
StatusCode ConditionBlock::append_pointer<simulator::Society>(const Node& node)
{
	return StatusCode::NOT_IMPLIMENTED;
}

template <>
StatusCode ConditionBlock::append_pointer<simulator::Culture>(const Node& node)
{
	return StatusCode::NOT_IMPLIMENTED;
}

template <>
StatusCode ConditionBlock::append_pointer<simulator::ReligionFeature>(const Node& node)
{
	return StatusCode::NOT_IMPLIMENTED;
}

template <>
StatusCode ConditionBlock::append_pointer<simulator::Dlc>(const Node& node)
{
	return StatusCode::NOT_IMPLIMENTED;
}

template <>
StatusCode ConditionBlock::append_pointer<simulator::BloodLine>(const Node& node)
{
	return StatusCode::NOT_IMPLIMENTED;
}

StatusCode ConditionBlock::compile_had_flag(const Node& node)
{
	const Node* flag;
	RETURN_RESULT_IF(StatusCode::SUCCESS, !=, node.get_child_by_name(flag, "flag"));
	return compile_date(node);
}

template <>
StatusCode ConditionBlock::handle_opcode<ConditionBlock::CharacterScope>(const Node& node, const ConditionBlock::CharacterScope::OpCode op)
{
	switch (op)
	{
		case CharacterScope::OpCode::RELIGION:
		{
			return append_pointer<simulator::Religion>(node);
			break;
		}
		case CharacterScope::OpCode::CONTROLS_RELIGION:
		{
			RETURN_RESULT_IF(StatusCode::SUCCESS, ==, append_bool_val(node));
			return append_pointer<simulator::Religion>(node);
			break;
		}
		case CharacterScope::OpCode::RELIGION_GROUP:
		{
			return append_pointer<simulator::ReligionGroup>(node);
			break;
		}
		case CharacterScope::OpCode::HAS_RELIGION_FEATURE:
		{
			return append_pointer<simulator::ReligionFeature>(node);
			break;
		}
		case CharacterScope::OpCode::TRAIT:
		{
			return append_pointer<simulator::Trait>(node);
			break;
		}
		case CharacterScope::OpCode::SOCIETY_MEMBER_OF:
		{
			return append_pointer<simulator::Society>(node);
			break;
		}
		case CharacterScope::OpCode::CULTURE:
		{
			return append_pointer<simulator::Culture>(node);
			break;
		}
		case CharacterScope::OpCode::HAS_FLAG:
		{
			return append_string<false>(node);
			break;
		}
		case CharacterScope::OpCode::IS_FEMALE:
		case CharacterScope::OpCode::IS_RULER:
		case CharacterScope::OpCode::IS_TRIBAL:
		case CharacterScope::OpCode::IS_THEOCRACY:
		case CharacterScope::OpCode::AI:
		case CharacterScope::OpCode::PRISONER:
		{
			return append_bool_val(node);
			break;
		}
		case CharacterScope::OpCode::CHARACTER:
		{
			RETURN_RESULT_IF(StatusCode::SUCCESS, ==, append_register(node));
			return append_pointer<simulator::Character>(node);
			break;
		}
		case CharacterScope::OpCode::ANY_OWNED_BLOODLINE:
		{
			return compile<BloodlineScope>(node);
		}
		case CharacterScope::OpCode::AGE:
		case CharacterScope::OpCode::RACE:
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
StatusCode ConditionBlock::handle_opcode<ConditionBlock::BloodlineScope>(const Node& node, const ConditionBlock::BloodlineScope::OpCode op)
{
	switch (op)
	{
		case BloodlineScope::OpCode::BLOODLINE:
		{
			RETURN_RESULT_IF(StatusCode::SUCCESS, ==, append_pointer<simulator::BloodLine>(node));
			break;
		}
		case BloodlineScope::OpCode::BLOODLINE_IS_ACTIVE_FOR:
		{
			RETURN_RESULT_IF(StatusCode::SUCCESS, ==, append_register(node));
			return append_pointer<simulator::Character>(node);
			break;
		}
		case BloodlineScope::OpCode::HAD_BLOODLINE_FLAG:
		{
			return compile_had_flag(node);
			break;
		}
		case BloodlineScope::OpCode::HAS_BLOODLINE_FLAG:
		{
			return append_string<false>(node);
			break;
		}
	}

	return StatusCode::FAILURE;
}

StatusCode ConditionBlock::handle_anyscope_opcode(const Node& node, const AnyScope::OpCode op)
{
	switch (op)
	{
		case AnyScope::OpCode::HAS_DLC:
		{
			return append_pointer<simulator::Dlc>(node);
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
	RETURN_RESULT_IF(StatusCode::SUCCESS, !=, node.get_value(condition));
	ConditionBlock::LoadOpCode condition_op =  condition ? ConditionBlock::LoadOpCode::LOAD_TRUE : ConditionBlock::LoadOpCode::LOAD_FALSE;
	this->instructions.push_back(static_cast<uint8_t>(condition_op));
	return StatusCode::SUCCESS;
}

template <typename SimulatorType>
StatusCode ConditionBlock::append_pointer(const Node& node)
{
	const SimulatorType* ptr;
	RETURN_RESULT_IF(StatusCode::SUCCESS, !=, SimulatorType::get_by_name(ptr, node.value));
	append_immediate<true, LoadOpCode::LOAD_IMEDIATE>(this->pointers, ptr);

	return StatusCode::SUCCESS;
}

StatusCode ConditionBlock::append_register(const Node& node)
{
	if ("ROOT" == node.value)
		this->instructions.push_back(static_cast<uint8_t>(ConditionBlock::LoadOpCode::LOAD_ROOT));
	else if ("FROM" == node.value)
		this->instructions.push_back(static_cast<uint8_t>(ConditionBlock::LoadOpCode::LOAD_FROM));
	else
		return StatusCode::NOT_FOUND;

	return StatusCode::SUCCESS;
}

template <bool SHOULD_APPEND_LOAD_OPCODE>
StatusCode ConditionBlock::append_string(const Node& node)
{
	append_immediate<SHOULD_APPEND_LOAD_OPCODE, LoadOpCode::LOAD_STRING>(this->strings, node.value);
	return StatusCode::SUCCESS;
}

template <bool SHOULD_APPEND_LOAD_OPCODE>
StatusCode ConditionBlock::append_number(const Node& node)
{
	decltype(this->numbers)::value_type val;
	RETURN_RESULT_IF(StatusCode::SUCCESS, !=, node.get_value(val));
	append_immediate<SHOULD_APPEND_LOAD_OPCODE, LoadOpCode::LOAD_NUMBER>(this->numbers, val);
	return StatusCode::SUCCESS;
}

template <bool SHOULD_APPEND_LOAD_OPCODE, ConditionBlock::LoadOpCode LOAD_OPOCDE>
void ConditionBlock::append_immediate(auto& immediate_list, const auto& val)
{
	uint8_t indx;
	auto iter = std::ranges::find(immediate_list, val);
	if (immediate_list.end() != iter)
		indx = std::distance(immediate_list.begin(), iter);
	else
		indx = immediate_list.size();

	assert(std::numeric_limits<decltype(this->instructions)::value_type>::max() > indx);

	immediate_list.push_back(val);
	if constexpr(SHOULD_APPEND_LOAD_OPCODE)
		this->instructions.push_back(static_cast<uint8_t>(LOAD_OPOCDE));
	this->instructions.push_back(indx);
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

}