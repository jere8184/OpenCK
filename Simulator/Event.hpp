
#pragma once

#include <string>
#include <vector>
#include <set>
#include "scripting/condition.hpp"
#include "base.hpp"
#include "character.hpp"

namespace openck::simulator
{

struct Actor;

struct Condition {};
struct Effects {};

struct Option
{
	std::string name;
	std::string text;
	Condition condition;
	Effects effects;
};


struct Event : Base<Event>
{
	openck::scripting::ConditionBlock m_conditionBlock;

	static void init_field_setters()
	{
		field_setters = 
		{
			{"id", [](Event* event, const Node& node){event->id = node.value; return StatusCode::SUCCESS;}},
			{"desc", [](Event* event, const Node& node){event->desc = node.value; return StatusCode::SUCCESS;}},
			{"picture", [](Event* event, const Node& node){event->picture = node.value; return StatusCode::SUCCESS;}},
			{"border", [](Event* event, const Node& node){event->border = node.value; return StatusCode::SUCCESS;}},

			{"trigger", [](Event* event, const Node& node){return event->m_conditionBlock.compile<scripting::characterScope>(node);}},

			{"hide_from", [](Event* event, const Node& node){return node.get_value(event->hide_from);}},
			{"capable_only", [](Event* event, const Node& node){return node.get_value(event->only_capable);}},
			{"only_capable", [](Event* event, const Node& node){return node.get_value(event->only_capable);}},
			{"capable_only", [](Event* event, const Node& node){return node.get_value(event->only_capable);}},
			{"only_capable", [](Event* event, const Node& node){return node.get_value(event->only_capable);}},
		};
	}
	
	std::string			id;
	std::string			desc;
	std::string			picture;
	std::string			border;
	
	bool				is_ai;
	bool				is_patrician;
	bool				only_independent;
	bool				only_playable;
	bool				is_part_of_plot;
	bool				only_rulers;
	Religion::Id		religion;
	ReligionGroup::Id	religion_group;
	bool				hide_from;
	bool				prisoner;
	bool				only_rulers;
	bool				only_capable;

	Character::Age		min_age;
};

}
