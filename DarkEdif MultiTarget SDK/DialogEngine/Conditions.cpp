
#include "Common.h"


bool Extension::IsThereDialogToShow()
{
	return !DialogEngine::IsDialogOver();
}

bool Extension::IsDialogOver()
{
	return DialogEngine::IsDialogOver();
}

bool Extension::IsReponseASpecialColor(int index)
{
	return DialogEngine::IsResponseSpecialColored(index);
}

bool Extension::DoesValueExist(TCHAR* name)
{
	std::string str = DarkEdif::TStringToUTF8(name);
	return DialogEngine::GameFloatExists(str);
}

bool Extension::DoesPlayerHaveItem(TCHAR* name)
{
	std::string str = DarkEdif::TStringToUTF8(name);
	return Inventory::HasItem(str);
}

bool Extension::IsFusionActionTriggered(TCHAR* name)
{
	std::string str = DarkEdif::TStringToUTF8(name);
	return DialogEngine::TriggerFusionAction(str);
}

bool Extension::IsReponseEmpty(int index)
{
	if (index < 0 || index >= (int)DialogEngine::s_avaliableResponses.size())
		return true;

	if (DialogEngine::s_avaliableResponses[index].m_text.length() > 0)
		return false;
	else
		return true;
}

bool Extension::CompareDialogImageName(TCHAR* query)
{
	std::string str = DarkEdif::TStringToUTF8(query);
	return DialogEngine::CompareDialogImageNameToString(str);
}

bool Extension::IsFlagTrue(TCHAR* query)
{
	std::string str = DarkEdif::TStringToUTF8(query);
	return (DialogEngine::GetGameFlagState(str) == true);
}

bool Extension::IsFlagFalse(TCHAR* query)
{
	std::string str = DarkEdif::TStringToUTF8(query);
	return (DialogEngine::GetGameFlagState(str) == false);
}


bool Extension::IsQuestActive(TCHAR* query) {

	return (DialogEngine::IsQuestActive(DarkEdif::TStringToUTF8(query)));
}

bool Extension::IsQuestInactive(TCHAR* query) {

	return (DialogEngine::IsQuestInactive(DarkEdif::TStringToUTF8(query)));
}

bool Extension::AreQuestRequirementsFullfilled(TCHAR* query)
{
	return (DialogEngine::AreQuestRequirementsFullfilled(DarkEdif::TStringToUTF8(query)));
}

bool Extension::IsQuestComplete(TCHAR* query) 
{
	return (DialogEngine::IsQuestCompleted(DarkEdif::TStringToUTF8(query)));
}

bool Extension::IsQuestFailed(TCHAR* query) 
{
	return (DialogEngine::IsQuestFailed(DarkEdif::TStringToUTF8(query)));
}

bool Extension::WasAnyQuestJustCompleted() 
{
	return DialogEngine::WasIfAnyQuestWasJustCompleted();
}

bool Extension::WasAnyQuestJustFailed()
{
	return DialogEngine::WasIfAnyQuestWasJustFailed();

}

bool Extension::IsCurrentBagGeneral() {
	return Inventory::IsGeneralInventoryBagOpen();
}
bool Extension::IsCurrentBagWearable() {
	return Inventory::IsWearableInventoryBagOpen();
}
bool Extension::IsCurrentBagEquipable() {
	return Inventory::IsEquipableInventoryBagOpen();
}
bool Extension::IsCurrentBagMaterial() {
	return Inventory::IsMaterialInventoryBagOpen();
}
bool Extension::IsCurrentBagConsumable() {
	return Inventory::IsConsumableInventoryBagOpen();
}
bool Extension::IsCurrentBagQuest() {
	return Inventory::IsQuestInventoryBagOpen();
}


