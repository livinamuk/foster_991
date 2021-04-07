#include "Common.h"

struct EdifGlobal
{
	TCHAR name[256];
	void * Value;

	EdifGlobal * Next;
};

#ifdef _WIN32
Edif::Runtime::Runtime(HeaderObject * _hoPtr) : hoPtr(_hoPtr),
ObjectSelection(hoPtr->AdRunHeader)
{
}

Edif::Runtime::~Runtime()
{
}

void Edif::Runtime::Rehandle()
{
	CallRunTimeFunction2(hoPtr, RFUNCTION::REHANDLE, 0, 0);
}

void Edif::Runtime::GenerateEvent(int EventID)
{
	CallRunTimeFunction2(hoPtr, RFUNCTION::GENERATE_EVENT, EventID, 0);
}

void Edif::Runtime::PushEvent(int EventID)
{
	CallRunTimeFunction2(hoPtr, RFUNCTION::PUSH_EVENT, EventID, 0);
}

void * Edif::Runtime::Allocate(size_t size)
{
    return (void *) CallRunTimeFunction2(hoPtr, RFUNCTION::GET_STRING_SPACE_EX, 0, size * sizeof(TCHAR));
}

TCHAR * Edif::Runtime::CopyString(const TCHAR * String)
{
	TCHAR * New = NULL;
    New = (TCHAR *) Allocate(_tcslen(String) + 1);
    _tcscpy(New, String);
    
    return New;
}

char * Edif::Runtime::CopyStringEx(const char * String)
{
	char * New = NULL;
	New = (char *)CallRunTimeFunction2(hoPtr, RFUNCTION::GET_STRING_SPACE_EX, 0, (strlen(String) + 1) * sizeof(char));
	strcpy(New, String);

	return New;
}


wchar_t * Edif::Runtime::CopyStringEx(const wchar_t * String)
{
	wchar_t * New = NULL;
	New = (wchar_t *)CallRunTimeFunction2(hoPtr, RFUNCTION::GET_STRING_SPACE_EX, 0, (wcslen(String) + 1) * sizeof(wchar_t));
	wcscpy(New, String);

	return New;
}


void Edif::Runtime::Pause()
{
	CallRunTimeFunction2(hoPtr, RFUNCTION::PAUSE, 0, 0);
}

void Edif::Runtime::Resume()
{
	CallRunTimeFunction2(hoPtr, RFUNCTION::CONTINUE, 0, 0);
}

void Edif::Runtime::Redisplay()
{
	CallRunTimeFunction2(hoPtr, RFUNCTION::REDISPLAY, 0, 0);
}

void Edif::Runtime::GetApplicationDrive(TCHAR * Buffer)
{
	CallRunTimeFunction2(hoPtr, RFUNCTION::GET_FILE_INFOS, (WPARAM)FILEINFO::DRIVE, (long) Buffer);
}

void Edif::Runtime::GetApplicationDirectory(TCHAR * Buffer)
{
	CallRunTimeFunction2(hoPtr, RFUNCTION::GET_FILE_INFOS, (WPARAM)FILEINFO::DIR, (long) Buffer);
}

void Edif::Runtime::GetApplicationPath(TCHAR * Buffer)
{
	CallRunTimeFunction2(hoPtr, RFUNCTION::GET_FILE_INFOS, (WPARAM)FILEINFO::PATH, (long) Buffer);
}

void Edif::Runtime::GetApplicationName(TCHAR * Buffer)
{
	CallRunTimeFunction2(hoPtr, RFUNCTION::GET_FILE_INFOS, (WPARAM)FILEINFO::APP_NAME, (long) Buffer);
}

void Edif::Runtime::GetApplicationTempPath(TCHAR * Buffer)
{
	CallRunTimeFunction2(hoPtr, RFUNCTION::GET_FILE_INFOS, (WPARAM)FILEINFO::TEMP_PATH, (long) Buffer);
}

void Edif::Runtime::Redraw()
{
	CallRunTimeFunction2(hoPtr, RFUNCTION::REDRAW, 0, 0);
}

void Edif::Runtime::Destroy()
{
	CallRunTimeFunction2(hoPtr, RFUNCTION::DESTROY, 0, 0);
}

