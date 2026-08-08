
#include "Parser/PrdxParser.hpp"
#include "Simulator/Trait.hpp"
#include "Simulator/Army.hpp"
#include "Simulator/ScriptedTrigger.hpp"

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

std::vector<PathNodesPair> parse_files(const std::vector<std::filesystem::path>& paths)
{
	std::vector<PathNodesPair> result;
	for (const std::filesystem::path& file_path : paths)
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
bool initalise_objects(const std::vector<PathNodesPair>& nodes)
{
	bool was_success = true;
	std::ranges::for_each(
	nodes, 
	[&was_success](const PathNodesPair& pair)
	{
		std::println("Initalising objects from <{}>", pair.first.generic_string());
		
		if (ObjectType::initalise_range(pair.second))
			was_success = false;
	});

	return was_success;
}

template <typename ObjectType>
std::vector<PathNodesPair> allocate_objects(const std::filesystem::path& folder_path)
{
	std::vector<std::filesystem::path> files = get_text_files(folder_path);
	std::vector<PathNodesPair> pairs = parse_files(files);
	std::ranges::for_each(pairs, [](const PathNodesPair& p){ObjectType::allocate_range(p.second);});
	return pairs;
}

int main()
{
	std::setlocale(LC_ALL, "en_US.UTF-8");
	openck::simulator::Trait::initalise_static_fields();
	openck::simulator::Religion::initalise_static_fields();
	openck::simulator::UnitType::initalise_static_fields();

	const std::vector<PathNodesPair>& religion_group_nodes = allocate_objects<openck::simulator::ReligionGroup>("./ck2_dir/common/religions/");
	const std::vector<PathNodesPair>& trait_nodes = allocate_objects<openck::simulator::Trait>("./ck2_dir/common/traits/");
	const std::vector<PathNodesPair>& scripted_trigger_nodes = allocate_objects<openck::simulator::ScriptedTrigger>("./ck2_dir/common/scripted_triggers/");
	

	initalise_objects<openck::simulator::ScriptedTrigger>(scripted_trigger_nodes);
	initalise_objects<openck::simulator::Trait>(trait_nodes);

	return 0;
}