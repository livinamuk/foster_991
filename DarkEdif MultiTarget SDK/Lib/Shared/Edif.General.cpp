#include "Common.h"

#ifdef _WIN32

const TCHAR ** Dependencies = 0;

const TCHAR ** DLLExport GetDependencies()
{
	if (!Dependencies)
    {
        const json_value &DependenciesJSON = SDK->json["Dependencies"];

        Dependencies = new const TCHAR * [DependenciesJSON.u.object.length + 2];

        int Offset = 0;

        if (Edif::ExternalJSON)
        {
            TCHAR JSONFilename [MAX_PATH];

#ifdef _WIN32
            GetModuleFileName (hInstLib, JSONFilename, sizeof (JSONFilename)/sizeof(TCHAR));
#endif

            TCHAR * Iterator = JSONFilename + _tcslen(JSONFilename) - 1;

            while(*Iterator != '.')
                -- Iterator;

            _tcscpy(++ Iterator, _T("json"));

            Iterator = JSONFilename + _tcslen(JSONFilename) - 1;

            while(*Iterator != '\\' && *Iterator != '/')
                -- Iterator;

            Dependencies [Offset ++] = ++ Iterator;
        }

        unsigned int i = 0;

        for(; i < DependenciesJSON.u.object.length; ++ i)
		{
			TCHAR* tstr = Edif::ConvertString(DependenciesJSON[i]);
            Dependencies[Offset + i] = tstr;
			Edif::FreeString(tstr);
		}

        Dependencies[Offset + i] = 0;
    }

    return Dependencies;
}
short DLLExport GetRunObjectInfos(mv * mV, kpxRunInfos * infoPtr)
{
	infoPtr->Conditions = &::SDK->ConditionJumps[0];
	infoPtr->Actions = &::SDK->ActionJumps[0];
	infoPtr->Expressions = &::SDK->ExpressionJumps[0];

	infoPtr->NumOfConditions = CurLang["Conditions"].u.object.length;
	infoPtr->NumOfActions = CurLang["Actions"].u.object.length;
	infoPtr->NumOfExpressions = (short)::SDK->ExpressionInfos.size();
	
	infoPtr->EDITDATASize = sizeof(EDITDATA);
#ifndef NOPROPS
	{
		const json_value JSON = CurLang["Properties"];
		for(unsigned int i = 0; i < JSON.u.object.length; ++i)
		{
			switch (::SDK->EdittimeProperties[i].Type_ID)
			{
				case Edif::Properties::PROPTYPE_EDIT_STRING:
				case Edif::Properties::PROPTYPE_STATIC:
					infoPtr->EDITDATASize += sizeof(Prop_AStr)+255;
					break;
				default:
					infoPtr->EDITDATASize += sizeof(Prop);
			}
			++infoPtr->EDITDATASize;
		}
	}
#endif //NOPROPS
	
	//+(GetPropertyChbx(edPtr, CurLang["Properties"].u.object.length+1)-&edPtr);

    infoPtr->WindowProcPriority = Extension::WindowProcPriority;

	infoPtr->EditFlags = Extension::OEFLAGS;
	infoPtr->EditPrefs = Extension::OEPREFS;

    memcpy(&infoPtr->Identifier, ::SDK->json["Identifier"], 4);
	
    infoPtr->Version = Extension::Version;
	
	return TRUE;
}

extern "C" unsigned int DLLExport GetInfos(int info)
{
	switch ((KGI)info)
	{
		case KGI::VERSION:
			return 0x300; // I'm a MMF2 extension!

		case KGI::PLUGIN:
			return 0x100; // Version 1 type o' plugin

		case KGI::PRODUCT:
			#ifdef PROEXT
				return 3; // MMF Developer
			#endif
			#ifdef TGFEXT
				return 1; // TGF2
			#endif
			return 2;		// MMF Standard

		case KGI::BUILD:
			return Extension::MinimumBuild;

	#ifdef _UNICODE
		case KGI::UNICODE:
			return TRUE;	// I'm building in Unicode
	#endif

		default:
			return 0;
	}
}

short DLLExport CreateRunObject(RUNDATA * rdPtr, EDITDATA * edPtr, CreateObjectInfo * cobPtr)
{
    /* Global to all extensions! Use the constructor of your Extension class (Extension.cpp) instead! */

    rdPtr->pExtension = new Extension(rdPtr, edPtr, cobPtr);
	rdPtr->pExtension->Runtime.ObjectSelection.pExtension = rdPtr->pExtension;
	
	return 0;
}

