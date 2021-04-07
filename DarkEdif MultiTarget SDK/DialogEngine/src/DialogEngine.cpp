
#include "DialogEngine.h"

std::vector<GameItem> DialogEngine::s_gameItems;
std::vector<GameFlag> DialogEngine::s_gameFlags;
std::vector<GameFloat> DialogEngine::s_gameFloats;
std::vector<DialogEntry> DialogEngine::s_dialogEntries;
std::vector<Response> DialogEngine::s_avaliableResponses;
std::vector<std::string> DialogEngine::s_pendingFusionActions; // these aren't finished yet
DialogEntry DialogEngine::s_currentDialog;
int DialogEngine::s_lastSelectedResponseIndex;
std::vector<Quest> DialogEngine::s_Quests;
bool DialogEngine::s_wasAnyQuestJustCompleted = false;
bool DialogEngine::s_wasAnyQuestJustFailed = false;


////////////////////////////////
//                            //
//   F I L E   L O A D I N G  // 
//                            //
//////////////////////////////// and saving...


void DialogEngine::SavePlayerFile(std::string filename)
{
	// Create and open a text file
	std::ofstream MyFile(filename);

	// Save flags
	for (GameFlag& gameFlag : s_gameFlags) {
		if (gameFlag.m_state)
			MyFile << gameFlag.m_name << "=true\n";
		else
			MyFile << gameFlag.m_name << "=false\n";
	}

	// Save floats
	for (GameFloat& gameFloat : s_gameFloats) {
		MyFile << gameFloat.m_name << "=" << (int)gameFloat.m_value << "\n";
	}

	// Save items
	for (GameItem& gameItem : s_gameItems) {
		MyFile << gameItem.m_name << "=give" << gameItem.m_quantity << "\n";
	}

	// Close the file
	MyFile.close();
}

void DialogEngine::LoadQuestFile(std::string filename)
{
	std::ifstream file(filename);
	std::string line;
	Quest tempQuest = Quest();
	while (getline(file, line))
	{
		// Found a new Quest? Reset and store name
		if (Util::StringContains(line, "NAME:"))
		{
			// You're working on a new one Quest now so push the last one into the array.
			if (tempQuest.m_name != "UNDEFINED") {
				s_Quests.push_back(tempQuest);
				// reset for reuse
				tempQuest.m_name = "";
				tempQuest.m_description = "";
				tempQuest.m_requiredFlags.clear();
				tempQuest.m_requiredItems.clear();
				tempQuest.m_rewardItems.clear();
				tempQuest.m_rewardFloats.clear();
			}
			tempQuest.m_name = Util::GetSubstringAfterColon(line);
		}

		else if (Util::StringContains(line, "DESCRIPTION:"))
			tempQuest.m_description = Util::GetSubstringAfterColon(line);
		
		// Requirement flag
		else if (Util::StringContains(line, "REQUIREMENT_FLAG_TRUE:"))
			tempQuest.m_requiredFlags.push_back(GameFlag(Util::GetSubstringAfterColon(line), true));

		else if (Util::StringContains(line, "REQUIREMENT_FLAG_FALSE:"))
			tempQuest.m_requiredFlags.push_back(GameFlag(Util::GetSubstringAfterColon(line), false));

		// Requirement item
		else if (Util::StringContains(line, "REQUIREMENT_ITEM:"))
			tempQuest.m_requiredItems.push_back(GameItem(Util::GetSubstringAfterColon(line), 1));

		// Reward Item
		else if (line.substr(0, 12) == "REWARD_ITEM:")
		{
			std::string str = Util::GetSubstringAfterColon(line);
			std::string itemName = Util::GetSubstringBeforeParentheses(str);
			int quantity = (int)Util::GetFloatBetweenParentheses(str);
			if (quantity == 0)
				quantity = 1;

			tempQuest.m_rewardItems.push_back(GameItem(itemName, quantity));
		//	std::cout << " " << itemName << " [" << quantity << "]\n";
		}

		// Reward Float		
		else if (line.substr(0, 13) == "REWARD_VALUE:")
		{
			std::string str = Util::GetSubstringAfterColon(line);
			std::string valueName = Util::GetSubstringBeforeParentheses(str);
			int quantity = (int)Util::GetFloatBetweenParentheses(str);

			tempQuest.m_rewardFloats.push_back(GameFloat(valueName, quantity));
			//std::cout << " " << valueName << " " << "[" << quantity << "]\n";
		}
	}

	// You're at the end of file now, if you're working with a quest you haven't added yet, do so.
	if (tempQuest.m_name != "UNDEFINED")
		s_Quests.push_back(tempQuest);

	file.close();
}

