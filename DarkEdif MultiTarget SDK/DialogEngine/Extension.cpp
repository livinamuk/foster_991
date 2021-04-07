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
	LinkAction(3, SavePlayerDataFile);
	LinkAction(4, ShowDialogByID);
	LinkAction(5, SelectResponseByIndex);
	LinkAction(6, SetGameValue);
	LinkAction(7, SetGameValueMin);
	LinkAction(8, SetGameValueMax);
	LinkAction(9, AddToGameValue);
	LinkAction(10, SubtractFromValue);
	LinkAction(11, SetGameFlagToTrue);
	LinkAction(12, SetGameFlagToFalse);
	LinkAction(13, ToggleGameFlag);
	LinkAction(14, GiveItem);
	LinkAction(15, TakeItem);
	LinkAction(16, TriggerFusionActionByName);
	LinkAction(17, ActivateQuestByName);
	LinkAction(18, CompleteQuestByName);
	LinkAction(19, FailQuestByName);
	LinkAction(20, CheckForQuestCompletion);
	LinkAction(21, ClearAllData);


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
	LinkExpression(5, GetGameValueByName);
	LinkExpression(6, GetGameValueNameByIndex);
	LinkExpression(7, GetTotalGameValueCount);
	LinkExpression(8, GetGameFlagStateAsString);
	LinkExpression(9, GetGameFlagNameByIndex);
	LinkExpression(10, GetTotalGameFlagCount);
	LinkExpression(11, GetGameItemQuantityByName);
	LinkExpression(12, GetGameItemNameByIndex);
	LinkExpression(13, GetTotalGameItems);
	LinkExpression(14, GetFusionActionNameIndex);
	LinkExpression(15, GetTotalPendingFusionActions);
	//
	LinkExpression(16, GetQuestNameByIndex);
	LinkExpression(17, GetQuestStateAsString);
	LinkExpression(18, GetQuestDescriptionByName);
	LinkExpression(19, GetTotalQuestCount);
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

