
#include "Condition.hpp"

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

std::unordered_map<std::string, ConditionBlock::AnyScope::Opcode> ConditionBlock::AnyScope::s_nameToOpcode =
{
	{"has_dlc", Opcode::HAS_DLC},
	{"INTERNAL_CHARECTER_SCOPE", Opcode::CHARECTER_SCOPE}
};

std::unordered_map<ConditionBlock::AnyScope::Opcode, std::string> ConditionBlock::AnyScope::s_opcodeToName = Reverse(ConditionBlock::AnyScope::s_nameToOpcode);

std::unordered_map<std::string, ConditionBlock::Control::Opcode> ConditionBlock::Control::s_nameToOpcode =
{
	{"OR", Opcode::OR},
	{"RETURN", Opcode::RETURN},	
	{"AND", Opcode::AND},
	{"NOR", Opcode::NOR},
	{"NAND", Opcode::NAND},
	{"NOT", Opcode::NOT},
	{"LOAD_TRUE", Opcode::LOAD_TRUE},
	{"LOAD_FALSE", Opcode::LOAD_FALSE},
	{"LOAD_POINTER", Opcode::LOAD_POINTER},
	{"LOAD_STRING", Opcode::LOAD_STRING},
	{"LOAD_FROM", Opcode::LOAD_FROM},
	{"LOAD_ROOT", Opcode::LOAD_ROOT},
	{"LOAD_NUMBER", Opcode::LOAD_NUMBER},
	{"LOAD_RELIGION_ID", Opcode::LOAD_RELIGION_ID},
	{"MAX_VALUE", Opcode::MAX_VALUE},
};

std::unordered_map<ConditionBlock::Control::Opcode, std::string> ConditionBlock::Control::s_opcodeToName = Reverse(ConditionBlock::Control::s_nameToOpcode);

std::unordered_map<std::string, ConditionBlock::CharacterScope::Opcode> ConditionBlock::CharacterScope::s_nameToOpcode
{
	{"controls_religion", Opcode::CONTROLS_RELIGION},
	{"religion", Opcode::RELIGION},
	{"culture", Opcode::CULTURE},
	{"religion_group", Opcode::RELIGION_GROUP},
	{"is_triba", Opcode::IS_TRIBAL},
	{"is_theocracy", Opcode::IS_THEOCRACY},
	{"trait", Opcode::TRAIT},
	{"is_ruler", Opcode::IS_RULER},
	{"is_female", Opcode::IS_FEMALE},
	{"character", Opcode::CHARACTER},
	{"society_member_of", Opcode::SOCIETY_MEMBER_OF},
	{"has_religion_feature", Opcode::HAS_RELIGION_FEATURE},
	{"age", Opcode::AGE},
	{"any_owned_bloodline",	Opcode::ANY_OWNED_BLOODLINE},
	{"ai", Opcode::AI},
	{"prisoner", Opcode::PRISONER},
	{"race", Opcode::RACE},
	{"has_character_flag", Opcode::HAS_FLAG},
	{"has_flag", Opcode::HAS_FLAG},
};

std::unordered_map<ConditionBlock::CharacterScope::Opcode, std::string> ConditionBlock::CharacterScope::s_opcodeToName = Reverse(ConditionBlock::CharacterScope::s_nameToOpcode);

std::unordered_map<std::string, ConditionBlock::BloodlineScope::Opcode> ConditionBlock::BloodlineScope::s_nameToOpcode =
{
	{"bloodline", Opcode::BLOODLINE},
	{"bloodline_is_active_for", Opcode::BLOODLINE_IS_ACTIVE_FOR},
	{"had_bloodline_flag", Opcode::HAD_BLOODLINE_FLAG},
	{"has_bloodline_flag", Opcode::HAS_BLOODLINE_FLAG},
};

ConditionBlock::ConditionBlock(ConditionBlock::AnyScope::Opcode initalScopeOpcode)
{
	this->instructions.push_back(static_cast<uint8_t>(initalScopeOpcode));
}