/* Don't touch any of these, they're global to all extensions! See Extension.cpp */

short DLLExport DestroyRunObject(RUNDATA * rdPtr, long fast)
{
	delete rdPtr->pExtension;
	rdPtr->pExtension = NULL;

	return 0;
}

REFLAG DLLExport HandleRunObject(RUNDATA * rdPtr)
{
	return rdPtr->pExtension->Handle();
}

short DLLExport DisplayRunObject(RUNDATA * rdPtr)
{
	return rdPtr->pExtension->Display();
}

short DLLExport PauseRunObject(RUNDATA * rdPtr)
{
	return rdPtr->pExtension->Pause();
}

short DLLExport ContinueRunObject(RUNDATA * rdPtr)
{
	return rdPtr->pExtension->Continue();
}

unsigned short DLLExport GetRunObjectDataSize(RunHeader * rhPtr, EDITDATA * edPtr)
{
	return (sizeof(RUNDATA));
}

BOOL SaveRunObject(RUNDATA * rdPtr, HANDLE hf)
{
	return rdPtr->pExtension->Save(hf) ? TRUE : FALSE;
}

BOOL LoadRunObject(RUNDATA * rdPtr, HANDLE hf)
{
	return rdPtr->pExtension->Load(hf) ? TRUE : FALSE;
}

#elif defined(__ANDROID__)

ProjectFunc jint getNumberOfConditions(JNIEnv *, jobject, jlong cptr)
{
	//raise(SIGTRAP);
	return CurLang["Conditions"].u.array.length;
}
typedef jobject ByteBufferDirect;
typedef jobject CCreateObjectInfo;

static RuntimeFunctions runFuncs;
ProjectFunc jlong createRunObject(JNIEnv * env, jobject javaExtPtr, ByteBufferDirect edPtr, CCreateObjectInfo coi, jint version)
{
	void * edPtrReal = global_env->GetDirectBufferAddress(edPtr);
	LOGI("Note: global_env is %p, env is %p; javaExtPtr is %p, edPtr %p, edPtrReal %p, coi %p.", global_env, env, javaExtPtr, edPtr, edPtrReal, coi);
	global<jobject> javaExtP(javaExtPtr);
	runFuncs.ext = javaExtP;

	Extension * ext = new Extension(runFuncs, (EDITDATA *)edPtrReal, javaExtPtr);
	runFuncs.ext = ext->javaExtPtr; // this is global so it's safer
	ext->Runtime.ObjectSelection.pExtension = ext;
	return (jlong)ext;
}

ProjectFunc void destroyRunObject(JNIEnv *, jobject, jlong ext, bool fast)
{
	delete ((Extension *)ext);
}

ProjectFunc REFLAG handleRunObject(JNIEnv *, jobject, jlong ext)
{
    return ((Extension *)ext)->Handle();
}

ProjectFunc short displayRunObject(JNIEnv *, jobject, jlong ext)
{
	// WARNING: not sure if this will work. Function signature was not in native SDK.
	return ((Extension *)ext)->Display();
}

ProjectFunc short pauseRunObject(JNIEnv *, jobject, jlong ext)
{
	return ((Extension *)ext)->Pause();
}

ProjectFunc short continueRunObject(JNIEnv *, jobject, jlong ext)
{
	return ((Extension *)ext)->Continue();
}
/*
// Called after JNI_OnLoad, before getNumberOfConditions. Good for Edif::Init().
ProjectFunc(jboolean, extInit)(JNIEnv *, jobject javaExtPtr)
{
	__android_log_write(ANDROID_LOG_INFO, "MMFRuntimeNative", "Inside extInit.");
	if (global_env == NULL)
		__android_log_print(ANDROID_LOG_INFO, "MMFRuntimeNative", "The global_env is NULL. Why is this.");
	else
		__android_log_print(ANDROID_LOG_INFO, "MMFRuntimeNative", "The global_env is %d. Why is this.", (int)global_env);

	// Note: javaExtPtr has CExtension ho and CRun rh inited (unlike in JNI_OnLoad). Useful stuff.

	mv * mV = NULL;
	if (!::SDK) {
		__android_log_print(ANDROID_LOG_INFO, "MMFRuntimeNative", "The SDK is being initialised.");
		Edif::Init(mV);
		return true;
	}
	__android_log_write(ANDROID_LOG_INFO, "MMFRuntimeNative", "extInit ended.");
	return false;
}*/