void DialogEngine::LoadPlayerFile(std::string filename)
{
	std::ifstream file(filename);
	std::string line;
	while (getline(file, line))
	{
		int colonPos = line.find('=');
		std::string name = line.substr(0, colonPos);
		std::string value = line.substr(colonPos + 1); // everything after the =

		// FIRST check if it is an item. Which is in the format: give=itemnameX and X is the quantity
		if (value.find("give") != std::string::npos)
		{
			// Check quantity
			int quantity = Util::StringToInt(value.substr(4));
			if (quantity == 0)
				quantity = 1;

			s_gameItems.push_back(GameItem(name, quantity));
			continue;
		}
		// Is it a boolean?
		if (value.find("true") != std::string::npos)
			SetGameFlag(name, true);
		else if (value.find("false") != std::string::npos)
			SetGameFlag(name, false);
		// Is it a float
		else {
			SetGameFloat(name, std::stof(value));
		}
	}
}


void DialogEngine::LoadDialogFile(std::string filename)
{
	std::ifstream file(filename);
	std::string line;
	DialogEntry tempDialogEntry = DialogEntry();

	while (getline(file, line))
	{
		// New dialog entry
		if (line[0] == '#')
		{
			// Add the last dialog entry to the vector, cause you're working on a new one now.
			if (tempDialogEntry.m_text != "NO TEXT") {
				s_dialogEntries.push_back(tempDialogEntry);
				// reset for reuse
				tempDialogEntry.m_ID = -1;
				tempDialogEntry.m_imageName = "NO IMAGE";
				tempDialogEntry.m_text = "NO TEXT";
				tempDialogEntry.m_responses.clear();
			}

			// ID
			std::string ID_string = line.substr(1, line.length() - 1);
			int ID = Util::StringToInt(ID_string);
			tempDialogEntry.m_ID = ID;
		}

		// Set image name
		if (line.substr(0, 3) == "IMG") {
			tempDialogEntry.m_imageName = line.substr(5, line.length() - 1);
		}

		// Set text
		if (line.substr(0, 4) == "TEXT") {
			tempDialogEntry.m_text = line.substr(6, line.length() - 1);
		}

		// Add response
		if (line.substr(0, 5) == "REPLY")
		{
			std::string wholeLine = line.substr(7, line.length() - 1);
			Response response;

			// Check for no special shit...
			if (wholeLine.find('[') == std::string::npos) {
				response.m_text = wholeLine;
			}
			// Well I guess it has some then...
			else
			{
				// First isolate the main text;
				response.m_text = wholeLine.substr(0, wholeLine.find('['));

				// Now isolate any special tags
				std::vector<std::string> tags;
				bool finished = false;
				while (!finished)
				{
					int begin = wholeLine.find('[');
					int end = wholeLine.find(']');
					std::string tag = wholeLine.substr(begin + 1, end - begin - 1);
					wholeLine = Util::RemoveFromBeginning(wholeLine, end + 1); // delete everything before the found closing tag
					tags.push_back(tag);

					if (wholeLine.find(']') == std::string::npos)
						finished = true;
				}

				// Now loop over those tags and "process" them
				for (std::string tag : tags)
				{
					std::string leftSide = Util::GetSubstringBeforeColon(tag);
					std::string rightSide = Util::GetSubstringAfterColon(tag);

					//	std::cout << " " << commandName << '\n';

						//if (commandName == "COMPLETE_QUEST" || commandName == "ACTIVATE_QUEST" || commandName == "FAIL_QUEST")
						//	std::cout << " [" << commandName << "] [" << parameter << "]\n";

					if (leftSide == "COMPLETE_QUEST")
						response.m_CompleteQuests.push_back(rightSide);

					else if (leftSide == "ACTIVATE_QUEST")
						response.m_ActivateQuests.push_back(rightSide);

					else if (leftSide == "FAIL_QUEST")
						response.m_FailQuests.push_back(rightSide);

					// Fusion actions
					else if (leftSide == "FUSION_ACTION")
						response.m_fusionActions.push_back(rightSide);

					// Engine Actions
					else if (leftSide == "ACTION")
					{
						std::string name = Util::GetSubstringBeforeEquals(rightSide);
						std::string actionRightSide = Util::GetSubstringAfterEquals(rightSide);
						int quantity = (int)Util::GetFloatBetweenParentheses(actionRightSide);

						ResponseAction responseAction;
						responseAction.m_name = name;

						// Give item
						if (Util::StringContains(actionRightSide, "give_item")) {
							if (quantity == 0)
								quantity = 1;
							responseAction.m_actionType = ActionType::GIVE_ITEM;
							responseAction.m_modifierValue = quantity;
						}

						// Take item
						else if (Util::StringContains(actionRightSide, "take_item")) {
							if (quantity == 0)
								quantity = 99999;

							responseAction.m_actionType = ActionType::TAKE_ITEM;
							responseAction.m_modifierValue = quantity;
						}

						// Bool action
						else if (actionRightSide == "true")
							responseAction.m_actionType = ActionType::SET_BOOL_TRUE;
						else if (actionRightSide == "false")
							responseAction.m_actionType = ActionType::SET_BOOL_FALSE;

						// Set value. You can reuse your earlier "equals detection"
						else if (Util::StringContains(rightSide, "=")) {
							responseAction.m_modifierValue = Util::StringToFloat(actionRightSide);
							responseAction.m_actionType = ActionType::SET_FLOAT;

							//std::cout << " [" << name << "] = [" << responseAction.m_modifierValue << "]\n";
						}
						// Add to value
						else if (Util::StringContains(rightSide, "+")) {
							name = Util::GetSubstringBeforeChar(rightSide, '+');
							responseAction.m_name = name;
							responseAction.m_modifierValue = Util::StringToFloat(Util::GetSubstringAfterChar(rightSide, '+'));
							responseAction.m_actionType = ActionType::ADD_FLOAT;

							//	std::cout << " [" << name << "] + [" << responseAction.m_modifierValue << "]\n";
						}
						// Subtract from value
						else if (Util::StringContains(rightSide, "-")) {
							name = Util::GetSubstringBeforeChar(rightSide, '-');
							responseAction.m_name = name;
							responseAction.m_modifierValue = Util::StringToFloat(Util::GetSubstringAfterChar(rightSide, '-'));
							responseAction.m_actionType = ActionType::SUBRACT_FLOAT;

							//::cout << " [" << name << "] - [" << responseAction.m_modifierValue << "]\n";
						}
						response.m_responseActions.push_back(responseAction);
					}

					// Conditions
					else if (leftSide == "CONDITION")
					{
						ResponseCondition responseCondition;
						std::string name;

						if (Util::StringContains(rightSide, "==")) {

							responseCondition.m_conditionName = Util::GetSubstringBeforeSubstring(rightSide, "==");
							std::string conditionParameter = Util::GetSubstringAfterSubstring(rightSide, "==");

							// Bool
							if (conditionParameter == "true") {
								responseCondition.m_requiredConditionBoolState = true;
								responseCondition.m_ConditionType = ConditionType::BOOOL;
							}
							else if (conditionParameter == "false") {
								responseCondition.m_requiredConditionBoolState = false;
								responseCondition.m_ConditionType = ConditionType::BOOOL;
							}
							// Quest requirements met
							else if (conditionParameter == "fullfilled") {
								responseCondition.m_requiredConditionBoolState = true;
								responseCondition.m_ConditionType = ConditionType::QUEST_REQUIREMENTS_MET;
							}
							else if (conditionParameter == "not_fullfilled") {
								responseCondition.m_requiredConditionBoolState = false;
								responseCondition.m_ConditionType = ConditionType::QUEST_REQUIREMENTS_MET;
							}
							// Item check							
							else if (conditionParameter == "have_item")
								responseCondition.m_ConditionType = ConditionType::HAVE_ITEM;

							else if (conditionParameter == "no_item")
								responseCondition.m_ConditionType = ConditionType::NO_ITEM;

							// Well if not then it's a float equality comparison
							else {
								responseCondition.m_ConditionType = ConditionType::FLOAT_EQUAL;
								responseCondition.m_conditionComparisonValue = Util::StringToFloat(conditionParameter);
							}

						}
						// Float not equal
						else if (Util::StringContains(rightSide, "!=")) {

							responseCondition.m_ConditionType = ConditionType::FLOAT_NOT_EQUAL;
							responseCondition.m_conditionName = Util::GetSubstringBeforeSubstring(rightSide, "!=");
							responseCondition.m_conditionComparisonValue = Util::StringToFloat(Util::GetSubstringAfterSubstring(rightSide, "!="));

							//		std::cout << " [" << responseCondition.m_conditionName << "] != [" << responseCondition.m_conditionComparisonValue << "]\n";

						}
						else if (Util::StringContains(rightSide, "<=")) {

							responseCondition.m_ConditionType = ConditionType::FLOAT_LESS_OR_EQUAL;
							responseCondition.m_conditionName = Util::GetSubstringBeforeSubstring(rightSide, "<=");
							responseCondition.m_conditionComparisonValue = Util::StringToFloat(Util::GetSubstringAfterSubstring(rightSide, "<="));
						}
						else if (Util::StringContains(rightSide, ">=")) {

							responseCondition.m_ConditionType = ConditionType::FLOAT_GREATER_OR_EQUAL;
							responseCondition.m_conditionName = Util::GetSubstringBeforeSubstring(rightSide, ">=");
							responseCondition.m_conditionComparisonValue = Util::StringToFloat(Util::GetSubstringAfterSubstring(rightSide, ">="));
						}
						else if (Util::StringContains(rightSide, "<")) {

							responseCondition.m_ConditionType = ConditionType::FLOAT_LESS;
							responseCondition.m_conditionName = Util::GetSubstringBeforeSubstring(rightSide, "<");
							responseCondition.m_conditionComparisonValue = Util::StringToFloat(Util::GetSubstringAfterSubstring(rightSide, "<"));
						}
						else if (Util::StringContains(rightSide, ">")) {

							responseCondition.m_ConditionType = ConditionType::FLOAT_GREATER;
							responseCondition.m_conditionName = Util::GetSubstringBeforeSubstring(rightSide, ">");
							responseCondition.m_conditionComparisonValue = Util::StringToFloat(Util::GetSubstringAfterSubstring(rightSide, ">"));
						}
						response.m_responseConditions.push_back(responseCondition);
					}

					// Special color
					else if (tag == "SPECIAL_COLOR")
						response.m_isSpecialColor = true;

					// Goto
					else if (tag.find("GOTO") != std::string::npos)
					{
						// Then find the fucking ID
						response.m_gotoID = Util::StringToInt(tag.substr(tag.find(':') + 1));
					}

				}
			}
			tempDialogEntry.m_responses.push_back(response);
		}
	}

	// Well add the dialog entry you were last processing, cause they are only added when the next one is found remember.
	if (tempDialogEntry.m_text != "NO TEXT")
		s_dialogEntries.push_back(tempDialogEntry);

	// Close the file
	file.close();
}