void Edif::Runtime::ExecuteProgram(ParamProgram * Program)
{
	CallRunTimeFunction2(hoPtr, RFUNCTION::EXECUTE_PROGRAM, 0, (long) Program);
}

long Edif::Runtime::EditInteger(EditDebugInfo * EDI)
{
    return CallRunTimeFunction2(hoPtr, RFUNCTION::EDIT_INT, 0, (long) EDI);
}

long Edif::Runtime::EditText(EditDebugInfo * EDI)
{
    return CallRunTimeFunction2(hoPtr, RFUNCTION::EDIT_TEXT, 0, (long) EDI);
}

void Edif::Runtime::CallMovement(int ID, long Parameter)
{
	CallRunTimeFunction2(hoPtr, RFUNCTION::CALL_MOVEMENT, ID, Parameter);
}

void Edif::Runtime::SetPosition(int X, int Y)
{
	CallRunTimeFunction2(hoPtr, RFUNCTION::SET_POSITION, X, Y);
}

CallTables * Edif::Runtime::GetCallTables()
{
    return (CallTables *)CallRunTimeFunction2(hoPtr, RFUNCTION::GET_CALL_TABLES, 0, 0);
}

bool Edif::Runtime::IsHWA()
{
#ifdef _WIN32
	return hoPtr->AdRunHeader->rh4.rh4Mv->CallFunction(NULL, CallFunctionIDs::ISHWA, 0, 0, 0) == 1;
#else
	// TODO: This is wrong
	return false;
#endif
}

bool Edif::Runtime::IsUnicode()
{
#ifdef _WIN32
	return hoPtr->AdRunHeader->rh4.rh4Mv->CallFunction(NULL, CallFunctionIDs::ISUNICODE, 0, 0, 0) == 1;
#else
	// TODO: This is wrong
	return false;
#endif
}

event2 &Edif::Runtime::CurrentEvent()
{
    return *(event2 *) (((char *) param1) - CND_SIZE);
}

RunObject * Edif::Runtime::RunObjPtrFromFixed(int fixedvalue)
{
	objectsList * objList = hoPtr->AdRunHeader->ObjectList;
	int index = fixedvalue & 0x0000FFFF;

	if (index < 0 || index >= hoPtr->AdRunHeader->MaxObjects)
		return NULL;

	RunObject * theObject = (RunObject *)objList[index].oblOffset;

	if (theObject == NULL)
		return NULL;
	else if (FixedFromRunObjPtr(theObject) != fixedvalue)
		return NULL;

	return theObject;
}

int Edif::Runtime::FixedFromRunObjPtr(RunObject * object)
{
	if (object != NULL)
		return (object->roHo.CreationId << 16) + object->roHo.Number;
	return 0;
}

extern HINSTANCE hInstLib;

void Edif::Runtime::WriteGlobal(const TCHAR * name, void * Value)
{
    RunHeader * rhPtr = hoPtr->AdRunHeader;

	while (rhPtr->App->ParentApp)
		rhPtr = rhPtr->App->ParentApp->Frame->rhPtr;

    EdifGlobal * Global = (EdifGlobal *) rhPtr->rh4.rh4Mv->GetExtUserData(rhPtr->App, hInstLib);

    if (!Global)
    {
        Global = new EdifGlobal;

        _tcscpy(Global->name, name);
        Global->Value = Value;

        Global->Next = 0;

        rhPtr->rh4.rh4Mv->SetExtUserData(rhPtr->App, hInstLib, Global);

        return;
    }

    while (Global)
    {
        if (!_tcsicmp(Global->name, name))
        {
            Global->Value = Value;
            return;
        }

        if (!Global->Next)
            break;

        Global = Global->Next;
    }

    Global->Next = new EdifGlobal;
    Global = Global->Next;

    _tcscpy(Global->name, name);

    Global->Value = Value;
    Global->Next = 0;
}