jclass GetExtClass(void * javaExtPtr) {
	static global<jclass> clazz(global_env->GetObjectClass((jobject)javaExtPtr));
	return clazz;
};
jobject GetRH(void * javaExtPtr) {
	static jfieldID getRH(global_env->GetFieldID(GetExtClass(javaExtPtr), "rh", "LRunLoop/CRun;"));
	return global_env->GetObjectField((jobject)javaExtPtr, getRH);
};

int act_getParamExpression(void * javaExtPtr, void * act) {
	static global<jclass> actClass(global_env->GetObjectClass((jobject)act));
	static jmethodID getActExpr(global_env->GetMethodID(actClass, "getParamExpression", "(LRunLoop/CRun;I)I"));
	return global_env->CallIntMethod((jobject)act, getActExpr, GetRH(javaExtPtr), -1);
}
RuntimeFunctions::string act_getParamExpString(void * javaExtPtr, void * act) {
	
	static global<jclass> actClass(global_env->GetObjectClass((jobject)act));
	static jmethodID getActExpr(global_env->GetMethodID(actClass, "getParamFilename2", "(LRunLoop/CRun;I)Ljava/lang/String;"));
	RuntimeFunctions::string str;
	str.ctx = (jstring)global_env->CallObjectMethod((jobject)act, getActExpr, GetRH(javaExtPtr), -1);
	str.ptr = global_env->GetStringUTFChars((jstring)str.ctx, NULL);
	return str;
}
float act_getParamExpFloat(void * javaExtPtr, void * act) {
	static global<jclass> actClass(global_env->GetObjectClass((jobject)act));
	static jmethodID getActExpr(global_env->GetMethodID(actClass, "getParamExpFloat", "(LRunLoop/CRun;I)F"));
	return global_env->CallFloatMethod((jobject)act, getActExpr, GetRH(javaExtPtr), -1);
}

int cnd_getParamExpression(void * javaExtPtr, void * cnd) {
	static global<jclass> cndClass(global_env->GetObjectClass((jobject)cnd));
	static jmethodID getcndExpr(global_env->GetMethodID(cndClass, "getParamExpression", "(LRunLoop/CRun;I)I"));
	return global_env->CallIntMethod((jobject)cnd, getcndExpr, GetRH(javaExtPtr), -1);
}
RuntimeFunctions::string cnd_getParamExpString(void * javaExtPtr, void * cnd) {
	static global<jclass> cndClass(global_env->GetObjectClass((jobject)cnd));
	static jmethodID getcndExpr(global_env->GetMethodID(cndClass, "getParamFilename2", "(LRunLoop/CRun;I)Ljava/lang/String;"));
	RuntimeFunctions::string str;
	str.ctx = (jstring)global_env->CallObjectMethod((jobject)cnd, getcndExpr, GetRH(javaExtPtr), -1);
	str.ptr = global_env->GetStringUTFChars((jstring)str.ctx, NULL);
	return str;
}
float cnd_getParamExpFloat(void * javaExtPtr, void * cnd) {
	static global<jclass> cndClass(global_env->GetObjectClass((jobject)cnd));
	static jmethodID getcndExpr(global_env->GetMethodID(cndClass, "getParamExpFloat", "(LRunLoop/CRun;I)F"));
	float f = global_env->CallFloatMethod((jobject)cnd, getcndExpr, GetRH(javaExtPtr), -1);
	return f;
}

int exp_getParamExpression(void * javaExtPtr, void * exp) {
	static global<jclass> expClass(global_env->GetObjectClass((jobject)exp));
	static jmethodID getexpExpr(global_env->GetMethodID(expClass, "getParamInt", "()I"));
	return global_env->CallIntMethod((jobject)exp, getexpExpr);
}
RuntimeFunctions::string exp_getParamExpString(void * javaExtPtr, void * exp) {
	static global<jclass> expClass(global_env->GetObjectClass((jobject)exp));
	static jmethodID getexpExpr(global_env->GetMethodID(expClass, "getParamString", "()Ljava/lang/String;"));
	RuntimeFunctions::string str;
	str.ctx = (jstring)global_env->CallObjectMethod((jobject)exp, getexpExpr);
	str.ptr = global_env->GetStringUTFChars((jstring)str.ctx, NULL);
	return str;
}
float exp_getParamExpFloat(void * javaExtPtr, void * exp) {
	static global<jclass> expClass(global_env->GetObjectClass((jobject)exp));
	static jmethodID setexpExpr(global_env->GetMethodID(expClass, "getParamFloat", "()F"));
	return global_env->CallFloatMethod((jobject)exp, setexpExpr);
}