StatusCode ConditionBlock::execute()
{
	while(Advance())
	{
		switch (static_cast<AnyScope::Opcode>(*(this->ip)))
		{
			case AnyScope::Opcode::CHARECTER_SCOPE:
				//RETURN_RESULT_IF(StatusCode::SUCCESS, !=, ExecuteCharacter());
				break;

			case AnyScope::Opcode::BLOODLINE_SCOPE:
				
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
		switch (static_cast<CharacterScope::Opcode>(*(this->ip)))
		{
		case CharacterScope::Opcode::CONTROLS_RELIGION:
			result = ControlsReligion(charecter);
			break;

		case CharacterScope::Opcode::RELIGION:
			result = Religion(charecter);
			break;

		case CharacterScope::Opcode::ANY_OWNED_BLOODLINE:
			//result = AnyOwnedBloodline(charecter);
			break;

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
	if (this->ip == this->instructions.end())
	{
		return false;
	}
	else
	{
		this->ip++;
		return true;
	}
}

bool ConditionBlock::ControlsReligion(const simulator::Character& character)
{
	const simulator::Religion* religion = static_cast<const simulator::Religion*>(this->load_pointer());
	return religion->get_head() == &character; 
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
StatusCode ConditionBlock::HandleOpcode<ConditionBlock::CharacterScope>(const Node& node, const ConditionBlock::CharacterScope::Opcode op)
{
	switch (op)
	{
		case CharacterScope::Opcode::RELIGION:
		{
			return append_id<simulator::Religion, Control::Opcode::LOAD_RELIGION_ID>(node);
			//others...
			break;
		}
		case CharacterScope::Opcode::CONTROLS_RELIGION:
		{
			return append_bool_val(node);
			break;
		}
		case CharacterScope::Opcode::RELIGION_GROUP:
		{
			return append_id<simulator::ReligionGroup>(node);
			//others
			break;
		}
		case CharacterScope::Opcode::HAS_RELIGION_FEATURE:
		{
			return append_id<simulator::ReligionFeature>(node);
			break;
		}
		case CharacterScope::Opcode::TRAIT:
		{
			return append_id<simulator::Trait>(node);
			break;
		}
		case CharacterScope::Opcode::SOCIETY_MEMBER_OF:
		{
			return append_id<simulator::Society>(node);
			break;
		}
		case CharacterScope::Opcode::CULTURE:
		{
			return append_id<simulator::Culture>(node);
			break;
		}
		case CharacterScope::Opcode::HAS_FLAG:
		{
			return append_string<false>(node);
			break;
		}
		case CharacterScope::Opcode::IS_FEMALE:
		case CharacterScope::Opcode::IS_RULER:
		case CharacterScope::Opcode::IS_TRIBAL:
		case CharacterScope::Opcode::IS_THEOCRACY:
		case CharacterScope::Opcode::AI:
		case CharacterScope::Opcode::PRISONER:
		{
			return append_bool_val(node);
			break;
		}
		case CharacterScope::Opcode::CHARACTER:
		{
			RETURN_RESULT_IF(StatusCode::SUCCESS, ==, append_register(node));
			return append_pointer<simulator::Character>(node);
			break;
		}
		case CharacterScope::Opcode::ANY_OWNED_BLOODLINE:
		{
			this->instructions.push_back(static_cast<uint8_t>(AnyScope::GetScopeOpcode<BloodlineScope>()));
			return Compile<BloodlineScope>(node);
		}
		case CharacterScope::Opcode::AGE:
		case CharacterScope::Opcode::RACE:
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
StatusCode ConditionBlock::HandleOpcode<ConditionBlock::BloodlineScope>(const Node& node, const ConditionBlock::BloodlineScope::Opcode op)
{
	switch (op)
	{
		case BloodlineScope::Opcode::BLOODLINE:
		{
			RETURN_RESULT_IF(StatusCode::SUCCESS, ==, append_pointer<simulator::BloodLine>(node));
			break;
		}
		case BloodlineScope::Opcode::BLOODLINE_IS_ACTIVE_FOR:
		{
			RETURN_RESULT_IF(StatusCode::SUCCESS, ==, append_register(node));
			return append_pointer<simulator::Character>(node);
			break;
		}
		case BloodlineScope::Opcode::HAD_BLOODLINE_FLAG:
		{
			return compile_had_flag(node);
			break;
		}
		case BloodlineScope::Opcode::HAS_BLOODLINE_FLAG:
		{
			return append_string<false>(node);
			break;
		}
	}

	return StatusCode::FAILURE;
}

StatusCode ConditionBlock::handle_anyscope_opcode(const Node& node, const AnyScope::Opcode op)
{
	switch (op)
	{
		case AnyScope::Opcode::HAS_DLC:
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
	RETURN_RESULT_IF(StatusCode::SUCCESS, !=, node.get_value(condition));
	ConditionBlock::Control::Opcode condition_op =  condition ? ConditionBlock::Control::Opcode::LOAD_TRUE : ConditionBlock::Control::Opcode::LOAD_FALSE;
	this->instructions.push_back(static_cast<uint8_t>(condition_op));
	return StatusCode::SUCCESS;
}

template <typename SimulatorType, ConditionBlock::Control::Opcode LOAD_OP_CODE>
StatusCode ConditionBlock::append_pointer(const Node& node)
{
	const SimulatorType* ptr;
	RETURN_RESULT_IF(StatusCode::SUCCESS, !=, SimulatorType::get_by_name(ptr, node.value));
	
	append_immediate<LOAD_OP_CODE>(this->pointers, ptr);

	return StatusCode::SUCCESS;
}

template <typename SimulatorType, ConditionBlock::Control::Opcode LOAD_OP_CODE>
StatusCode ConditionBlock::append_id(const Node& node)
{
	const SimulatorType* ptr;
	RETURN_RESULT_IF(StatusCode::SUCCESS, !=, SimulatorType::get_by_name(ptr, node.value));
	
	append_immediate<LOAD_OP_CODE>(this->numbers, ptr->id);

	return StatusCode::SUCCESS;
}

StatusCode ConditionBlock::append_register(const Node& node)
{
	if ("ROOT" == node.value)
		this->instructions.push_back(static_cast<uint8_t>(ConditionBlock::Control::Opcode::LOAD_ROOT));
	else if ("FROM" == node.value)
		this->instructions.push_back(static_cast<uint8_t>(ConditionBlock::Control::Opcode::LOAD_FROM));
	else
		return StatusCode::NOT_FOUND;

	return StatusCode::SUCCESS;
}

template <bool SHOULD_APPEND_LOAD_OPCODE>
StatusCode ConditionBlock::append_string(const Node& node)
{
	if constexpr (SHOULD_APPEND_LOAD_OPCODE)
		append_immediate<Control::Opcode::LOAD_STRING>(this->strings, node.value);
	else
		append_immediate<Control::Opcode::MAX_VALUE>(this->strings, node.value);
	return StatusCode::SUCCESS;
}

template <bool SHOULD_APPEND_LOAD_OPCODE>
StatusCode ConditionBlock::append_number(const Node& node)
{
	typename decltype(this->numbers)::value_type val;
	RETURN_RESULT_IF(StatusCode::SUCCESS, !=, node.get_value(val));

	if constexpr (SHOULD_APPEND_LOAD_OPCODE)
		append_immediate<Control::Opcode::LOAD_NUMBER>(this->numbers, val);
	else
		append_immediate<Control::Opcode::MAX_VALUE>(this->numbers, val);

	return StatusCode::SUCCESS;
}

template <ConditionBlock::Control::Opcode LOAD_OPOCDE>
void ConditionBlock::append_immediate(auto& immediate_list, const auto& val)
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