
#include "Common.h"


const TCHAR* Extension::GetCurrentDialogText()
{
	std::string str = DialogEngine::GetCurrentDialogText();
	std::tstring tstr = DarkEdif::UTF8ToTString(str);
	return Runtime.CopyString(tstr.c_str());
}

const TCHAR* Extension::GetcurrentDialogImageName()
{
	std::string str = DialogEngine::GetCurrentDialogImageName();
	std::tstring tstr = DarkEdif::UTF8ToTString(str);
	return Runtime.CopyString(tstr.c_str()); 
}

const TCHAR* Extension::GetResponseTextByIndex(int index)
{
	std::string str = DialogEngine::GetResponseTextByIndex(index);
	std::tstring tstr = DarkEdif::UTF8ToTString(str);
	return Runtime.CopyString(tstr.c_str());
}

int Extension::GetAvaliableReponseCount()
{
	return DialogEngine::s_avaliableResponses.size();
}

float Extension::GetGameValueByName(TCHAR* name)
{
	return DialogEngine::GetGameFloat(DarkEdif::TStringToUTF8(name));
}

const TCHAR* Extension::GetGameValueNameByIndex(int index)
{
	std::string str = DialogEngine::GetGameFloatNameByIndex(index);
	std::tstring tstr = DarkEdif::UTF8ToTString(str);
	return Runtime.CopyString(tstr.c_str());
}

int Extension::GetTotalGameValueCount()
{
	return DialogEngine::s_gameFloats.size();
}

const TCHAR* Extension::GetGameFlagStateAsString(TCHAR* name)
{
	bool state = DialogEngine::GetGameFlagState(DarkEdif::TStringToUTF8(name));

	if (state)
		return Runtime.CopyString(_T("TRUE"));
	else
		return Runtime.CopyString(_T("FALSE"));
}

const TCHAR* Extension::GetGameFlagNameByIndex(int index)
{
	std::string str = DialogEngine::GetGameFlagNameByIndex(index);
	std::tstring tstr = DarkEdif::UTF8ToTString(str);
	return Runtime.CopyString(tstr.c_str());
}

int Extension::GetTotalGameFlagCount()
{
	return DialogEngine::s_gameFlags.size();
}

int Extension::GetGameItemQuantityByName(TCHAR* name)
{
	std::string str = DarkEdif::TStringToUTF8(name);
	return DialogEngine::GetItemQuantity(str);
}

const TCHAR* Extension::GetGameItemNameByIndex(int index)
{
	std::string str = DialogEngine::GetGameItemNameByIndex(index);
	std::tstring tstr = DarkEdif::UTF8ToTString(str);
	return Runtime.CopyString(tstr.c_str());
}

int Extension::GetTotalGameItems()
{
	return DialogEngine::s_gameItems.size();
}

const TCHAR* Extension::GetFusionActionNameIndex(int index)
{
	std::string str = DialogEngine::GetFusionActionNameByIndex(index);
	std::tstring tstr = DarkEdif::UTF8ToTString(str);
	return Runtime.CopyString(tstr.c_str());
}

int Extension::GetTotalPendingFusionActions()
{
	return DialogEngine::s_pendingFusionActions.size();
}

int Extension::GetLastSelectedResponseIndex()
{
	return DialogEngine::s_lastSelectedResponseIndex;
}

const TCHAR* Extension::GetQuestStateAsString(TCHAR* name)
{
	std::string state = DialogEngine::GetQuestStateAsString(DarkEdif::TStringToUTF8(name));
	return Runtime.CopyString(DarkEdif::UTF8ToTString(state).c_str());
}

const TCHAR* Extension::GetQuestNameByIndex(int index)
{
	std::string str = DialogEngine::GetQuestNameByIndex(index);
	std::tstring tstr = DarkEdif::UTF8ToTString(str);
	return Runtime.CopyString(tstr.c_str());
}

int Extension::GetTotalQuestCount()
{
	return DialogEngine::s_Quests.size();
}

const TCHAR* Extension::GetQuestDescriptionByName(TCHAR* name)
{
	std::string state = DialogEngine::GetQuestDescription(DarkEdif::TStringToUTF8(name));
	return Runtime.CopyString(DarkEdif::UTF8ToTString(state).c_str());
}

int Extension::GetCurrentDialogID()
{
	return DialogEngine::s_currentDialog.m_ID;
}

int Extension::GetNPCCount()
{
	return DialogEngine::s_NPCs.size();
}

const TCHAR* Extension::GetNPCInWorldDialogStringByIndex(int index)
{
	std::string str = DialogEngine::GetNPCInWorldDialogStringByIndex(index);
	std::tstring tstr = DarkEdif::UTF8ToTString(str);
	return Runtime.CopyString(tstr.c_str());
}

int Extension::GetNPCFixedValueByIndex(int index)
{
	return DialogEngine::GetNPCFixedValueByIndex(index);
}
