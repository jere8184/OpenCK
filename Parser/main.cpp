
#include "prdx_parser.h"
#include "simulator/traits.hpp"

int main()
{
    std::vector<openck::parser::Node> nodes;
    openck::parser::generate_nodes("01_traits.txt", nodes);
    openck::simulator::generate_traits_from_nodes(nodes); 
}