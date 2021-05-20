#include "Common.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


void Extension::LoadDialogFile(TCHAR* name)
{
	DialogEngine::LoadDialogFile(DarkEdif::TStringToUTF8(name));
}

void Extension::LoadQuestFile(TCHAR* name)
{
	DialogEngine::LoadQuestFile(DarkEdif::TStringToUTF8(name));
}

void Extension::LoadPlayerDataFile(TCHAR* name)
{
	DialogEngine::LoadPlayerFile(DarkEdif::TStringToUTF8(name));
}

void Extension::SavePlayerDataFile(TCHAR* name)
{
	DialogEngine::SavePlayerFile(DarkEdif::TStringToUTF8(name));
}

void Extension::ShowDialogByID(int ID)
{
	DialogEngine::SetCurrentDialogByID(ID);
}

void Extension::SelectResponseByIndex(int index)
{
	DialogEngine::SelectResponse(index);
}

void Extension::SetGameValue(TCHAR* name, float value)
{
	DialogEngine::SetGameFloat(DarkEdif::TStringToUTF8(name), value);
}

void Extension::SetGameValueMin(TCHAR* name, float value)
{
	DialogEngine::SetGameFloatMin(DarkEdif::TStringToUTF8(name), value);
}

void Extension::SetGameValueMax(TCHAR* name, float value)
{
	DialogEngine::SetGameFloatMax(DarkEdif::TStringToUTF8(name), value);
}

void Extension::AddToGameValue(TCHAR* name, float value)
{
	DialogEngine::AddToGameFloat(DarkEdif::TStringToUTF8(name), value);
}

void Extension::SubtractFromValue(TCHAR* name, float value)
{
	DialogEngine::SubractFromGameFloat(DarkEdif::TStringToUTF8(name), value);
}

void Extension::SetGameFlagToTrue(TCHAR* name)
{
	DialogEngine::SetGameFlag(DarkEdif::TStringToUTF8(name), true);
}

void Extension::SetGameFlagToFalse(TCHAR* name)
{
	DialogEngine::SetGameFlag(DarkEdif::TStringToUTF8(name), false);
}

void Extension::ToggleGameFlag(TCHAR* name)
{
	DialogEngine::ToggleGameFlag(DarkEdif::TStringToUTF8(name));
}

void Extension::GiveItem(TCHAR* name, int quanitity)
{
	Inventory::GiveItem(DarkEdif::TStringToUTF8(name), quanitity);
}

void Extension::TakeItem(TCHAR* name, int quanitity)
{
	Inventory::TakeItem(DarkEdif::TStringToUTF8(name), quanitity);
}

void Extension::TriggerFusionActionByName(TCHAR* name)
{
	DialogEngine::AddFusionAction(DarkEdif::TStringToUTF8(name));
}

void Extension::ClearAllData()
{
	DialogEngine::ClearAllData();
}


/*
void Extension::CheckNPCForQuest(TCHAR* npcName)
{
	DialogEngine::CheckNPCForQuest(DarkEdif::TStringToUTF8(npcName));
}*/
void Extension::CheckNPCForInworldQuestDialog(TCHAR* npcName)
{
	DialogEngine::CheckNPCForInworldQuestDialog(DarkEdif::TStringToUTF8(npcName));
}
void Extension::CheckNPCForStandardDialogAndQuests(TCHAR* npcName)
{
	DialogEngine::CheckNPCForStandardDialogAndQuests(DarkEdif::TStringToUTF8(npcName));
}
void Extension::CheckNPCForInWorldDialog(TCHAR* npcName)
{
	DialogEngine::CheckNPCForInWorldDialog(DarkEdif::TStringToUTF8(npcName));
}

