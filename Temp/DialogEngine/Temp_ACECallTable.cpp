// AltPlatformPostBuildTool: This file was generated for a Extension.cpp/h that was modified at 2021-05-08T16:12:27.9598813Z.
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
			case 17:
				return ext->IsCurrentBagGeneral();
			case 18:
				return ext->IsCurrentBagWearable();
			case 19:
				return ext->IsCurrentBagEquipable();
			case 20:
				return ext->IsCurrentBagMaterial();
			case 21:
				return ext->IsCurrentBagConsumable();
			case 22:
				return ext->IsCurrentBagQuest();

#elif DARKEDIF_ACE_CALL_TABLE_INDEX==1
			case 0:
				return ext->LoadDialogFile(*(TCHAR* *)&Parameters[0]), 0;
			case 1:
				return ext->LoadQuestFile(*(TCHAR* *)&Parameters[0]), 0;
			case 2:
				return ext->LoadPlayerDataFile(*(TCHAR* *)&Parameters[0]), 0;
			case 3:
				return ext->LoadInWorldDialogFile(*(TCHAR* *)&Parameters[0]), 0;
			case 4:
				return ext->SavePlayerDataFile(*(TCHAR* *)&Parameters[0]), 0;
			case 5:
				return ext->ShowDialogByID(*(int *)&Parameters[0]), 0;
			case 6:
				return ext->SelectResponseByIndex(*(int *)&Parameters[0]), 0;
			case 7:
				return ext->SetGameValue(*(TCHAR* *)&Parameters[0], *(float *)&Parameters[1]), 0;
			case 8:
				return ext->SetGameValueMin(*(TCHAR* *)&Parameters[0], *(float *)&Parameters[1]), 0;
			case 9:
				return ext->SetGameValueMax(*(TCHAR* *)&Parameters[0], *(float *)&Parameters[1]), 0;
			case 10:
				return ext->AddToGameValue(*(TCHAR* *)&Parameters[0], *(float *)&Parameters[1]), 0;
			case 11:
				return ext->SubtractFromValue(*(TCHAR* *)&Parameters[0], *(float *)&Parameters[1]), 0;
			case 12:
				return ext->SetGameFlagToTrue(*(TCHAR* *)&Parameters[0]), 0;
			case 13:
				return ext->SetGameFlagToFalse(*(TCHAR* *)&Parameters[0]), 0;
			case 14:
				return ext->ToggleGameFlag(*(TCHAR* *)&Parameters[0]), 0;
			case 15:
				return ext->RegisterItemFixedValue(*(TCHAR* *)&Parameters[0], *(int *)&Parameters[1]), 0;
			case 16:
				return ext->GiveItem(*(TCHAR* *)&Parameters[0], *(int *)&Parameters[1]), 0;
			case 17:
				return ext->TakeItem(*(TCHAR* *)&Parameters[0], *(int *)&Parameters[1]), 0;
			case 18:
				return ext->PickedUpFusionItem(*(int *)&Parameters[0]), 0;
			case 19:
				return ext->TriggerFusionActionByName(*(TCHAR* *)&Parameters[0]), 0;
			case 20:
				return ext->ActivateQuestByName(*(TCHAR* *)&Parameters[0]), 0;
			case 21:
				return ext->CompleteQuestByName(*(TCHAR* *)&Parameters[0]), 0;
			case 22:
				return ext->FailQuestByName(*(TCHAR* *)&Parameters[0]), 0;
			case 23:
				return ext->CheckForQuestCompletion(), 0;
			case 24:
				return ext->RegisterNPCFixedValue(*(TCHAR* *)&Parameters[0], *(int *)&Parameters[1]), 0;
			case 25:
				return ext->CheckNPCQuestDialog(*(int *)&Parameters[0]), 0;
			case 26:
				return ext->CheckNPCInWorldDialog(*(int *)&Parameters[0]), 0;
			case 27:
				return ext->SetTextTimerDuration(*(int *)&Parameters[0]), 0;
			case 28:
				return ext->CheckNPCInWorldQuestDialogText(*(int *)&Parameters[0]), 0;
			case 29:
				return ext->CheckNPCStandardDialogText(*(int *)&Parameters[0]), 0;
			case 30:
				return ext->ClearAllData(), 0;
			case 31:
				return ext->UpdateEngine(), 0;
			case 32:
				return ext->LoadItemDatabase(*(TCHAR* *)&Parameters[0]), 0;
			case 33:
				return ext->DespositItemIntoContainer(*(TCHAR* *)&Parameters[0], *(TCHAR* *)&Parameters[1], *(int *)&Parameters[2], *(int *)&Parameters[3]), 0;
			case 34:
				return ext->WithdrawItemFromContainer(*(TCHAR* *)&Parameters[0], *(TCHAR* *)&Parameters[1], *(int *)&Parameters[2], *(int *)&Parameters[3]), 0;
			case 35:
				return ext->MoveInventoryItem(*(TCHAR* *)&Parameters[0], *(int *)&Parameters[1]), 0;
			case 36:
				return ext->MoveContaineryItem(*(TCHAR* *)&Parameters[0], *(int *)&Parameters[1]), 0;
			case 37:
				return ext->SetInventoryBagGeneralSize(*(int *)&Parameters[0]), 0;
			case 38:
				return ext->SetInventoryBagWearableSize(*(int *)&Parameters[0]), 0;
			case 39:
				return ext->SetInventoryBagEquipableSize(*(int *)&Parameters[0]), 0;
			case 40:
				return ext->SetInventoryBagMaterialSize(*(int *)&Parameters[0]), 0;
			case 41:
				return ext->SetInventoryBagConsumableSize(*(int *)&Parameters[0]), 0;
			case 42:
				return ext->SetInventoryBagQuestSize(*(int *)&Parameters[0]), 0;
			case 43:
				return ext->ShowContainerInventory(), 0;
			case 44:
				return ext->ShowCompanionInventory(), 0;
			case 45:
				return ext->setCurrentBagToGeneral(), 0;
			case 46:
				return ext->setCurrentBagToWearable(), 0;
			case 47:
				return ext->setCurrentBagToEquipable(), 0;
			case 48:
				return ext->setCurrentBagToMaterial(), 0;
			case 49:
				return ext->setCurrentBagToConsumable(), 0;
			case 50:
				return ext->setCurrentBagToQuest(), 0;
			case 51:
				return ext->SetCurrentContainerByName(*(TCHAR* *)&Parameters[0]), 0;
			case 52:
				return ext->SetCurrentCompanionByName(*(TCHAR* *)&Parameters[0]), 0;
			case 53:
				return ext->EquipToHead(*(TCHAR* *)&Parameters[0]), 0;
			case 54:
				return ext->EquipToUpperBody(*(TCHAR* *)&Parameters[0]), 0;
			case 55:
				return ext->EquipToLowerBody(*(TCHAR* *)&Parameters[0]), 0;
			case 56:
				return ext->EquipToFeet(*(TCHAR* *)&Parameters[0]), 0;
			case 57:
				return ext->EquipToHands(*(TCHAR* *)&Parameters[0]), 0;
			case 58:
				return ext->EquipToBelt(*(TCHAR* *)&Parameters[0]), 0;
			case 59:
				return ext->EquipToEquipSlot(*(TCHAR* *)&Parameters[0]), 0;

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
			*((int *)&Result) = ext->GetCurrentDialogID();
			break;
		case 6:
			*((float *)&Result) = ext->GetGameValueByName(*(TCHAR* *)&Parameters[0]);
			break;
		case 7:
			*((const TCHAR* *)&Result) = ext->GetGameValueNameByIndex(*(int *)&Parameters[0]);
			break;
		case 8:
			*((int *)&Result) = ext->GetTotalGameValueCount();
			break;
		case 9:
			*((const TCHAR* *)&Result) = ext->GetGameFlagStateAsString(*(TCHAR* *)&Parameters[0]);
			break;
		case 10:
			*((const TCHAR* *)&Result) = ext->GetGameFlagNameByIndex(*(int *)&Parameters[0]);
			break;
		case 11:
			*((int *)&Result) = ext->GetTotalGameFlagCount();
			break;
		case 12:
			*((int *)&Result) = ext->GetGameItemQuantityByName(*(TCHAR* *)&Parameters[0]);
			break;
		case 13:
			*((const TCHAR* *)&Result) = ext->GetResponseTextByIndex(*(int *)&Parameters[0]);
			break;
		case 14:
			*((int *)&Result) = ext->GetAvaliableReponseCount();
			break;
		case 15:
			*((const TCHAR* *)&Result) = ext->GetFusionActionNameIndex(*(int *)&Parameters[0]);
			break;
		case 16:
			*((int *)&Result) = ext->GetTotalPendingFusionActions();
			break;
		case 17:
			*((const TCHAR* *)&Result) = ext->GetQuestNameByIndex(*(int *)&Parameters[0]);
			break;
		case 18:
			*((const TCHAR* *)&Result) = ext->GetQuestStateAsString(*(TCHAR* *)&Parameters[0]);
			break;
		case 19:
			*((const TCHAR* *)&Result) = ext->GetQuestDescriptionByName(*(TCHAR* *)&Parameters[0]);
			break;
		case 20:
			*((int *)&Result) = ext->GetTotalQuestCount();
			break;
		case 21:
			*((int *)&Result) = ext->GetNPCCount();
			break;
		case 22:
			*((const TCHAR* *)&Result) = ext->GetNPCInWorldDialogStringByIndex(*(int *)&Parameters[0]);
			break;
		case 23:
			*((int *)&Result) = ext->GetNPCFixedValueByIndex(*(int *)&Parameters[0]);
			break;
		case 24:
			*((int *)&Result) = ext->GetInventoryBagGeneralSize();
			break;
		case 25:
			*((int *)&Result) = ext->GetInventoryBagWearableSize();
			break;
		case 26:
			*((int *)&Result) = ext->GetInventoryBagEquipableSize();
			break;
		case 27:
			*((int *)&Result) = ext->GetInventoryBagMaterialSize();
			break;
		case 28:
			*((int *)&Result) = ext->GetInventoryBagConsumableSize();
			break;
		case 29:
			*((int *)&Result) = ext->GetInventoryBagQuestsize();
			break;
		case 30:
			*((const TCHAR* *)&Result) = ext->GetGeneralBagItemNameByIndex(*(int *)&Parameters[0]);
			break;
		case 31:
			*((const TCHAR* *)&Result) = ext->GetWearableBagItemNameByIndex(*(int *)&Parameters[0]);
			break;
		case 32:
			*((const TCHAR* *)&Result) = ext->GetMaterialItemNameByIndex(*(int *)&Parameters[0]);
			break;
		case 33:
			*((const TCHAR* *)&Result) = ext->GetConsumableBagItemNameByIndex(*(int *)&Parameters[0]);
			break;
		case 34:
			*((const TCHAR* *)&Result) = ext->GetEquipableBagItemNameByIndex(*(int *)&Parameters[0]);
			break;
		case 35:
			*((const TCHAR* *)&Result) = ext->GetQuestBagItemNameByIndex(*(int *)&Parameters[0]);
			break;
		case 36:
			*((const TCHAR* *)&Result) = ext->GetCurrentCompanionName();
			break;
		case 37:
			*((const TCHAR* *)&Result) = ext->GetCurrentContainerName();
			break;
		case 38:
			*((int *)&Result) = ext->GetSizeOfContainerByName(*(TCHAR* *)&Parameters[0]);
			break;
		case 39:
			*((const TCHAR* *)&Result) = ext->GetItemTypeAsStringByName(*(const TCHAR* *)&Parameters[0]);
			break;
		case 40:
			*((float *)&Result) = ext->GetItemWeightByName(*(const TCHAR* *)&Parameters[0]);
			break;
		case 41:
			*((const TCHAR* *)&Result) = ext->GetItemDescriptionByName(*(const TCHAR* *)&Parameters[0]);
			break;
		case 42:
			*((int *)&Result) = ext->GetItemNumberOfModifierEffectsByName(*(const TCHAR* *)&Parameters[0]);
			break;
		case 43:
			*((const TCHAR* *)&Result) = ext->GetItemModiferEffectNameByIndex(*(const TCHAR* *)&Parameters[0], *(int *)&Parameters[1]);
			break;
		case 44:
			*((int *)&Result) = ext->GetItemModiferEffectValueByIndex(*(const TCHAR* *)&Parameters[0], *(int *)&Parameters[1]);
			break;
		case 45:
			*((const TCHAR* *)&Result) = ext->GetCurrentBagItemNameByIndex(*(int *)&Parameters[0]);
			break;
		case 46:
			*((int *)&Result) = ext->GetCurrentBagIteQuantityByIndex(*(int *)&Parameters[0]);
			break;
		case 47:
			*((int *)&Result) = ext->GetCurrentBagSize();
			break;
		case 48:
			*((const TCHAR* *)&Result) = ext->GetCurrentContainerName();
			break;
		case 49:
			*((int *)&Result) = ext->GetCurrentContainerSize();
			break;
		case 50:
			*((const TCHAR* *)&Result) = ext->GetCurrentContainerItemNameByIndex(*(int *)&Parameters[0]);
			break;
		case 51:
			*((int *)&Result) = ext->GetCurrentContainerItemQuanityByIndex(*(int *)&Parameters[0]);
			break;
		case 52:
			*((const TCHAR* *)&Result) = ext->GetCurrentContainerIconName();
			break;
		case 53:
			*((const TCHAR* *)&Result) = ext->GetItemNameEquippedToHead();
			break;
		case 54:
			*((const TCHAR* *)&Result) = ext->GetItemNameEquippedToUpperBody();
			break;
		case 55:
			*((const TCHAR* *)&Result) = ext->GetItemNameEquippedToLowerBody();
			break;
		case 56:
			*((const TCHAR* *)&Result) = ext->GetItemNameEquippedToFeet();
			break;
		case 57:
			*((const TCHAR* *)&Result) = ext->GetItemNameEquippedToHands();
			break;
		case 58:
			*((const TCHAR* *)&Result) = ext->GetItemNameEquippedToBelt();
			break;
		case 59:
			*((const TCHAR* *)&Result) = ext->GetItemNameEquippedToEquipSlot();
			break;
		case 60:
			*((float *)&Result) = ext->GetCurrentWeight();
			break;
		case 61:
			*((float *)&Result) = ext->GetMaxWeight();
			break;

#else
	#error Incorrect DarkEdif ACE Call Table index
#endif
