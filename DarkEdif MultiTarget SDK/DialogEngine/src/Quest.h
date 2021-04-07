#pragma once
#include <string>
#include <vector>
#include "Header.h"
#include "GameFlag.h"
#include "GameItem.h"
#include "GameFloat.h"

class Quest {
public:
    // Fields
    std::string m_name = "UNDEFINED";
    std::string m_description = "";
    QuestState m_questState = QuestState::INACTIVE;
    std::vector<GameFlag> m_requiredFlags;
    std::vector<GameItem> m_requiredItems;
    std::vector<GameItem> m_rewardItems;
    std::vector<GameFloat> m_rewardFloats;
    bool m_paidOut = false;
    bool m_requirementsFullfiled = false;

    // Methods
    void CheckRequirements(std::vector<GameFlag>& engineFlags, std::vector<GameItem>& engineItems);
};