/*
void Extension::RegisterNPCFixedValue(TCHAR* name, int value)
{
	DialogEngine::RegisterNPC(DarkEdif::TStringToUTF8(name), value);
}

void Extension::RegisterItemFixedValue(TCHAR* name, int value)
{
	DialogEngine::RegisterItem(DarkEdif::TStringToUTF8(name), value);
}

void Extension::PickedUpFusionItem(int fixedValue)
{
	DialogEngine::PickedUpFusionItem(fixedValue);
}*/
/*
void Extension::CheckNPCQuestDialog(int fixedValue)
{
	DialogEngine::CheckNPCQuestDialog(fixedValue);
}

void Extension::CheckNPCInWorldDialog(int fixedValue)
{
	DialogEngine::CheckNPCInWorldDialog(fixedValue);
}

void Extension::CheckNPCInWorldQuestDialogText(int fixedValue)
{
	DialogEngine::CheckNPCInWorldQuestDialogText(fixedValue);
}

void Extension::CheckNPCStandardDialogText(int fixedValue)
{
	DialogEngine::CheckNPCStandardDialogText(fixedValue);
}*/

void Extension::LoadInWorldDialogFile(TCHAR* name)
{
	DialogEngine::LoadInWorldDialogFile(DarkEdif::TStringToUTF8(name));
}

/*void Extension::AddFixedValueTimer(int fixedValue, int timerDuration)
{
	FixedValueTimer timer;
	timer.fixedValue = fixedValue;
	timer.timer = timerDuration;
	m_fixedValueTimers.push_back(timer);
}*/

void Extension::UpdateEngine()
{
	DialogEngine::UpdateEngine();
}

void Extension::SetTextTimerDuration(int duration)
{
	DialogEngine::SetTextDuration(duration);
}

void Extension::ActivateQuestByName(TCHAR* name)
{
	DialogEngine::ActivateQuestByName(DarkEdif::TStringToUTF8(name));
}

void Extension::CompleteQuestByName(TCHAR* name)
{
	DialogEngine::CompleteQuestByName(DarkEdif::TStringToUTF8(name));
}

void Extension::FailQuestByName(TCHAR* name)
{
	DialogEngine::FailQuestByName(DarkEdif::TStringToUTF8(name));

}

void Extension::CheckForQuestCompletion()
{
	DialogEngine::CheckQuestRequirementsAndPayOuts();
}










void Extension::LoadItemDatabase(TCHAR* filename)
{
	// replace with loading from a file soon u dummy
	Inventory::LoadInventoryDatabase(DarkEdif::TStringToUTF8(filename));
	Inventory::InitDefaults();
}

void Extension::DespositItemIntoContainer(TCHAR* containerName, TCHAR* itemName, int itemQuanitity, int gridLocatiom)
{
	Inventory::DepositItemsIntoContainer(DarkEdif::TStringToUTF8(containerName), DarkEdif::TStringToUTF8(itemName), itemQuanitity, gridLocatiom);
}

void Extension::WithdrawItemFromContainer(TCHAR* containerName, TCHAR* itemName, int itemQuanitity, int gridLocatiom)
{
	Inventory::WithdrawItemsFromContainer(DarkEdif::TStringToUTF8(containerName), DarkEdif::TStringToUTF8(itemName), itemQuanitity, gridLocatiom);
}

