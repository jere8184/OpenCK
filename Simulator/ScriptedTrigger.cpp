
#include "ScriptedTrigger.hpp"


namespace openck::simulator 
{

bool ScriptedTrigger::initalise_impl(const Node& node)
{
	this->condition_block_source = &node;
	return true;
}

ScriptedTrigger::ScriptedTrigger(const std::string& name) : 
	Base(name)
{

}

}