void exp_setReturnInt(void * javaExtPtr, void * exp, int val) {
	static global<jclass> expClass(global_env->GetObjectClass((jobject)exp));
	static jmethodID setexpExpr(global_env->GetMethodID(expClass, "setReturnInt", "(I)V"));
	global_env->CallVoidMethod((jobject)exp, setexpExpr, val);
}
void exp_setReturnString(void * javaExtPtr, void * exp, const char * val) {
	static global<jclass> expClass(global_env->GetObjectClass((jobject)exp));
	static jmethodID setexpExpr(global_env->GetMethodID(expClass, "setReturnString", "(Ljava/lang/String;)V"));

	// Convert into Java memory
	jstring jStr = global_env->NewStringUTF(val);
	global_env->CallVoidMethod((jobject)exp, setexpExpr, jStr);
	global_env->DeleteLocalRef(jStr); // not strictly needed
}
void exp_setReturnFloat(void * javaExtPtr, void * exp, float val) {
	static global<jclass> expClass(global_env->GetObjectClass((jobject)exp));
	static jmethodID getexpExpr(global_env->GetMethodID(expClass, "setReturnFloat", "(F)V"));
	global_env->CallVoidMethod((jobject)exp, getexpExpr, val);
}
void freeString(void * ext, RuntimeFunctions::string str)
{
	global_env->ReleaseStringUTFChars((jstring)str.ctx, str.ptr);
	str = { NULL, NULL };
}
void generateEvent(void * javaExtPtr, int code, int param) {
	// CExtension ho in 
	static global<jclass> expClass(global_env->GetObjectClass((jobject)javaExtPtr));
	static jfieldID getHo(global_env->GetFieldID(expClass, "ho", "LObjects/CExtension;")); // ?
	jobject ho = global_env->GetObjectField((jobject)javaExtPtr, getHo);
	static global<jclass> hoClass(global_env->GetObjectClass(ho));
	static jmethodID genEvent(global_env->GetMethodID(hoClass, "generateEvent", "(II)V"));
	global_env->CallVoidMethod(ho, genEvent, code, param);
};
void pushEvent(void * javaExtPtr, int code, int param) {
	// CExtension ho in 
	static global<jclass> expClass(global_env->GetObjectClass((jobject)javaExtPtr));
	static jfieldID getHo(global_env->GetFieldID(expClass, "ho", "LObjects/CExtension;")); // ?
	jobject ho = global_env->GetObjectField((jobject)javaExtPtr, getHo);
	static global<jclass> hoClass(global_env->GetObjectClass(ho));
	static jmethodID pushEvent(global_env->GetMethodID(hoClass, "pushEvent", "(II)V"));
	global_env->CallVoidMethod(ho, pushEvent, code, param);
};
void LOGF(const char * x, ...)
{
	va_list va;
	va_start(va, x);
	__android_log_vprint(ANDROID_LOG_ERROR, "MMFRuntimeNative", x, va);
	va_end(va);
	__android_log_write(ANDROID_LOG_FATAL, "MMFRuntimeNative", "Killed by extension " PROJECT_NAME ".");
	global_env->FatalError("Killed by extension " PROJECT_NAME ".");
}
std::string GetJavaExceptionStr()
{
	if (!global_env->ExceptionCheck())
		return std::string("No exception!");
	jthrowable exc = global_env->ExceptionOccurred();
	global_env->ExceptionClear(); // else GetObjectClass fails, which is dumb enough.
	jclass exccls = global_env->GetObjectClass(exc);
	jmethodID getMsgMeth = global_env->GetMethodID(exccls, "toString", "()Ljava/lang/String;");
	
	jstring excStr = (jstring)global_env->CallObjectMethod(exc, getMsgMeth);
	const char * c = global_env->GetStringUTFChars(excStr, NULL);
	
	std::string ret(c);
	
	global_env->ReleaseStringUTFChars(excStr, c);
	return ret;
}

#include <iostream>
#include <iomanip>
#include <unwind.h>
#include <dlfcn.h>

namespace {

	struct BacktraceState
	{
		void** current;
		void** end;
	};

