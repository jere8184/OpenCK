
#include "prdx_parser.h"
#include "Simulator/Traits.hpp"

int main()
{
    std::vector<openck::parser::Node> nodes;
    openck::parser::generate_nodes("C:/Program Files (x86)/Steam/steamapps/common/Crusader Kings II/common/traits/00_traits.txt", nodes);
    openck::generate_traits_from_nodes(nodes); 
}