///////////////////////////////
//                           //
//    M I S C    S T U F F   //
//                           //
///////////////////////////////


DialogEntry DialogEngine::GetDialogByID(int ID)
{
	// Find match
	for (DialogEntry& dialogEntry : s_dialogEntries) {
		if (dialogEntry.m_ID == ID)
			return dialogEntry;
	}
	// Otherwise return an empty new one
	return DialogEntry();
}

void DialogEngine::SetCurrentDialogByID(int ID)
{
	s_currentDialog = DialogEngine::GetDialogByID(ID);

	// Determine which responses should be shown
	UpdateAvaliableResponses();
}

std::string DialogEngine::GetCurrentDialogText()
{
	return s_currentDialog.m_text;
}

std::string DialogEngine::GetCurrentDialogImageName()
{
	return s_currentDialog.m_imageName;
}

bool DialogEngine::EvaluateConditionalComparison(std::string name, ConditionType conditionType, float comparisonValue)
{
	// If item or value is not found, assume 0, and perform check...
	int objectValue = 0;

	// Otherwise, get the value
	if (HasItem(name))
		objectValue = GetItemQuantity(name);
	if (GameFloatExists(name))
		objectValue = GetGameFloat(name);

	// Evaluate comparison
	if (conditionType == ConditionType::FLOAT_GREATER_OR_EQUAL)
		return (objectValue >= comparisonValue);

	else if (conditionType == ConditionType::FLOAT_GREATER)
		return (objectValue > comparisonValue);

	else if (conditionType == ConditionType::FLOAT_LESS_OR_EQUAL)
		return (objectValue <= comparisonValue);

	else if (conditionType == ConditionType::FLOAT_LESS)
		return (objectValue < comparisonValue);

	else if (conditionType == ConditionType::FLOAT_EQUAL)
		return (objectValue == comparisonValue);

	else if (conditionType == ConditionType::FLOAT_NOT_EQUAL)
		return (objectValue != comparisonValue);

	else return false;
}

