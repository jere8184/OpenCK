


#include "condition.hpp"

#include "religion.hpp"

#include "charecter.hpp"

namespace openck 
{

bool ConditionBlock::execute()
{
	while(advance())
	{
		switch (static_cast<OpCode>(*(this->ip)))
		{
			case OpCode::SCOPE_CHARECTER:
				//load charecter in this->root? 
				execute_charecter();
				break;
		}
	}
}

bool ConditionBlock::execute_charecter()
{
	simulator::Charecter* root = static_cast<simulator::Charecter*>(this->root); 
	bool result;

	while (advance())
	{
		switch (static_cast<CharecterOpCode>(*(this->ip)))
		{
		case CharecterOpCode::CONTROLS_RELIGION:
			result = controls_religion(root);
			break;
		
		case CharecterOpCode::RELIGION:
			result = religion(root);

		case CharecterOpCode::SCOPE_ANY_OWNED_BLOODLINE:
			result = any_owned_bloodline(root);
		
		default:
			break;
		}

		// needs to be switch
		StatusCode status_code = evaluate_result(result);
		if (status_code == StatusCode::SUCCESS)
			return true;
		else if (status_code == StatusCode::FAILURE)
			return false;
		else
			continue;
	}
	return false;
}

bool ConditionBlock::execute_bloodline()
{
	return false;
}

bool ConditionBlock::advance()
{
	return ++this->ip != this->instructions.end();
}

bool ConditionBlock::controls_religion(simulator::Charecter *charecter)
{
	simulator::Religion* religion = static_cast<simulator::Religion*>(this->load_pointer());
	return religion->get_head() == charecter;
}

bool ConditionBlock::religion(simulator::Charecter *charecter)
{
	simulator::Religion* religion = static_cast<simulator::Religion*>(this->load_pointer());
	return religion == charecter->religion;
}

bool ConditionBlock::any_owned_bloodline(simulator::Charecter *charecter)
{
	for (simulator::BloodLine* bloodline : charecter->bloodlines)
		execute_bloodline();
}

void* ConditionBlock::load_pointer()
{
	this->advance();
	return this->pointers[*(this->ip)];
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

}