void * Edif::Runtime::ReadGlobal(const TCHAR * name)
{
    RunHeader * rhPtr = hoPtr->AdRunHeader;
	
	while (rhPtr->App->ParentApp)
		rhPtr = rhPtr->App->ParentApp->Frame->rhPtr;

    EdifGlobal * Global = (EdifGlobal *) rhPtr->rh4.rh4Mv->GetExtUserData(rhPtr->App, hInstLib);

    while (Global)
    {
        if (!_tcsicmp(Global->name, name))
            return Global->Value;

        Global = Global->Next;
    }

    return NULL;
}

#elif defined(__ANDROID__) // !_WIN32

Edif::Runtime::Runtime(RuntimeFunctions &runFuncs, jobject javaExtPtr2) : 
	runFuncs(runFuncs), javaExtPtr(javaExtPtr2), ObjectSelection(NULL)
{
	javaExtPtrClass = global_env->GetObjectClass(javaExtPtr);
	
	std::string exc;
	if (javaExtPtrClass.invalid()) {
		exc = GetJavaExceptionStr();
		LOGE("Could not get javaExtPtrClass, got exception %s.", exc.c_str());
	}
	
	jfieldID javaHoField = global_env->GetFieldID(javaExtPtrClass, "ho", "LObjects/CExtension;");
	if (javaHoField == NULL) {
		exc = GetJavaExceptionStr();
		LOGE("Could not get javaHoField, got exception %s.", exc.c_str());
	}

	javaHoObject = global_env->GetObjectField(javaExtPtr, javaHoField);
	if (javaHoObject.invalid()) {
		exc = GetJavaExceptionStr();
		LOGE("Could not get javaHoObject, got exception %s.", exc.c_str());
	}

	javaHoClass = global_env->GetObjectClass(javaHoObject);
	if (javaHoClass.invalid()) {
		exc = GetJavaExceptionStr();
		LOGE("Could not find javaHoClass method, got exception %s.", exc.c_str());
	}
}

#define GenEdifFunction(x) \
	static jmethodID javaMethodID = global_env->GetMethodID(javaHoClass, x, "()V"); \
	if (javaMethodID == NULL) {\
		std::string exc = GetJavaExceptionStr(); \
		__android_log_print(ANDROID_LOG_ERROR, "MMFRuntimeNative", "Could not find %s method, got exception %s.", x, exc.c_str()); \
	} \
	else \
		global_env->CallVoidMethod(javaHoObject, javaMethodID);

Edif::Runtime::~Runtime()
{
	javaExtPtr = NULL;
}

void Edif::Runtime::Rehandle()
{
	LOGI("Calling ReHandle, awaiting... class = %p", (jclass)javaHoClass);
	static jmethodID javaMethodID = global_env->GetMethodID(javaHoClass.ref, "reHandle", "()V");
	if (javaMethodID == NULL) {
		std::string exc = GetJavaExceptionStr();
		__android_log_print(ANDROID_LOG_ERROR, "MMFRuntimeNative", "Could not find %s method, got exception %s.", "reHandle", exc.c_str());
	}
	else {
		LOGI("Calling ReHandle on method %p, object %p...", javaMethodID, javaHoObject.ref);
		global_env->CallVoidMethod(javaHoObject.ref, javaMethodID);
		LOGI("ReHandle success. %d", 0);
		if (global_env->ExceptionCheck())
		{
			std::string exc2 = GetJavaExceptionStr();
			LOGE("ReHandle exception, %s.", exc2.c_str());
		}
		
	}
	// GenEdifFunction reHandle
}

void Edif::Runtime::GenerateEvent(int EventID)
{
	jmethodID javaMethodID = global_env->GetMethodID(javaHoClass, "generateEvent", "(II)V");
	global_env->CallVoidMethod(javaHoObject, javaMethodID, EventID, 0);
	//runFuncs.generateEvent(javaHoObject, EventID, 0);
}

void Edif::Runtime::PushEvent(int EventID)
{
	jmethodID javaMethodID = global_env->GetMethodID(javaHoClass, "pushEvent", "(II)V");
	global_env->CallVoidMethod(javaHoObject, javaMethodID, EventID, 0);
}

void * Edif::Runtime::Allocate(size_t size)
{
	LOGF("Function %s not implemented in DarkEdif Android, and cannot be called.", __PRETTY_FUNCTION__);
	// TODO: Allocate missing.
	// return (void *)CallRunTimeFunction2(hoPtr, RFUNCTION::GET_STRING_SPACE_EX, 0, size * sizeof(TCHAR));
	return NULL;
}

