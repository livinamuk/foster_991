#include "Common.h"

///
/// EXTENSION CONSTRUCTOR/DESTRUCTOR
///

#ifdef _WIN32
Extension::Extension(RUNDATA * _rdPtr, EDITDATA * edPtr, CreateObjectInfo * cobPtr) :
	rdPtr(_rdPtr), rhPtr(_rdPtr->rHo.AdRunHeader), Runtime(&_rdPtr->rHo)
#else
Extension::Extension(RuntimeFunctions &runFuncs, EDITDATA * edPtr, jobject javaExtPtr) :
	runFuncs(runFuncs), javaExtPtr(javaExtPtr), Runtime(runFuncs, this->javaExtPtr)
#endif
{
	/*
        Link all your action/condition/expression functions to their IDs to match the
        IDs in the JSON here
   */


	LinkAction(0, LoadDialogFile);
	LinkAction(1, LoadQuestFile);
	LinkAction(2, LoadPlayerDataFile);
	LinkAction(3, LoadInWorldDialogFile);
	LinkAction(4, SavePlayerDataFile);

	LinkAction(5, ShowDialogByID);
	LinkAction(6, SelectResponseByIndex);
	LinkAction(7, SetGameValue);
	LinkAction(8, SetGameValueMin);
	LinkAction(9, SetGameValueMax);
	LinkAction(10, AddToGameValue);
	LinkAction(11, SubtractFromValue);
	LinkAction(12, SetGameFlagToTrue);
	LinkAction(13, SetGameFlagToFalse);
	LinkAction(14, ToggleGameFlag);

	LinkAction(15, RegisterItemFixedValue);
	LinkAction(16, GiveItem);
	LinkAction(17, TakeItem);
	LinkAction(18, PickedUpFusionItem);

	LinkAction(19, TriggerFusionActionByName);

	LinkAction(20, ActivateQuestByName);
	LinkAction(21, CompleteQuestByName);
	LinkAction(22, FailQuestByName);
	LinkAction(23, CheckForQuestCompletion);

	LinkAction(24, RegisterNPCFixedValue);
	LinkAction(25, CheckNPCQuestDialog);
	LinkAction(26, CheckNPCInWorldDialog);
	LinkAction(27, SetTextTimerDuration);
	LinkAction(28, CheckNPCInWorldQuestDialogText);
	LinkAction(29, CheckNPCStandardDialogText);

	LinkAction(30, ClearAllData);
	LinkAction(31, UpdateEngine);

	LinkCondition(0, IsThereDialogToShow);
	LinkCondition(1, IsDialogOver);
	LinkCondition(2, IsReponseASpecialColor);
	LinkCondition(3, DoesValueExist);
	LinkCondition(4, DoesPlayerHaveItem);
	LinkCondition(5, IsFusionActionTriggered);
	LinkCondition(6, IsReponseEmpty);
	LinkCondition(7, CompareDialogImageName);
	LinkCondition(8, IsFlagTrue);
	LinkCondition(9, IsFlagFalse);
	//
	LinkCondition(10, IsQuestActive);
	LinkCondition(11, IsQuestInactive);
	LinkCondition(12, AreQuestRequirementsFullfilled);
	LinkCondition(13, IsQuestComplete);
	LinkCondition(14, IsQuestFailed);
	LinkCondition(15, WasAnyQuestJustCompleted);
	LinkCondition(16, WasAnyQuestJustFailed);

	LinkExpression(0, GetCurrentDialogText);
	LinkExpression(1, GetcurrentDialogImageName);
	LinkExpression(2, GetResponseTextByIndex);
	LinkExpression(3, GetAvaliableReponseCount);
	LinkExpression(4, GetLastSelectedResponseIndex);
	LinkExpression(5, GetCurrentDialogID);
	LinkExpression(6, GetGameValueByName);
	LinkExpression(7, GetGameValueNameByIndex);
	LinkExpression(8, GetTotalGameValueCount);
	LinkExpression(9, GetGameFlagStateAsString);
	LinkExpression(10, GetGameFlagNameByIndex);
	LinkExpression(11, GetTotalGameFlagCount);
	LinkExpression(12, GetGameItemQuantityByName);
	LinkExpression(13, GetGameItemNameByIndex);
	LinkExpression(14, GetTotalGameItems);
	LinkExpression(15, GetFusionActionNameIndex);
	LinkExpression(16, GetTotalPendingFusionActions);
	//
	LinkExpression(17, GetQuestNameByIndex);
	LinkExpression(18, GetQuestStateAsString);
	LinkExpression(19, GetQuestDescriptionByName);
	LinkExpression(20, GetTotalQuestCount);

	LinkExpression(21, GetNPCCount);
	LinkExpression(22, GetNPCInWorldDialogStringByIndex);
	LinkExpression(23, GetNPCFixedValueByIndex);

	/*
   
	{
		LinkAction(0, FirstAction);
		LinkAction(1, SecondAction);
		LinkAction(2, AllParamsAction);
	}
	{
		LinkCondition(0, AlwaysTrue);
		LinkCondition(1, AlwaysFalse);
		LinkCondition(2, ConditionWithParams);
		LinkCondition(3, CondWithAllParams);
	}
	{
		LinkExpression(0, TextExpression);
		LinkExpression(1, IntegerExpression);
		LinkExpression(2, FloatExpression);
		LinkExpression(3, AllParamsRetInt);
	}
    */
	/*
        This is where you'd do anything you'd do in CreateRunObject in the original SDK

        It's the only place you'll get access to edPtr at runtime, so you should transfer
        anything from edPtr to the extension class here.
    */

	Runtime.Rehandle(); // we may need this later, so tagging it here so it comes up as missing
}

Extension::~Extension()
{
}

REFLAG Extension::Handle()
{
	bool runNextLoop = rand() % 2 == 0;
	// Will not be called next loop if runNextLoop is false
	return runNextLoop ? REFLAG::NONE : REFLAG::ONE_SHOT;
}

short Extension::Display()
{
    /*
       If you return REFLAG::DISPLAY in Handle() this routine will run.
    */

    // Ok
    return 0;
}

short Extension::Pause()
{

    // Ok
    return 0;
}

short Extension::Continue()
{

    // Ok
    return 0;
}

bool Extension::Save(HANDLE File)
{
	bool OK = false;

    #ifndef VITALIZE

	    // Save the object's data here

	    OK = true;

    #endif

	return OK;
}

bool Extension::Load(HANDLE File)
{
	bool OK = false;

    #ifndef VITALIZE

	    // Load the object's data here

	    OK = true;

    #endif

	return OK;
}


// These are called if there's no function linked to an ID

void Extension::Action(int ID)
{

}

long Extension::Condition(int ID)
{
    return false;
}

long Extension::Expression(int ID)
{
    return 0;
}