void DialogEngine::UpdateAvaliableResponses()
{
	// Reset
	s_avaliableResponses.clear();

	// Get avaliable responses
	for (Response& response : s_currentDialog.m_responses)
	{
		// Check if all conditions are met
		for (ResponseCondition& responseCondition : response.m_responseConditions)
		{
			// Bool conditions
			if (responseCondition.m_ConditionType == ConditionType::BOOOL)
			{
				if (responseCondition.m_requiredConditionBoolState != DialogEngine::GetGameFlagState(responseCondition.m_conditionName))
					goto loop_exit;
			}
			// Quest conditions
			else if (responseCondition.m_ConditionType == ConditionType::QUEST_REQUIREMENTS_MET)
			{
				if (responseCondition.m_requiredConditionBoolState != DialogEngine::AreQuestRequirementsFullfilled(responseCondition.m_conditionName))
					goto loop_exit;
			}
			// Could be an item check
			else if (responseCondition.m_ConditionType == ConditionType::HAVE_ITEM)
			{
				if (!DialogEngine::HasItem(responseCondition.m_conditionName))
					goto loop_exit;
			}
			else if (responseCondition.m_ConditionType == ConditionType::NO_ITEM)
			{
				if (DialogEngine::HasItem(responseCondition.m_conditionName))
					goto loop_exit;
			}
			// Must be a value or item comparison
			else
			{
				std::string name = responseCondition.m_conditionName;
				ConditionType type = responseCondition.m_ConditionType;
				float value = responseCondition.m_conditionComparisonValue;

				if (!DialogEngine::EvaluateConditionalComparison(name, type, value)) {
					//std::cout << "FAILED CHECK: " << response.m_text << "\n";
					goto loop_exit;
				}
			}
		}
		// It's good then
		s_avaliableResponses.push_back(response);

	loop_exit:;
	}
}

