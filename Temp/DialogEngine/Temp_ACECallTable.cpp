// AltPlatformPostBuildTool: This file was generated for a Extension.cpp/h that was modified at 2021-04-07T00:55:38.0677263Z.
// This file is used by the AltPlatformPreBuildTool as an input template for the calltable generator.
// Do not modify or delete this file.
#if DARKEDIF_ACE_CALL_TABLE_INDEX==0
			case 0:
				return ext->IsThereDialogToShow();
			case 1:
				return ext->IsDialogOver();
			case 2:
				return ext->IsReponseASpecialColor(*(int *)&Parameters[0]);
			case 3:
				return ext->DoesValueExist(*(TCHAR* *)&Parameters[0]);
			case 4:
				return ext->DoesPlayerHaveItem(*(TCHAR* *)&Parameters[0]);
			case 5:
				return ext->IsFusionActionTriggered(*(TCHAR* *)&Parameters[0]);
			case 6:
				return ext->IsReponseEmpty(*(int *)&Parameters[0]);
			case 7:
				return ext->CompareDialogImageName(*(TCHAR* *)&Parameters[0]);
			case 8:
				return ext->IsFlagTrue(*(TCHAR* *)&Parameters[0]);
			case 9:
				return ext->IsFlagFalse(*(TCHAR* *)&Parameters[0]);
			case 10:
				return ext->IsQuestActive(*(TCHAR* *)&Parameters[0]);
			case 11:
				return ext->IsQuestInactive(*(TCHAR* *)&Parameters[0]);
			case 12:
				return ext->AreQuestRequirementsFullfilled(*(TCHAR* *)&Parameters[0]);
			case 13:
				return ext->IsQuestComplete(*(TCHAR* *)&Parameters[0]);
			case 14:
				return ext->IsQuestFailed(*(TCHAR* *)&Parameters[0]);
			case 15:
				return ext->WasAnyQuestJustCompleted();
			case 16:
				return ext->WasAnyQuestJustFailed();

#elif DARKEDIF_ACE_CALL_TABLE_INDEX==1
			case 0:
				return ext->LoadDialogFile(*(TCHAR* *)&Parameters[0]), 0;
			case 1:
				return ext->LoadQuestFile(*(TCHAR* *)&Parameters[0]), 0;
			case 2:
				return ext->LoadPlayerDataFile(*(TCHAR* *)&Parameters[0]), 0;
			case 3:
				return ext->SavePlayerDataFile(*(TCHAR* *)&Parameters[0]), 0;
			case 4:
				return ext->ShowDialogByID(*(int *)&Parameters[0]), 0;
			case 5:
				return ext->SelectResponseByIndex(*(int *)&Parameters[0]), 0;
			case 6:
				return ext->SetGameValue(*(TCHAR* *)&Parameters[0], *(float *)&Parameters[1]), 0;
			case 7:
				return ext->SetGameValueMin(*(TCHAR* *)&Parameters[0], *(float *)&Parameters[1]), 0;
			case 8:
				return ext->SetGameValueMax(*(TCHAR* *)&Parameters[0], *(float *)&Parameters[1]), 0;
			case 9:
				return ext->AddToGameValue(*(TCHAR* *)&Parameters[0], *(float *)&Parameters[1]), 0;
			case 10:
				return ext->SubtractFromValue(*(TCHAR* *)&Parameters[0], *(float *)&Parameters[1]), 0;
			case 11:
				return ext->SetGameFlagToTrue(*(TCHAR* *)&Parameters[0]), 0;
			case 12:
				return ext->SetGameFlagToFalse(*(TCHAR* *)&Parameters[0]), 0;
			case 13:
				return ext->ToggleGameFlag(*(TCHAR* *)&Parameters[0]), 0;
			case 14:
				return ext->GiveItem(*(TCHAR* *)&Parameters[0], *(int *)&Parameters[1]), 0;
			case 15:
				return ext->TakeItem(*(TCHAR* *)&Parameters[0], *(int *)&Parameters[1]), 0;
			case 16:
				return ext->TriggerFusionActionByName(*(TCHAR* *)&Parameters[0]), 0;
			case 17:
				return ext->ActivateQuestByName(*(TCHAR* *)&Parameters[0]), 0;
			case 18:
				return ext->CompleteQuestByName(*(TCHAR* *)&Parameters[0]), 0;
			case 19:
				return ext->FailQuestByName(*(TCHAR* *)&Parameters[0]), 0;
			case 20:
				return ext->CheckForQuestCompletion(), 0;
			case 21:
				return ext->ClearAllData(), 0;

#elif DARKEDIF_ACE_CALL_TABLE_INDEX==2
		case 0:
			*((const TCHAR* *)&Result) = ext->GetCurrentDialogText();
			break;
		case 1:
			*((const TCHAR* *)&Result) = ext->GetcurrentDialogImageName();
			break;
		case 2:
			*((const TCHAR* *)&Result) = ext->GetResponseTextByIndex(*(int *)&Parameters[0]);
			break;
		case 3:
			*((int *)&Result) = ext->GetAvaliableReponseCount();
			break;
		case 4:
			*((int *)&Result) = ext->GetLastSelectedResponseIndex();
			break;
		case 5:
			*((float *)&Result) = ext->GetGameValueByName(*(TCHAR* *)&Parameters[0]);
			break;
		case 6:
			*((const TCHAR* *)&Result) = ext->GetGameValueNameByIndex(*(int *)&Parameters[0]);
			break;
		case 7:
			*((int *)&Result) = ext->GetTotalGameValueCount();
			break;
		case 8:
			*((const TCHAR* *)&Result) = ext->GetGameFlagStateAsString(*(TCHAR* *)&Parameters[0]);
			break;
		case 9:
			*((const TCHAR* *)&Result) = ext->GetGameFlagNameByIndex(*(int *)&Parameters[0]);
			break;
		case 10:
			*((int *)&Result) = ext->GetTotalGameFlagCount();
			break;
		case 11:
			*((int *)&Result) = ext->GetGameItemQuantityByName(*(TCHAR* *)&Parameters[0]);
			break;
		case 12:
			*((const TCHAR* *)&Result) = ext->GetGameItemNameByIndex(*(int *)&Parameters[0]);
			break;
		case 13:
			*((int *)&Result) = ext->GetTotalGameItems();
			break;
		case 14:
			*((const TCHAR* *)&Result) = ext->GetFusionActionNameIndex(*(int *)&Parameters[0]);
			break;
		case 15:
			*((int *)&Result) = ext->GetTotalPendingFusionActions();
			break;
		case 16:
			*((const TCHAR* *)&Result) = ext->GetQuestNameByIndex(*(int *)&Parameters[0]);
			break;
		case 17:
			*((const TCHAR* *)&Result) = ext->GetQuestStateAsString(*(TCHAR* *)&Parameters[0]);
			break;
		case 18:
			*((const TCHAR* *)&Result) = ext->GetQuestDescriptionByName(*(TCHAR* *)&Parameters[0]);
			break;
		case 19:
			*((int *)&Result) = ext->GetTotalQuestCount();
			break;

#else
	#error Incorrect DarkEdif ACE Call Table index
#endif
