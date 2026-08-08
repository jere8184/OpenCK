
#pragma once

#include "Parser/PrdxParser.hpp"

#include "Utils/StatusCode.hpp"

namespace openck::util
{

struct Colour
{
	float red;
	float blue;
	float green;

	static StatusCode create_from_node(const parser::Node& node, Colour& colour)
	{
		if ((node.children.at(0).get_value(colour.red) == StatusCode::SUCCESS) &&
			(node.children.at(1).get_value(colour.blue) == StatusCode::SUCCESS) &&
			(node.children.at(2).get_value(colour.green) == StatusCode::SUCCESS))
		{
			return StatusCode::SUCCESS;
		}
		else
		{
			return StatusCode::FAILURE;
		}
	}
};

}