	static _Unwind_Reason_Code unwindCallback(struct _Unwind_Context* context, void* arg)
	{
		BacktraceState* state = static_cast<BacktraceState*>(arg);
		uintptr_t pc = _Unwind_GetIP(context);
		if (pc) {
			if (state->current == state->end) {
				return _URC_END_OF_STACK;
			}
			else {
				*state->current++ = reinterpret_cast<void*>(pc);
			}
		}
		return _URC_NO_REASON;
	}

}

// Taken from https://stackoverflow.com/a/28858941
size_t captureBacktrace(void** buffer, size_t max)
{
	BacktraceState state = { buffer, buffer + max };
	_Unwind_Backtrace(unwindCallback, &state);

	return state.current - buffer;
}
#include <cxxabi.h> 
void dumpBacktrace(std::ostream& os, void** buffer, size_t count)
{
	os << "Call stack, last function is bottommost:\n";
	size_t outputMemSize = 512;
	char * outputMem = (char *)malloc(outputMemSize);
	
	for (int idx = count - 1; idx >= 0; idx--) {
//	for (size_t idx = 0; idx < count; ++idx) {
		const void* addr = buffer[idx];
		const char* symbol = "";

		Dl_info info;
		if (dladdr(addr, &info) && info.dli_sname) {
			symbol = info.dli_sname;
		}
		memset(outputMem, 0, outputMemSize);
		int status = 0;
		abi::__cxa_demangle(symbol, outputMem, &outputMemSize, &status);
		os << "  #" << std::setw(2) << idx << ": " << addr << "  " << (status == 0 ? outputMem : symbol) << "\n";
	}
	free(outputMem);
}
#include <sstream>
#include <android/log.h>
#include <map>

//int signalCatches[] = { SIGABRT, SIGSEGV, SIGBUS, SIGPIPE };
struct Signal {
	int signalNum;
	const char * signalName;
};
std::vector<Signal> signalCatches = {
	{ SIGABRT, "SIGABRT" },
	{ SIGSEGV, "SIGSEGV" },
	{ SIGBUS, "SIGBUS" },
	{ SIGPIPE, "SIGPIPE" }
};

static void my_handler(const int code, siginfo_t *const si, void *const sc)
{
	static size_t numCatches = 0;
	if (++numCatches > 3) {
		exit(0);
		return;
	}

	auto details = std::find_if(signalCatches.begin(), signalCatches.end(), [=](Signal &s) { return s.signalNum == code; });
	const char * signalName = details == signalCatches.end() ? "Unknown" : details->signalName;
	
	const size_t max = 30;
	void* buffer[max];
	std::ostringstream oss;

	dumpBacktrace(oss, buffer, captureBacktrace(buffer, max));

	__android_log_print(ANDROID_LOG_INFO, "MMFRuntimeNative", "%s", oss.str().c_str());
	LOGE("code raised: %d, %s.", code, signalName);
	raise(SIGTRAP); // SIGTRAP is 
}
static bool didSignals = false;

__attribute__((constructor))
static void prepareSignals()
{
	didSignals = true;

	for (int i = 0; i < signalCatches.size(); i++) {

		struct sigaction sa;
		struct sigaction sa_old;
		memset(&sa, 0, sizeof(sa));
		sigemptyset(&sa.sa_mask);
		sa.sa_sigaction = my_handler;
		sa.sa_flags = SA_SIGINFO;
		if (sigaction(signalCatches[i].signalNum, &sa, &sa_old) != 0) {
			__android_log_print(ANDROID_LOG_WARN, "MMFRuntimeNative", "Failed to set up %s sigaction.", signalCatches[i].signalName);
		}
	}
	__android_log_print(ANDROID_LOG_INFO, "MMFRuntimeNative", "Set up %zu sigactions.", signalCatches.size());
}

