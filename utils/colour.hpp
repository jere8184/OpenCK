
#pragma once

#include "parser/prdx_parser.h"

namespace openck::util 
{

struct Colour
{
    float red;
    float blue;
    float green;

    static bool create_from_node(const parser::Node& node, Colour& colour) 
    {
        return  node.children.at(0).get_value(colour.red) && 
                node.children.at(1).get_value(colour.blue) &&
                node.children.at(2).get_value(colour.green);
    }
};

}