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
	DialogEngine::GiveItem(DarkEdif::TStringToUTF8(name), quanitity);
}

void Extension::TakeItem(TCHAR* name, int quanitity)
{
	DialogEngine::TakeItem(DarkEdif::TStringToUTF8(name), quanitity);
}

void Extension::TriggerFusionActionByName(TCHAR* name)
{
	DialogEngine::AddFusionAction(DarkEdif::TStringToUTF8(name));
}

void Extension::ClearAllData()
{
	DialogEngine::ClearAllData();
}

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
}

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
}

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