extern "C"
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved) {
	// https://developer.android.com/training/articles/perf-jni.html#native_libraries

	if (vm->GetEnv(reinterpret_cast<void**>(&global_env), JNI_VERSION_1_6) != JNI_OK) {
		__android_log_write(ANDROID_LOG_INFO, "MMFRuntimeNative", "GetEnv failed.");
		return -1;
	}
	global_vm = vm;
	LOGV("GetEnv OK, returned %p.", global_env);

	JavaVMAttachArgs args;
	args.version = JNI_VERSION_1_6; // choose your JNI version
	args.name = PROJECT_NAME ", JNI_Load"; // you might want to give the java thread a name
	args.group = NULL; // you might want to assign the java thread to a ThreadGroup
	vm->AttachCurrentThread(&global_env, NULL);

	// Get jclass with global_env->FindClass.
	// Register methods with global_env->RegisterNatives.
	static char projName [] = PROJECT_NAME;
	for (size_t i = 0; i < sizeof(projName) - 1; i++)
		if (projName[i] == ' ')
			projName[i] = '_';
	std::string classNameCRun("Extensions/" "CRun"+ std::string(projName));
	std::string className("Extensions/"+ std::string(projName));
	LOGV("Looking for class %s... [1/2]", classNameCRun.c_str());
	jclass clazz = global_env->FindClass(classNameCRun.c_str());
	if (clazz == NULL) {
		LOGI("Couldn't find %s, now looking for %s... [2/2]", classNameCRun.c_str(), className.c_str());
		if (global_env->ExceptionCheck()) {
			global_env->ExceptionClear();
			LOGV("EXCEPTION [1] %d", 0);
		}
		clazz = global_env->FindClass(className.c_str());
		if (clazz == NULL)
		{
			if (global_env->ExceptionCheck()) {
				global_env->ExceptionClear();
				LOGV("EXCEPTION [2] %d", 0);
			}
			LOGF("Couldn't find class %s. Aborting load of extension.", className.c_str());
			return JNI_VERSION_1_6;
		}
		LOGV("Found %s. [2/2]", className.c_str());
	}
	else
		LOGV("Found %s. [1/2]", classNameCRun.c_str());

#define method(a,b) { "darkedif_" #a, b, (void *)&a }
	//public native long DarkEdif_createRunObject(ByteBuffer edPtr, CCreateObjectInfo cob, int version);
	static JNINativeMethod methods[] = {
		//method(extInit, "()Z"),
		method(getNumberOfConditions, "(J)I"),
		method(createRunObject, "(Ljava/nio/ByteBuffer;LRunLoop/CCreateObjectInfo;I)J"),
		method(destroyRunObject, "(JZ)V"),
		method(handleRunObject, "(J)S"),
		method(displayRunObject, "(J)S"),
		method(pauseRunObject, "(J)S"),
		method(continueRunObject, "(J)S"),
		method(condition, "(JILConditions/CCndExtension;)Z"),
		method(action, "(JILActions/CActExtension;)V"),
		method(expression, "(JILExpressions/CNativeExpInstance;)V"),
	};

	LOGV("Registering natives for %s...", PROJECT_NAME);
	if (global_env->RegisterNatives(clazz, methods, sizeof(methods) / sizeof(methods[0])) < 0) {
		std::string excStr = GetJavaExceptionStr();
		LOGF("Failed to register natives for class %s; error %s.", PROJECT_NAME, excStr.c_str());
	}
	else
		LOGV("Registered natives for class %s successfully.", PROJECT_NAME);
	global_env->DeleteLocalRef(clazz);
	runFuncs.ext = NULL;

	// could be Actions.RunLoop.CRun
	runFuncs.act_getParamExpression = act_getParamExpression;
	runFuncs.act_getParamExpString = act_getParamExpString;
	runFuncs.act_getParamExpFloat = act_getParamExpFloat;

	runFuncs.cnd_getParamExpression = cnd_getParamExpression;
	runFuncs.cnd_getParamExpString = cnd_getParamExpString;
	runFuncs.cnd_getParamExpFloat = cnd_getParamExpFloat;

	runFuncs.exp_getParamInt = exp_getParamExpression;
	runFuncs.exp_getParamString = exp_getParamExpString;
	runFuncs.exp_getParamFloat = exp_getParamExpFloat;

	runFuncs.exp_setReturnInt = exp_setReturnInt;
	runFuncs.exp_setReturnString = exp_setReturnString;
	runFuncs.exp_setReturnFloat = exp_setReturnFloat;

	runFuncs.freeString = freeString;
	
	runFuncs.generateEvent = generateEvent;
	
	if (!didSignals)
		prepareSignals();

	mv * mV = NULL;
	if (!::SDK) {
		__android_log_print(ANDROID_LOG_INFO, "MMFRuntimeNative", "The SDK is being initialised.");
		Edif::Init(mV);
	}
	
	return JNI_VERSION_1_6;
}
extern "C"
JNIEXPORT void JNICALL JNI_OnUnload(JavaVM *vm, void *reserved)
{
}



#endif