bool DialogEngine::IsDialogOver()
{
	if (s_currentDialog.m_ID == -1)
		return true;
	else
		return false;
}

std::string DialogEngine::GetResponseTextByIndex(int index)
{
	if (index < 0 || index >= s_avaliableResponses.size())
		return "out_of_range";

	return s_avaliableResponses[index].m_text;
}


void DialogEngine::SelectResponse(int index)
{
	s_lastSelectedResponseIndex = index;

	// Did something fuck up and you need to bail early?
	if (index < 0 || index >= s_avaliableResponses.size())
		return;

	// Otherwise begin
	Response selectedRespone = s_avaliableResponses[index];

	// Get any fusion actions
	for (std::string fusionAction : selectedRespone.m_fusionActions) {
		AddFusionAction(fusionAction);
		//std::cout << "\nFUSION ACTION: " << fusionAction << "\n";
	}

	// Do any quest shit you need to
	for (std::string& quest : selectedRespone.m_ActivateQuests)
		ActivateQuestByName(quest);
	for (std::string& quest : selectedRespone.m_CompleteQuests)
		CompleteQuestByName(quest);
	for (std::string& quest : selectedRespone.m_FailQuests)
		FailQuestByName(quest);

	// Perform any other actions
	for (ResponseAction& responseAction : selectedRespone.m_responseActions)
	{
		// First you gotta find the GameValue based on the name.
		for (GameFloat& gameFloat : DialogEngine::s_gameFloats)
		{
			if (responseAction.m_name == gameFloat.m_name)
			{
				if (responseAction.m_actionType == ActionType::SUBRACT_FLOAT) {
					gameFloat.Subract(responseAction.m_modifierValue);
				}
				else if (responseAction.m_actionType == ActionType::ADD_FLOAT) {
					gameFloat.Add(responseAction.m_modifierValue);
				}
				else if (responseAction.m_actionType == ActionType::SET_FLOAT) {
					gameFloat.SetTo(responseAction.m_modifierValue);
				}
			}
		}
		// But what if it's a BOOL
		if (responseAction.m_actionType == ActionType::SET_BOOL_TRUE) {
			DialogEngine::SetGameFlag(responseAction.m_name, true);
		}
		else if (responseAction.m_actionType == ActionType::SET_BOOL_FALSE) {
			DialogEngine::SetGameFlag(responseAction.m_name, false);
		}

		// Ok but what if it's a GameItem kinda thing
		if (responseAction.m_actionType == ActionType::GIVE_ITEM) {
			DialogEngine::GiveItem(responseAction.m_name, responseAction.m_modifierValue);
		}
		if (responseAction.m_actionType == ActionType::TAKE_ITEM) {
			DialogEngine::TakeItem(responseAction.m_name, responseAction.m_modifierValue);
		}
	}

	// Switch to the new dialog
	SetCurrentDialogByID(selectedRespone.m_gotoID);
}

