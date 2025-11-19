
#include "prdx_parser.h"
#include "simulator/traits.hpp"


void test_conditional()
{    
}

int main()
{
    std::vector<openck::parser::Node> nodes;
    openck::parser::generate_nodes("00_traits.txt", nodes);
    openck::simulator::generate_traits_from_nodes(nodes); 
}