
#include "Edif.h"
#include "src/DialogEngine.h"

class Extension
{
public:
	// Hide stuff requiring other headers
	//SaveExtInfo threadData; // Must be first variable in Extension class

#ifdef _WIN32
	RUNDATA * rdPtr;
	RunHeader * rhPtr;
#elif defined(__ANDROID__)
	RuntimeFunctions &runFuncs;
	global<jobject> javaExtPtr;
#else
    RuntimeFunctions& runFuncs;
    void* javaExtPtr;
#endif

    Edif::Runtime Runtime;

    static const int MinimumBuild = 251;
    static const int Version = 1;

	static const OEFLAGS OEFLAGS = OEFLAGS::NEVER_KILL | OEFLAGS::NEVER_SLEEP; // Use OEFLAGS namespace
    static const OEPREFS OEPREFS = OEPREFS::GLOBAL; // Use OEPREFS namespace
    
    static const int WindowProcPriority = 100;

#ifdef _WIN32
    Extension(RUNDATA * rdPtr, EDITDATA * edPtr, CreateObjectInfo * cobPtr);
#else
    Extension(RuntimeFunctions& runFuncs, EDITDATA* edPtr, jobject javaExtPtr);
#endif
    ~Extension();


    /*  Add any data you want to store in your extension to this class
        (eg. what you'd normally store in rdPtr).
		
		For those using multi-threading, any variables that are modified
		by the threads should be in SaveExtInfo.
		See MultiThreading.h.

        Unlike rdPtr, you can store real C++ objects with constructors
        and destructors, without having to call them manually or store
        a pointer.
    */
	
    // int MyVariable;

    /*  Add your actions, conditions and expressions as real class member
        functions here. The arguments (and return type for expressions) must
        match EXACTLY what you defined in the JSON.

        Remember to link the actions, conditions and expressions to their
        numeric IDs in the class constructor (Extension.cpp)
    */

    /*struct FixedValueTimer
    {
        int fixedValue;
        int timer;
    };
    std::vector<FixedValueTimer> m_fixedValueTimers;*/


    /// Actions

        void LoadDialogFile(TCHAR* name);
        void LoadQuestFile(TCHAR* name);
        void LoadPlayerDataFile(TCHAR* name);
        void SavePlayerDataFile(TCHAR* name);

        void ShowDialogByID(int ID);
        void SelectResponseByIndex(int index);

        void SetGameValue(TCHAR* name, float value);
        void SetGameValueMin(TCHAR* name, float value);
        void SetGameValueMax(TCHAR* name, float value);
        void AddToGameValue(TCHAR* name, float value);
        void SubtractFromValue(TCHAR* name, float value);

        void SetGameFlagToTrue(TCHAR* name);
        void SetGameFlagToFalse(TCHAR* name);
        void ToggleGameFlag(TCHAR* name);

        void GiveItem(TCHAR* name, int quanitity);
        void TakeItem(TCHAR* name, int quanitity);

        void TriggerFusionActionByName(TCHAR* name);

        void ActivateQuestByName(TCHAR* name);
        void CompleteQuestByName(TCHAR* name);
        void FailQuestByName(TCHAR* name);
        void CheckForQuestCompletion();

        void ClearAllData();


        void RegisterNPCFixedValue(TCHAR* name, int value);
        void RegisterItemFixedValue(TCHAR* name, int value);
        void PickedUpFusionItem(int fixedValue);
        
        void CheckNPCQuestDialog(int fixedValue);
        void CheckNPCInWorldDialog(int fixedValue);
        void CheckNPCInWorldQuestDialogText(int fixedValue);
        void CheckNPCStandardDialogText(int fixedValue);
        void LoadInWorldDialogFile(TCHAR* name);
        //void AddFixedValueTimer(int fixedValue, int timerDuration);
        

        void UpdateEngine();
        void SetTextTimerDuration(int duration);

		
	/// Conditions

        bool IsThereDialogToShow();
        bool IsDialogOver();
        bool IsReponseASpecialColor(int index);
        bool DoesValueExist(TCHAR* name);
        bool DoesPlayerHaveItem(TCHAR* name);
        bool IsFusionActionTriggered(TCHAR* name);
        bool IsReponseEmpty(int index);
        bool CompareDialogImageName(TCHAR* name);
        bool IsFlagTrue(TCHAR* name);
        bool IsFlagFalse(TCHAR* name);

        bool IsQuestActive(TCHAR* name);
        bool IsQuestInactive(TCHAR* name);
        bool AreQuestRequirementsFullfilled(TCHAR* name);
        bool IsQuestComplete(TCHAR* name); 
        bool IsQuestFailed(TCHAR* name);
        bool WasAnyQuestJustCompleted();
        bool WasAnyQuestJustFailed();

    /// Expressions
        const TCHAR* GetCurrentDialogText();
        const TCHAR* GetcurrentDialogImageName();
        const TCHAR* GetResponseTextByIndex(int index);
        int GetAvaliableReponseCount();

        float GetGameValueByName(TCHAR* name);
        const TCHAR* GetGameValueNameByIndex(int index);
        int GetTotalGameValueCount();

        const TCHAR* GetGameFlagStateAsString(TCHAR* name);
        const TCHAR* GetGameFlagNameByIndex(int index);
        int GetTotalGameFlagCount();

        int GetGameItemQuantityByName(TCHAR* name);
        const TCHAR* GetGameItemNameByIndex(int index);
        int GetTotalGameItems();

        const TCHAR* GetFusionActionNameIndex(int index);
        int GetTotalPendingFusionActions();
        int GetLastSelectedResponseIndex();

        const TCHAR* GetQuestStateAsString(TCHAR* name);
        const TCHAR* GetQuestNameByIndex(int index);
        int GetTotalQuestCount();
        const TCHAR* GetQuestDescriptionByName(TCHAR* name);
       
        //const TCHAR* GetCurrentInWorldDialogString();
        int GetCurrentDialogID();

       // int Get_FixedValueTimer_TimerByFixedValue(int fixedValue);
        int GetNPCCount();
        const TCHAR* GetNPCInWorldDialogStringByIndex(int index);
        int GetNPCFixedValueByIndex(int index);
       // int Get_FixedValueTimer_TimerByIndex(int index);

    /* These are called if there's no function linked to an ID */

    /*    std::string TCharToString(TCHAR* tchar)
        {
            std::string str;
            for (int i = 0; i < _tcslen(tchar); i++)
                str += tchar[i];
            return str;
        }

        const TCHAR* StringToTChar(std::string str)
        {
            std::basic_string<TCHAR> converted(str.begin(), str.end());
            const TCHAR* tchar = converted.c_str();
            return tchar;
        }*/

    void Action(int ID);
    long Condition(int ID);
    long Expression(int ID);

	

    /*  These replace the functions like HandleRunObject that used to be
        implemented in Runtime.cpp. They work exactly the same, but they're
        inside the extension class.
    */

    REFLAG Handle();
    short Display();

    short Pause();
    short Continue();

    bool Save(HANDLE File);
    bool Load(HANDLE File);

};
