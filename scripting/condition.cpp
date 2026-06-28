
#include "condition.hpp"

#include "simulator/religion.hpp"

#include "simulator/character.hpp"

namespace openck::scripting
{

std::unordered_map<std::string, ConditionBlock::Control::OpCode> ConditionBlock::Control::name_to_opcode =
{
	{"NOT", Control::OpCode::SCOPE_NOT},
	{"OR", Control::OpCode::SCOPE_OR}
};

std::unordered_map<std::string, ConditionBlock::CharacterScope::OpCode> ConditionBlock::CharacterScope::name_to_opcode =
{
	{"controls_religion", OpCode::CONTROLS_RELIGION},
	{"religion", OpCode::RELIGION},
	{"religion_group", OpCode::RELIGION_GROUP},
	{"is_tribal", OpCode::IS_TRIBAL},
	{"trait", OpCode::TRAIT},
	{"is_ruler", OpCode::IS_RULER},
	{"is_female", OpCode::IS_FEMALE},
	{"character", OpCode::CHARACTER},
	{"society_member_of", OpCode::SOCIETY_MEMBER_OF},
	{"has_religion_feature", OpCode::HAS_RELIGION_FEATURE},
	{"any_owned_bloodline",	OpCode::ANY_OWNED_BLOODLINE},
};

std::unordered_map<std::string, ConditionBlock::AnyScope::OpCode> ConditionBlock::AnyScope::name_to_opcode =
{
	{"has_dlc", OpCode::HAS_DLC}
};

ConditionBlock::ConditionBlock()
{
}

StatusCode ConditionBlock::compile_tree(const openck::parser::Node& node)
{
	/*const auto iter = name_to_opcode.find(node.name);
	if (name_to_opcode.end() != iter)
	{
		this->instructions.push_back(static_cast<uint8_t>(iter->second));

		switch (iter->second)
		{
			case OpCode::SCOPE_character:
				RETURN_RESULT_IF(StatusCode::SUCCESS, !=, character_compile(node));
		}
	}
	else
	{
		return StatusCode::FAILURE;
	}*/
	return StatusCode::NOT_IMPLIMENTED;
}

StatusCode ConditionBlock::execute()
{
	/*while(advance())
	{
		switch (static_cast<OpCode>(*(this->ip)))
		{
			case OpCode::SCOPE_NOT:
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
StatusCode ConditionBlock::append_imdieate<simulator::Society>(const parser::Node& node)
{
	return StatusCode::NOT_IMPLIMENTED;
}

template <>
StatusCode ConditionBlock::append_imdieate<simulator::ReligionFeature>(const parser::Node& node)
{
	return StatusCode::NOT_IMPLIMENTED;
}

template <>
StatusCode ConditionBlock::append_imdieate<simulator::Dlc>(const parser::Node& node)
{
	return StatusCode::NOT_IMPLIMENTED;
}

template <>
StatusCode ConditionBlock::handle_opcode<simulator::Character>(const openck::parser::Node& node, const CharacterScope::OpCode op)
{
	switch (op)
	{
		case CharacterScope::OpCode::RELIGION:
		{
			RETURN_RESULT_IF(StatusCode::SUCCESS, ==, append_imdieate<simulator::Religion>(node));
			break;
		}
		case CharacterScope::OpCode::CONTROLS_RELIGION:
		{
			RETURN_RESULT_IF(StatusCode::SUCCESS, ==, append_bool_val(node));
			return append_imdieate<simulator::Religion>(node);
			break;
		}
		case CharacterScope::OpCode::RELIGION_GROUP:
		{
			return append_imdieate<simulator::ReligionGroup>(node);
			break;
		}
		case CharacterScope::OpCode::HAS_RELIGION_FEATURE:
		{
			return append_imdieate<simulator::ReligionFeature>(node);
			break;
		}
		case CharacterScope::OpCode::TRAIT:
		{
			return append_imdieate<simulator::Trait>(node);
			break;
		}
		case CharacterScope::OpCode::SOCIETY_MEMBER_OF:
		{
			return append_imdieate<simulator::Society>(node);
			break;
		}
		case CharacterScope::OpCode::IS_FEMALE:
		case CharacterScope::OpCode::IS_RULER:
		case CharacterScope::OpCode::IS_TRIBAL:
		{
			return append_bool_val(node);
			break;
		}
		case CharacterScope::OpCode::CHARACTER:
		{
			return append_register(node);
			break;
		}
		case CharacterScope::OpCode::ANY_OWNED_BLOODLINE:
		{
			return compile<simulator::BloodLine>(node);
		}
		default:
		{
			return StatusCode::NOT_HANDLED;
		}
	}

	return StatusCode::FAILURE;
}

template <>
StatusCode ConditionBlock::handle_opcode<simulator::BloodLine>(const openck::parser::Node& node, const CharacterScope::OpCode op)
{
	return StatusCode::NOT_IMPLIMENTED;
}

StatusCode ConditionBlock::handle_anyscope_opcode(const openck::parser::Node& node, const AnyScope::OpCode op)
{
	switch (op)
	{
		case AnyScope::OpCode::HAS_DLC:
		{
			return append_imdieate<simulator::Dlc>(node);
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

StatusCode ConditionBlock::append_bool_val(const parser::Node& node)
{
	bool condition;
	RETURN_RESULT_IF(StatusCode::SUCCESS, !=, node.get_value(condition));
	ConditionBlock::ValueOpCode condition_op =  condition ? ConditionBlock::ValueOpCode::LOAD_TRUE : ConditionBlock::ValueOpCode::LOAD_FALSE;
	this->instructions.push_back(static_cast<uint8_t>(condition_op));
	return StatusCode::SUCCESS;
}

template <typename SimulatorType>
StatusCode ConditionBlock::append_imdieate(const parser::Node& node)
{
	const SimulatorType* simulator_obj;
	RETURN_RESULT_IF(StatusCode::SUCCESS, !=, SimulatorType::get_by_name(simulator_obj, node.value));

	uint8_t indx = this->store_pointer(simulator_obj);
	this->instructions.push_back(static_cast<uint8_t>(ConditionBlock::ValueOpCode::LOAD_IMEDIATE));
	this->instructions.push_back(indx);

	return StatusCode::SUCCESS;
}

StatusCode ConditionBlock::append_register(const parser::Node& node)
{
	if ("ROOT" == node.value)
		this->instructions.push_back(static_cast<uint8_t>(ConditionBlock::ValueOpCode::LOAD_ROOT));
	else if ("FROM" == node.value)
		this->instructions.push_back(static_cast<uint8_t>(ConditionBlock::ValueOpCode::LOAD_FROM));

	return StatusCode::SUCCESS;
}

}