bool DialogEngine::IsResponseSpecialColored(int index)
{
	if (index < 0 || index >= s_avaliableResponses.size())
		return false;
	else
		return s_avaliableResponses[index].m_isSpecialColor;;
}

void DialogEngine::AddFusionAction(std::string name)
{
	s_pendingFusionActions.push_back(name);
}

bool DialogEngine::TriggerFusionAction(std::string name)
{
	for (int i = 0; i < s_pendingFusionActions.size(); i++)
	{
		if (s_pendingFusionActions[i] == name)
		{
			s_pendingFusionActions.erase(s_pendingFusionActions.begin() + i);
			return true;
		}
	}
	return false;
}

std::string DialogEngine::GetFusionActionNameByIndex(int index)
{
	if (index < 0 || index >= s_pendingFusionActions.size())
		return "out_of_range";

	return s_pendingFusionActions[index];
}

bool DialogEngine::CompareDialogImageNameToString(std::string query)
{
	return (GetCurrentDialogImageName() == query);
}

void DialogEngine::ClearAllData()
{
	s_gameItems.clear();
	s_gameFlags.clear();
	s_gameFloats.clear();
	s_dialogEntries.clear();
	s_avaliableResponses.clear();
	s_pendingFusionActions.clear();
	s_Quests.clear();
	s_wasAnyQuestJustCompleted = false;
	s_wasAnyQuestJustFailed = false;
	s_currentDialog.m_ID = -1;
}

void DialogEngine::CheckQuestRequirementsAndPayOuts()
{
	for (Quest& quest : s_Quests)
	{
		// If the quest is active, check if the requirments are met
		if (quest.m_questState == QuestState::ACTIVE)
			quest.CheckRequirements(s_gameFlags, s_gameItems);

		// Pay out if you haven't
		if (quest.m_questState == QuestState::COMPLETE && !quest.m_paidOut)
		{
			for (GameItem& item : quest.m_rewardItems)
				GiveItem(item.m_name, item.m_quantity);
			for (GameFloat& value : quest.m_rewardFloats)
				AddToGameFloat(value.m_name, value.m_value);
			quest.m_paidOut = true;
			s_wasAnyQuestJustCompleted = true;
		}
	}
}

void DialogEngine::ActivateQuestByName(std::string query)
{
	for (Quest& quest : s_Quests)
		if (quest.m_name == query && quest.m_questState == QuestState::INACTIVE) {
			quest.m_questState = QuestState::ACTIVE;
			return;
		}
}

void DialogEngine::CompleteQuestByName(std::string query)
{
	for (Quest& quest : s_Quests)
		if (quest.m_name == query && quest.m_questState != QuestState::COMPLETE) {
			quest.m_questState = QuestState::COMPLETE;
			s_wasAnyQuestJustCompleted = true;
			return;
		}
}

