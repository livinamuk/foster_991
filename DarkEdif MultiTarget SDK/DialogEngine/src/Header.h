#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

enum class SaveCategory {
	NONE,
	FLAGS,
	VALUES,
	ITEMS,
	QUESTS
};


enum class QuestState {
	INACTIVE,
	ACTIVE,
	COMPLETE,
	FAILED,
	UNDEFINED
};

enum class ConditionType {
	CONDITION_UNDEFINED,
	HAVE_ITEM,
	BOOOL,
	FLOAT_EQUAL,
	FLOAT_LESS,
	FLOAT_GREATER,
	FLOAT_LESS_OR_EQUAL,
	FLOAT_GREATER_OR_EQUAL,
	FLOAT_NOT_EQUAL,
	QUEST_REQUIREMENTS_MET,
	QUEST_FAILED,
	QUEST_COMPLETE,
	QUEST_ACTIVE,
	QUEST_INACTIVE
};

enum class ActionType {
	ACTION_UNDEFINED,
	SET_BOOL_TRUE,
	SET_BOOL_FALSE,
	SUBRACT_FLOAT,
	ADD_FLOAT,
	SET_FLOAT,
	GIVE_ITEM,
	TAKE_ITEM
};

struct MyCondition {
public:
	std::string m_conditionName = "";
	ConditionType m_ConditionType = ConditionType::CONDITION_UNDEFINED;
	float m_conditionComparisonValue = 0;
	bool m_requiredConditionBoolState = false;
};

struct ResponseAction {
public:
	std::string m_name;
	ActionType m_actionType = ActionType::ACTION_UNDEFINED;
	float m_modifierValue = 0;
};

struct Response {

public:
	std::string m_text = "";
	std::vector<std::string> m_fusionActions;
	std::vector<std::string> m_CompleteQuests;
	std::vector<std::string> m_ActivateQuests;
	std::vector<std::string> m_FailQuests;
	std::vector<MyCondition> m_responseConditions;
	std::vector<ResponseAction> m_responseActions;
	bool m_isSpecialColor = false;
	int m_gotoID = -1;
};

struct DialogEntry {
public:
	int m_ID = -1;
	std::string m_imageName = "NO IMAGE";
	std::string m_text = "NO TEXT";
	std::vector<Response> m_responses;
	std::string m_npcTriggerName;
	std::vector<MyCondition> m_conditions;
};

/*
struct InWorldNPCDialogEntry {
	std::string m_npcName = "UNDEFINED";
	std::vector<std::string> m_replies;
};*/