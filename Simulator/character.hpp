#pragma once

#include "title.hpp"
#include "date.hpp"
#include "attributes.hpp"
#include "trait.hpp"
#include "holding.hpp"
#include "religion.hpp"

#include <set>
#include <unordered_map>
#include <array>
#include <string>
#include <vector>

namespace openck::simulator
{

	struct Army;

	struct Abilities
	{
		bool can_change_religion = true;
		bool can_call_crusade = false;
		bool can_copy_personality_trait_from = false;
		bool can_grant_title = false;
		bool can_have_more_leadership_traits = false;
		bool can_hold_title = false;
		bool can_join_society = false;
	};

	struct Family
	{
		Character* farther = nullptr;
		Character* real_farther = nullptr;
		Character* mother = nullptr;
		std::set<Character*> children = {};
	};

	struct Relations
	{
		std::set<Character*> friends = {};
		std::set<Character*> rivals = {};
		std::set<Character*> lovers = {};
		std::set<Character*> kills = {};
	};

	struct Mana
	{
		int gold = 0;
		int prestige = 0;
		int piety = 0;
	};

	enum class Gender
	{
		DEFAULT = -1,
		MALE = 0,
		FEMALE = 1
	};

	struct Character
	{
		Mana mana = {};
		Stats stats = {};
		Family family = {};
		Title* primary_title = nullptr;
		std::vector<Title*> titles = {};
		int id = -1;
		std::string first_name = "";
		std::string middle_name = "";
		std::string last_name = "";
		std::string nickname = "";
		Gender gender = Gender::DEFAULT;
		int age = -1;
		Date dateOfBirth ={};
		Date dateOfDeath = {};
		Holding* location = nullptr;

		Religion* religion = nullptr;
		Religion* secret_religion = nullptr;

		std::vector<BloodLine*> bloodlines;

		Attributes attributes = {};
		bool ai = false;

		std::set<const Title*> strong_claims;
		std::set<const Title*> weak_claims;
		std::set<const Title*> all_claims;

		std::vector<Army*> controlled_armies;

		bool can_copy_personality_trait_from(Character* someone_elese) const;
		bool can_grant_title(Character* someone_else) const;

		Religion* get_religion() const {return nullptr;};
		bool controls_religion() const {return false;}

		// house
		// culture
		// religion
		// bloodline
		// artifacts
		// genes
		static std::array<Character, 1000> character_list;
	};

	class Ruler : Character
	{

	};

}