void DialogEngine::FailQuestByName(std::string query)
{
	for (Quest& quest : s_Quests)
		if (quest.m_name == query && quest.m_questState != QuestState::FAILED) {
			quest.m_questState = QuestState::FAILED;
			s_wasAnyQuestJustFailed = true;
			return;
		}
}

int DialogEngine::GetQuestCount()
{
	return s_Quests.size();
}

std::string DialogEngine::GetQuestNameByIndex(int index)
{
	if (index < 0 || index >= s_Quests.size())
		return "out_of_range";

	return s_Quests[index].m_name;
}

QuestState DialogEngine::GetQuestStateByIndex(int index)
{
	if (index < 0 || index >= s_Quests.size())
		return QuestState::UNDEFINED;

	return s_Quests[index].m_questState;
}

bool DialogEngine::WasIfAnyQuestWasJustCompleted()
{
	if (s_wasAnyQuestJustCompleted) {
		s_wasAnyQuestJustCompleted = false;
		return true;
	}
	else
		return false;
}

bool DialogEngine::IsQuestActive(std::string query)
{
	for (Quest& quest : s_Quests)
		if (quest.m_name == query)
			return (quest.m_questState == QuestState::ACTIVE);
	return false;
}

bool DialogEngine::IsQuestInactive(std::string query)
{
	for (Quest& quest : s_Quests)
		if (quest.m_name == query)
			return (quest.m_questState == QuestState::INACTIVE);
	return false;
}

std::string DialogEngine::GetQuestStateAsString(std::string query)
{
	for (Quest& quest : s_Quests)
		if (quest.m_name == query)
		{
			if (quest.m_questState == QuestState::ACTIVE) return "ACTIVE";
			if (quest.m_questState == QuestState::INACTIVE) return "INACTIVE";
			if (quest.m_questState == QuestState::COMPLETE) return "COMPLETE";
			if (quest.m_questState == QuestState::FAILED) return "FAILED";
			if (quest.m_questState == QuestState::UNDEFINED) return "UNDEFINED";
		}
	return "QUEST NOT FOUND";
}

std::string DialogEngine::GetQuestDescription(std::string query)
{
	for (Quest& quest : s_Quests)
		if (quest.m_name == query)
			return quest.m_description;
	return "";
}

bool DialogEngine::AreQuestRequirementsFullfilled(std::string query)
{
	for (Quest& quest : s_Quests)
		if (quest.m_name == query)
			return (quest.m_requirementsFullfiled);
	return false;

}bool DialogEngine::IsQuestCompleted(std::string query)
{
	for (Quest& quest : s_Quests)
		if (quest.m_name == query)
			return (quest.m_questState == QuestState::COMPLETE);
	return false;
}

bool DialogEngine::IsQuestFailed(std::string query)
{
	for (Quest& quest : s_Quests)
		if (quest.m_name == query)
			return (quest.m_questState == QuestState::FAILED);
	return false;
}

bool DialogEngine::WasIfAnyQuestWasJustFailed()
{
	if (s_wasAnyQuestJustFailed) {
		s_wasAnyQuestJustFailed = false;
		return true;
	}
	else
		return false;
}



///////////////////
//               //
//  F L O A T S  //
//               //
///////////////////


void DialogEngine::SetGameFloat(std::string name, float value)
{
	// check if float exists
	for (GameFloat& gameValue : s_gameFloats) {

		if (gameValue.m_name == name) {
			gameValue.SetTo(value);
			return;
		}
	}
	// Otherwise create it
	s_gameFloats.push_back(GameFloat(name, value));
}

bool DialogEngine::DoesFlagExist(std::string name)
{
	for (GameFlag& flag : s_gameFlags)
		if (flag.m_name == name)
			return true;
	return false;
}

void DialogEngine::SetGameFloatMin(std::string name, float value)
{
	for (GameFloat& gameValue : s_gameFloats)
		if (gameValue.m_name == name) {
			gameValue.SetMin(value);
			return;
		}
}

