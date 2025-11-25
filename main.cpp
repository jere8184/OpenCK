
#include "parser/prdx_parser.h"
#include "simulator/traits.hpp"

#include <filesystem>
#include <vector>
#include <ranges>
#include <execution>
#include <tuple>
#include <chrono>
#include <print>
#include <map>
#include <print>
#include <utility>

using openck::parser::Node;

void test_conditional()
{    
}

std::vector<std::filesystem::path> get_text_files(const std::filesystem::path& path)
{
    std::vector<std::filesystem::path> result;
    std::filesystem::directory_iterator religions_directory_iterator(path);
    for(const std::filesystem::directory_entry& directory_entry : religions_directory_iterator)
    {
        if (directory_entry.is_regular_file() && directory_entry.path().extension() == ".txt")
            result.push_back(directory_entry.path());
    }
    return result;
}

using PathNodesPair = std::pair<std::filesystem::path, std::vector<Node>>;

std::vector<PathNodesPair> parse_files(const std::vector<std::filesystem::path>& pahts)
{
    std::vector<PathNodesPair> result;
    for (const std::filesystem::path& file_path : pahts)
    {
        std::vector<Node> nodes;
        if (openck::parser::generate_nodes(file_path, nodes)) 
            result.emplace_back(file_path, std::move(nodes));
        else
            std::println(stderr, "failed to parse {}", file_path.string());
    }
    return result;
}

template <typename ObjectType>
void allocate_objects(const std::vector<Node>& nodes)
{
    ObjectType::allocate_range(nodes);
}

template <typename ObjectType>
bool initalise_objects(const std::vector<Node>& nodes)
{
    return ObjectType::initalise_range(nodes);
}

template <typename ObjectType>
std::vector<PathNodesPair> allocate_objects(const std::filesystem::path& folder_path)
{
    std::vector<std::filesystem::path> files = get_text_files(folder_path);
    std::vector<PathNodesPair> pairs = parse_files(files);
    std::ranges::for_each(pairs, [](const PathNodesPair p){allocate_objects<ObjectType>(p.second);});
    return pairs;
}


int main()
{
    const auto& religion_group_nodes = allocate_objects<openck::simulator::ReligionGroup>("./ck2_dir/common/religions");

    const auto& trait_nodes = allocate_objects<openck::simulator::Trait>("./ck2_dir/common/traits");

    std::ranges::for_each(trait_nodes, [](PathNodesPair pair){initalise_objects<openck::simulator::Trait>(pair.second);});
    
    return 0;
}