TCHAR * Edif::Runtime::CopyString(const TCHAR * String)
{
	return (TCHAR  *)String;
}

char * Edif::Runtime::CopyStringEx(const char * String)
{
	return (char *)String;
}


wchar_t * Edif::Runtime::CopyStringEx(const wchar_t * String)
{
	return (wchar_t *)String;
}



void Edif::Runtime::Pause()
{
	GenEdifFunction("pause");
}

void Edif::Runtime::Resume()
{
	GenEdifFunction("resume");
}

void Edif::Runtime::Redisplay()
{
	GenEdifFunction("redisplay");
}

void Edif::Runtime::GetApplicationDrive(TCHAR * Buffer)
{
	LOGF("Function %s not implemented in DarkEdif Android, and cannot be called.", __PRETTY_FUNCTION__);
	// TODO: This is wrong, but not crucial.
	// CallRunTimeFunction2(hoPtr, RFUNCTION::GET_FILE_INFOS, (WPARAM)FILEINFO::DRIVE, (long)Buffer);
}

void Edif::Runtime::GetApplicationDirectory(TCHAR * Buffer)
{
	LOGF("Function %s not implemented in DarkEdif Android, and cannot be called.", __PRETTY_FUNCTION__);
	// TODO: This is wrong, but not crucial.
	//CallRunTimeFunction2(hoPtr, RFUNCTION::GET_FILE_INFOS, (WPARAM)FILEINFO::DIR, (long)Buffer);
}

void Edif::Runtime::GetApplicationPath(TCHAR * Buffer)
{
	LOGF("Function %s not implemented in DarkEdif Android, and cannot be called.", __PRETTY_FUNCTION__);
	// TODO: This is wrong, but not crucial.
	// CallRunTimeFunction2(hoPtr, RFUNCTION::GET_FILE_INFOS, (WPARAM)FILEINFO::PATH, (long)Buffer);
}

void Edif::Runtime::GetApplicationName(TCHAR * Buffer)
{
	LOGF("Function %s not implemented in DarkEdif Android, and cannot be called.", __PRETTY_FUNCTION__);
	// TODO: This is wrong, but not crucial.
	//CallRunTimeFunction2(hoPtr, RFUNCTION::GET_FILE_INFOS, (WPARAM)FILEINFO::APP_NAME, (long)Buffer);
}

void Edif::Runtime::GetApplicationTempPath(TCHAR * Buffer)
{
	LOGF("Function %s not implemented in DarkEdif Android, and cannot be called.", __PRETTY_FUNCTION__);
	// TODO: This is wrong, but not crucial.
}

void Edif::Runtime::Redraw()
{
	GenEdifFunction("redraw");
}

void Edif::Runtime::Destroy()
{
	GenEdifFunction("destroy");
}

void Edif::Runtime::CallMovement(int ID, long Parameter)
{
	LOGF("Function %s not implemented in DarkEdif Android, and cannot be called.", __PRETTY_FUNCTION__);
	// See ~line 592, CExtension.java
	// GenEdifFunction("redisplay");
	//CallRunTimeFunction2(hoPtr, RFUNCTION::CALL_MOVEMENT, ID, Parameter);
}

void Edif::Runtime::SetPosition(int X, int Y)
{
	jmethodID javaMethodID = global_env->GetMethodID(javaExtPtrClass, "setPosition", "(II)V");
	global_env->CallVoidMethod(javaExtPtr, javaMethodID, X, Y);
}

static EdifGlobal * staticEdifGlobal; // LB says static/global values are functionally equivalent to getUserExtData, so... yay.

void Edif::Runtime::WriteGlobal(const TCHAR * name, void * Value)
{
	EdifGlobal * Global = (EdifGlobal *)staticEdifGlobal;

	if (!Global)
	{
		Global = new EdifGlobal;

		_tcscpy(Global->name, name);
		Global->Value = Value;
		Global->Next = NULL;

		staticEdifGlobal = Global;

		return;
	}

	while (Global)
	{
		if (!_tcsicmp(Global->name, name))
		{
			Global->Value = Value;
			return;
		}

		if (!Global->Next)
			break;

		Global = Global->Next;
	}

	Global->Next = new EdifGlobal;
	Global = Global->Next;

	_tcscpy(Global->name, name);

	Global->Value = Value;
	Global->Next = 0;
}

