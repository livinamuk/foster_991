#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

enum class QuestState {
	INACTIVE,
	ACTIVE,
	COMPLETE,
	FAILED,
	UNDEFINED
};

enum ConditionType {
	CONDITION_UNDEFINED,
	HAVE_ITEM,
	NO_ITEM,
	BOOOL,
	FLOAT_EQUAL,
	FLOAT_LESS,
	FLOAT_GREATER,
	FLOAT_LESS_OR_EQUAL,
	FLOAT_GREATER_OR_EQUAL,
	FLOAT_NOT_EQUAL,
	QUEST_REQUIREMENTS_MET
};

enum ActionType {
	ACTION_UNDEFINED,
	SET_BOOL_TRUE,
	SET_BOOL_FALSE,
	SUBRACT_FLOAT,
	ADD_FLOAT,
	SET_FLOAT,
	GIVE_ITEM,
	TAKE_ITEM
};

struct ResponseCondition {
public:
	std::string m_conditionName = "";
	ConditionType m_ConditionType;
	float m_conditionComparisonValue = 0;
	bool m_requiredConditionBoolState = false;
};

struct ResponseAction {
public:
	std::string m_name;
	ActionType m_actionType;
	float m_modifierValue;
};

struct Response {

public:
	std::string m_text = "";
	std::vector<std::string> m_fusionActions;
	std::vector<std::string> m_CompleteQuests;
	std::vector<std::string> m_ActivateQuests;
	std::vector<std::string> m_FailQuests;
	std::vector<ResponseCondition> m_responseConditions;
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
};