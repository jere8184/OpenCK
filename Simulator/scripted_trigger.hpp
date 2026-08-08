#pragma once

#include "base.hpp"

namespace openck::simulator 
{

struct ScriptedTrigger : Base<ScriptedTrigger>
{
	// As scripted triggers are essentially inline functions, they must be compiled inplace by the invokeing script.
	const Node* condition_block_source; 

	bool initalise_impl(const Node& node);

	ScriptedTrigger(const std::string& name);
};

}