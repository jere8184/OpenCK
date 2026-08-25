
#pragma once

#include <string>
#include <vector>
#include <set>
#include "Religion.hpp"
#include "Culture.hpp"
#include "Scripting/Condition.hpp"
#include "Base.hpp"
#include "Character.hpp"
#include "Utils/StatusCode.hpp"

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
	static void init_field_setters()
	{
		Base<Event>::field_setters = 
		{
			{"id", [](Event& event, const Node& node){event.m_idStr = node.value; return StatusCode::SUCCESS;}},
			{"desc", [](Event& event, const Node& node){event.m_flags.m_desc = node.value; return StatusCode::SUCCESS;}},
			{"picture", [](Event& event, const Node& node){event.m_flags.m_picture = node.value; return StatusCode::SUCCESS;}},
			{"border", [](Event& event, const Node& node){event.m_flags.m_border = node.value; return StatusCode::SUCCESS;}},
			{"hide_from", [](Event& event, const Node& node){return node.GetValue(event.m_flags.m_hideFrom);}},	

			{"trigger", 
				[](Event& event, const Node& node)
				{
					event.m_triggerConditionBlock.Compile(node, event.m_scope);
					std::string output;
					event.m_triggerConditionBlock.Decompile<scripting::ConditionBlock::CharacterScope>(output);
					std::println("{} trigger: {}", event.m_idStr, output);
					return StatusCode::SUCCESS;
				}
			},
			{"immediate", [](Event& event, const Node& node)
				{
					event.m_immediateConditionBlock.Compile(node, event.m_scope);
					std::string output;
					event.m_immediateConditionBlock.Decompile<scripting::ConditionBlock::CharacterScope>(output);
					std::println("{} immediate: {}", event.m_idStr, output);
					return StatusCode::SUCCESS;
				}
			},

			{"capable_only", [](Event& event, const Node& node){return node.GetValue(event.m_preTriggers.m_onlyCapable);}},
			{"only_capable", [](Event& event, const Node& node){return node.GetValue(event.m_preTriggers.m_onlyCapable);}},

			{"portrait", [](Event& event, const Node& node){event.m_flags.m_portrait = node.value; return StatusCode::SUCCESS;}},
			{"major", [](Event& event, const Node& node){node.GetValue(event.m_flags.m_major); return StatusCode::SUCCESS;}},
			{"is_friendly", [](Event& event, const Node& node){return node.GetValue(event.m_flags.m_isFriendly);}},
			{"is_hostile", [](Event& event, const Node& node){return node.GetValue(event.m_flags.m_isHostile);}},
			{"is_triggered_only", [](Event& event, const Node& node){return node.GetValue(event.m_flags.m_isTriggeredOnly);}},
			{"offmap", [](Event& event, const Node& node){event.m_flags.m_offmap = node.value; return StatusCode::SUCCESS;}},
			{"triggered_from_code", [](Event& event, const Node& node){return node.GetValue(event.m_flags.m_triggeredFromCode);}},
			{"hide_new", [](Event& event, const Node& node){return node.GetValue(event.m_flags.m_hideNew);}},
			{"hide_window", [](Event& event, const Node& node){return node.GetValue(event.m_flags.m_hideWindow);}},
			{"show_root", [](Event& event, const Node& node){return node.GetValue(event.m_flags.m_showRoot);}},
			{"show_from_from", [](Event& event, const Node& node){return node.GetValue(event.m_flags.m_showFromFrom);}},
			{"show_from_from_from", [](Event& event, const Node& node){return node.GetValue(event.m_flags.m_showFromFromFrom);}},
			{"sound", [](Event& event, const Node& node){event.m_flags.m_sound = node.value; return StatusCode::SUCCESS;}},
			{"notification", [](Event& event, const Node& node){return node.GetValue(event.m_flags.m_notification);}},

			{"ai", [](Event& event, const Node& node){return node.GetValue(event.m_filteringPreTriggers.m_ai);}},
			{"is_patrician", [](Event& event, const Node& node){return node.GetValue(event.m_filteringPreTriggers.m_isPatrician);}},
			{"only_independent", [](Event& event, const Node& node){return node.GetValue(event.m_filteringPreTriggers.m_onlyIndependent);}},
			{"only_playable", [](Event& event, const Node& node){return node.GetValue(event.m_filteringPreTriggers.m_onlyPlayable);}},
			{"is_part_of_plot", [](Event& event, const Node& node){return node.GetValue(event.m_filteringPreTriggers.m_isPartOfPlot);}},
			{"only_rulers", [](Event& event, const Node& node){return node.GetValue(event.m_filteringPreTriggers.m_onlyRulers);}},
			{"religion", [](Event& event, const Node& node){return node.GetValue(event.m_filteringPreTriggers.m_religion);}},
			{"religion_group", [](Event& event, const Node& node){return node.GetValue(event.m_filteringPreTriggers.m_religionGroup);}}
		};
	}

	static void init_static_objects() {};
	

	struct FilteringPreTriggers
	{
		bool m_ai;
		bool m_isPatrician;
		bool m_onlyIndependent;
		bool m_onlyPlayable;
		bool m_isPartOfPlot;
		bool m_onlyRulers;
		Religion::Id m_religion;
		ReligionGroup::Id m_religionGroup;
	};

	struct PreTriggers
	{
		int m_minAge;
		int m_maxAge;
		bool m_onlyMen;
		bool m_onlyCapable;
		//bool m_capableOnly;
		std::string m_lacksDlc;
		std::string m_hasDlc;
		bool m_friends;
		bool m_rivals;
		bool m_prisoner;
		bool m_inCommand;
		bool m_isFemale;
		bool m_isMarried;
		bool m_isSick;
		//flag m_hasCharacterFlag;
		//flag m_hasGlobalFlag;
		bool m_war;
		Culture::Id m_culture;
		//m_cultureGroup;
		bool m_isInSociety;
		bool m_hasQuestTarget;
		bool m_hasJobTitle;
	};

	struct Flags
	{
		std::string /*LocalizationKey*/ m_title;	
		std::string /*LocalizationKeyOrclause*/ m_desc	; 
		std::string /*gui*/ m_window; 
		std::string /*gfx*/ m_background; 
		std::string /*gfx*/ m_picture; 
		std::string /*gfx*/ m_border; 
		std::string /*character/title/offmap/none*/ m_portrait; 
		bool m_major; 
		bool m_isFriendly; 
		bool m_isHostile; 
		bool m_isTriggeredOnly; 
		std::string m_offmap; 
		bool m_triggeredFromCode; 
		bool m_hideFrom; 
		bool m_hideNew; 
		bool m_hideWindow; 
		bool m_showRoot; 
		bool m_showFromFrom; 
		bool m_showFromFromFrom; 
		std::string /*sfx*/ m_sound; 
		bool m_notification; 
	};

	Event(std::string name) : 
		Base<Event>::Base(name)
	{
		if (("character_event" == name) || ("long_character_event" == name) || ("letter_event" == name) || ("narrative_event" == name) ||
			("diploresponse_event" == name))
			m_scope = scripting::ConditionBlock::AnyScope::ConditionOpcode::CHARECTER_SCOPE;
		else if ("province_event" == name)
			;
		else if ("unit_event" == name)
			;
		else if ("society_quest_event" == name)
			;
	};

	FilteringPreTriggers m_filteringPreTriggers;
	PreTriggers m_preTriggers;
	Flags m_flags;
	std::string m_idStr;
	openck::scripting::ConditionBlock m_triggerConditionBlock;
	openck::scripting::ConditionBlock m_immediateConditionBlock;
	scripting::ConditionBlock::AnyScope::ConditionOpcode m_scope;
};

}