void Extension::SetInventoryBagGeneralSize(int size)
{
	Inventory::m_max_general_slots = size;
}
void Extension::SetInventoryBagWearableSize(int size)
{
	Inventory::m_max_wearable_slots = size;
}
void Extension::SetInventoryBagEquipableSize(int size)
{
	Inventory::m_max_equipable_slots = size;
}
void Extension::SetInventoryBagMaterialSize(int size)
{
	Inventory::m_max_material_slots = size;
}
void Extension::SetInventoryBagConsumableSize(int size)
{
	Inventory::m_max_consumable_slots = size;
}
void Extension::SetInventoryBagQuestSize(int size)
{
	Inventory::m_max_quest_slots = size;
}
void Extension::SetInventoryBagSkillsSize(int size)
{
	Inventory::m_max_skill_slots = size;
}
void Extension::MoveInventoryItem(TCHAR* itemName, int gridLocatiom)
{
	Inventory::MoveInventoryItem(DarkEdif::TStringToUTF8(itemName), gridLocatiom);
}
void Extension::MoveContaineryItem(TCHAR* itemName, int gridLocatiom)
{
	Inventory::MoveContainerItem(DarkEdif::TStringToUTF8(itemName), gridLocatiom);
}
void Extension::SetCurrentContainerByName(TCHAR* containerName)
{
	Inventory::SetCurrentContainerByName(DarkEdif::TStringToUTF8(containerName));
}
void Extension::SetCurrentCompanionByName(TCHAR* companionName)
{
	Inventory::SetCurrentCompanionByName(DarkEdif::TStringToUTF8(companionName));
}
void Extension::ShowCompanionInventory()
{
	Inventory::ShowCompanion();
}
void Extension::ShowContainerInventory()
{
	Inventory::ShowContainer();
}
void Extension::setCurrentBagToGeneral()
{
	Inventory::SetCurrentInventoryBagToGeneral();
}
void Extension::setCurrentBagToWearable()
{
	Inventory::SetCurrentInventoryBagToWearable();
}
void Extension::setCurrentBagToEquipable()
{
	Inventory::SetCurrentInventoryBagToEquipable();
}
void Extension::setCurrentBagToMaterial()
{
	Inventory::SetCurrentInventoryBagToMaterial();
}
void Extension::setCurrentBagToConsumable()
{
	Inventory::SetCurrentInventoryBagToConsumeable();
}
void Extension::setCurrentBagToQuest()
{
	Inventory::SetCurrentInventoryBagToQuest();
}

void Extension::setCurrentBagToSkills()
{
	Inventory::SetCurrentInventoryBagToSkills();
}

void Extension::EquipToHead(TCHAR* itemName)
{
	std::string item = DarkEdif::TStringToUTF8(itemName);
	Inventory::EquipToHead(item);
}

void Extension::EquipToUpperBody(TCHAR* itemName)
{
	std::string item = DarkEdif::TStringToUTF8(itemName);
	Inventory::EquipToUpperBody(item);
}

void Extension::EquipToLowerBody(TCHAR* itemName)
{
	std::string item = DarkEdif::TStringToUTF8(itemName);
	Inventory::EquipToLowerBody(item);
}

void Extension::EquipToHands(TCHAR* itemName)
{
	std::string item = DarkEdif::TStringToUTF8(itemName);
	Inventory::EquipToHands(item);
}

void Extension::EquipToFeet(TCHAR* itemName)
{
	std::string item = DarkEdif::TStringToUTF8(itemName);
	Inventory::EquipToFeet(item);
}

void Extension::EquipToBelt(TCHAR* itemName)
{
	std::string item = DarkEdif::TStringToUTF8(itemName);
	Inventory::EquipToBelt(item);
//	Inventory::s_equippedItems.m_belt = "Utility Belt";
}

void Extension::EquipToEquipSlot(TCHAR* itemName)
{
	std::string item = DarkEdif::TStringToUTF8(itemName);
	Inventory::EquipToEquipSlot(item);
}

void Extension::EquipItemByName(TCHAR* itemName)
{
	std::string item = DarkEdif::TStringToUTF8(itemName);
	Inventory::EquipItemByName(item);
}

void Extension::DequipItemByName(TCHAR* itemName)
{
	std::string item = DarkEdif::TStringToUTF8(itemName);
	Inventory::DequipItemByName(item);
}

void Extension::UseItemByName(TCHAR* itemName)
{
	std::string item = DarkEdif::TStringToUTF8(itemName);
	Inventory::UseItem(item);
}

void Extension::SetCurrentContainerToNone()
{
	Inventory::SetCurrentContainerToNone();
}

void Extension::SetCurrentCompanionToNone()
{
	Inventory::SetCurrentCompanionToNone();
}
