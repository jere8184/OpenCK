
#include "prdx_parser.h"
#include "simulator/traits.hpp"

#include <filesystem>
#include <vector>
#include <ranges>
#include <execution>
#include <tuple>
#include <chrono>
#include <print>

void test_conditional()
{    
}



int main()
{
    using GenerateNodesArgs = std::tuple<std::filesystem::path, std::vector<openck::parser::Node>, bool>;
    std::vector<GenerateNodesArgs> args_vec;

    std::filesystem::directory_iterator directory_iterator("./ck2_dir/common/religions");
    for(const std::filesystem::directory_entry& directory_entry : directory_iterator)
    {
        if (directory_entry.is_regular_file() && directory_entry.path().extension() == ".txt")
            args_vec.push_back({directory_entry.path(), {}, true});
    }

    /*std::chrono::high_resolution_clock clock;
    auto start = clock.now();
    std::for_each(args_vec.begin(), args_vec.end(), [](GenerateNodesArgs& args){std::get<2>(args) = openck::parser::generate_nodes(std::get<0>(args), std::get<1>(args));});
    auto end = clock.now();

    auto duration = duration_cast<std::chrono::microseconds>(end - start);

    std::println("{}", duration);*/

    for (auto& [path, nodes, was_success] : args_vec)
    {
        was_success = openck::parser::generate_nodes(path, nodes);
        if (was_success)
        {
            openck::simulator::ReligionGroup::allocate_range(nodes);
        }
    }

    for (auto& [path, nodes, was_success] : args_vec)
    {
        if (was_success)
            was_success = openck::simulator::ReligionGroup::initalise_range(nodes);
    }
}