/*
// This code may be used by globals, if we ever re-integrate JNI method with getStorage and whatnot
static char * GetObjectNameWithPackage(const char * name)
{
	static bool gotten = false;
	static std::string packageName = "";
	if (!gotten)
	{
		jclass javaExtPtrClass = global_env->GetObjectClass(javaExtPtr);
		jmethodID getClassNameMethod = global_env->GetMethodID(javaExtPtrClass, "getName", "()Ljava/lang/String;");
		jstring className = (jstring)global_env->CallObjectMethod(javaExtPtrClass, getClassNameMethod);
		const char * classNameCPtr = global_env->GetStringUTFChars(className, NULL);
		packageName = classNameCPtr;
		global_env->ReleaseStringUTFChars(className, classNameCPtr);
		gotten = true;
	}

	static char newName[256];
	sprintf(newName, "L%s/%s;", packageName.c_str(), name);
	return newName;
}*/

void * Edif::Runtime::ReadGlobal(const TCHAR * name)
{
	/*
	JNIEnv * global_env = NULL;
	// Access CRun from CRunExtension::rh
	// Access CRunApp from CRun::rhApp
	// Access CRunApp from CRunApp::parentApp
	// Access CRun again from CRunApp::run

	jclass javaExtPtrClass = global_env->GetObjectClass(javaExtPtr);
	jclass CRunClass = global_env->FindClass(GetObjectNameWithPackage("CRun"));
	jclass CRunAppClass = global_env->FindClass(GetObjectNameWithPackage("CRunApp"));

	jfieldID CRunExtension_rh_fieldID = global_env->GetFieldID(javaExtPtrClass, "rh", GetObjectNameWithPackage("CRun"));
	jfieldID CRun_rhApp_fieldID = global_env->GetFieldID(CRunClass, "rhApp", GetObjectNameWithPackage("CRunApp"));
	jfieldID CRunApp_parentApp_fieldID = global_env->GetFieldID(CRunAppClass, "parentApp", GetObjectNameWithPackage("CRunApp"));
	jfieldID CRunApp_run_fieldID = global_env->GetFieldID(CRunAppClass, "run", GetObjectNameWithPackage("CRunApp"));
	
	jobject cRun = global_env->GetObjectField(javaExtPtr, CRunExtension_rh_fieldID);
	jobject cRunApp = global_env->GetObjectField(javaExtPtr, CRun_rhApp_fieldID);
	
	for (jobject cRunAppTemp = cRunApp; cRunAppTemp != NULL; )
	{
		cRunApp = cRunAppTemp;
		cRun = global_env->GetObjectField(cRunApp, CRunApp_run_fieldID);
	
		cRunAppTemp = global_env->GetObjectField(cRunApp, CRunApp_parentApp_fieldID);
	}
	char methodParams[256];
	
	sprintf(methodParams, "(%sI)V", GetObjectNameWithPackage("EdifGlobal"));
	jmethodID CRun_addStorage_methodID = global_env->GetMethodID(CRunClass, "addStorage", methodParams);

	sprintf(methodParams, "(I)%s", GetObjectNameWithPackage("EdifGlobal"));
	jmethodID CRun_getStorage_methodID = global_env->GetMethodID(CRunClass, "getStorage", methodParams);

	jmethodID CRun_delStorage_methodID = global_env->GetMethodID(CRunClass, "delStorage", "(I)V");

	int id = 'EDIF';

	jobject edifData = global_env->CallObjectMethod(cRun, CRun_getStorage_methodID, id);
	*/
	EdifGlobal * Global = staticEdifGlobal;

	while (Global)
	{
		if (!_tcsicmp(Global->name, name))
			return Global->Value;

		Global = Global->Next;
	}

	return NULL;
}

#else

Edif::Runtime::Runtime(RuntimeFunctions& runFuncs, jobject obj) :
	runFuncs(runFuncs), ObjectSelection(NULL)
{

}

#endif