void DialogEngine::SetGameFloatMax(std::string name, float value)
{
	for (GameFloat& gameValue : s_gameFloats)
		if (gameValue.m_name == name) {
			gameValue.SetMax(value);
			return;
		}
}

void DialogEngine::AddToGameFloat(std::string name, float value)
{
	for (GameFloat& gameValue : s_gameFloats)
		if (gameValue.m_name == name) {
			gameValue.m_value += value;
			return;
		}
}

void DialogEngine::SubractFromGameFloat(std::string name, float value)
{
	for (GameFloat& gameValue : s_gameFloats)
		if (gameValue.m_name == name) {
			gameValue.m_value -= value;
			return;
		}
}


float DialogEngine::GetGameFloat(std::string flagName)
{
	// Look for the fucking flag
	for (GameFloat& gamevalue : s_gameFloats) {
		if (gamevalue.m_name == flagName) {
			return gamevalue.m_value;
		}
	}
	// returns negative nine nine nine nine nine if flag not found
	return -99999;
}


bool DialogEngine::GameFloatExists(std::string name)
{
	for (GameFloat& value : s_gameFloats)
		if (value.m_name == name)
			return true;
	return false;
}

std::string DialogEngine::GetGameFloatNameByIndex(int index)
{
	if (index < 0 || index >= s_gameFloats.size())
		return "out_of_range";

	return s_gameFloats[index].m_name;
}

/////////////////
//             //
//  F L A G S  //
//             //
/////////////////


void DialogEngine::SetGameFlag(std::string flagName, bool state)
{
	// check if flag exists
	for (GameFlag& gameFlag : s_gameFlags) {

		//  if (TCharCompare(gameFlag.m_name.c_str(), flagName)) {
		if (gameFlag.m_name == flagName) {
			gameFlag.m_state = state;
			return;
		}
	}
	// Otherwise create it
	s_gameFlags.push_back(GameFlag(flagName, state));
}

bool DialogEngine::GetGameFlagState(std::string flagName)
{
	// Look for the fucking flag
	for (GameFlag& gameFlag : s_gameFlags) {
		if (gameFlag.m_name == flagName) {
			return gameFlag.m_state;
		}
	}
	// returns false if flag not found
	return false;
}

void DialogEngine::ToggleGameFlag(std::string flagName)
{
	// Look for the fucking flag
	for (GameFlag& gameFlag : s_gameFlags) {
		if (gameFlag.m_name == flagName) {
			gameFlag.Toggle();
		}
	}
}

std::string DialogEngine::GetGameFlagNameByIndex(int index)
{
	if (index < 0 || index >= s_gameFlags.size())
		return "out_of_range";

	return s_gameFlags[index].m_name;
}



/////////////////
//             //
//  I T E M S  //
//             //
/////////////////


void DialogEngine::TakeItem(std::string name, int quantity)
{
	for (int i = 0; i < s_gameItems.size(); i++) {
		// Find the item
		if (s_gameItems[i].m_name == name) {
			// Subract from the item's quantity
			s_gameItems[i].m_quantity -= quantity;
			// If quanitity is 0 then remove it
			if (s_gameItems[i].m_quantity <= 0)
				s_gameItems.erase(s_gameItems.begin() + i);

			return;
		}
	}
}

void DialogEngine::GiveItem(std::string name, int quantity)
{
	for (int i = 0; i < s_gameItems.size(); i++) {
		// Find the item
		if (s_gameItems[i].m_name == name) {
			// Add to the item's quantity
			s_gameItems[i].m_quantity += quantity;
			return;
		}
	}
	// Oh you don't have it, well then create the fucking item.
	s_gameItems.push_back(GameItem(name, quantity));
}

int DialogEngine::GetItemQuantity(std::string name)
{
	for (GameItem& gameItem : s_gameItems)
		if (gameItem.m_name == name) {
			return gameItem.m_quantity;
		}
	return -1;
}

bool DialogEngine::HasItem(std::string name)
{
	for (GameItem& item : DialogEngine::s_gameItems)
		if (item.m_name == name)
			return true;
	return false;
}

std::string DialogEngine::GetGameItemNameByIndex(int index)
{
	if (index < 0 || index >= s_gameItems.size())
		return "out_of_range";

	return s_gameItems[index].m_name;
}
