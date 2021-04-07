#include "Common.h"
extern HINSTANCE hInstLib;
extern Edif::SDK * SDK;

#ifndef RUN_ONLY
static const _json_value * StoredCurrentLanguage = &json_value_none;

static const _json_value * DefaultLanguageIndex()
{
	// Misuse of function; called before ::SDK is valid
	if (!::SDK)
	{
		char msgTitle[128] = {0};
		Edif::GetExtensionName(msgTitle);
		strcat_s(msgTitle, "- DarkEdif error");
		MessageBoxA(NULL, "Premature function call!\n  Called DefaultLanguageIndex() before ::SDK was a valid pointer.", msgTitle, MB_OK);

		return &json_value_none;
	}

	for (unsigned int i = 0; i < SDK->json.u.object.length; ++i)
	{
		if ((*SDK->json.u.object.values[i].value).type == json_object 
			&& (*SDK->json.u.object.values[i].value)["About"]["Name"].type == json_string)
		{
			return SDK->json.u.object.values[i].value;
		}
	}

	// Fallback on last object
	return SDK->json.u.object.values[SDK->json.u.object.length - 1].value;
}

const json_value & CurrentLanguage()
{
	if (StoredCurrentLanguage->type != json_none)
		return *StoredCurrentLanguage;

	// UNIX shouldn't execute in editor mode, so we can ignore langauge and just use whatever.
	// A/C/E details should remain the same regardless of language.
#ifndef _WIN32
	return *DefaultLanguageIndex();
#else

	char FileToLookup [MAX_PATH];
	{
 		GetModuleFileNameA(hInstLib, FileToLookup, sizeof(FileToLookup));

		// This mass of code converts Extensions\Bla.mfx and Extensions\Unicode\Bla.mfx to Extensions\DarkEdif.ini
		char * Filename = FileToLookup + strlen(FileToLookup) - 1;
		while (*Filename != '\\' && *Filename != '/')
			-- Filename;

		// Look in Extensions, not Extensions\Unicode
		if (!_strnicmp("Unicode", Filename - (sizeof("Unicode") - 1), sizeof("Unicode") - 1))
			Filename -= sizeof("Unicode\\") - 1;

		strcpy(++ Filename, "DarkEdif.ini");
		
		// Is the file in the directory of the MFX? (should be, languages are only needed in edittime)
		if (GetFileAttributesA(FileToLookup) == INVALID_FILE_ATTRIBUTES)
		{
			// DarkEdif.ini non-existent
			if (GetLastError() != ERROR_FILE_NOT_FOUND)
				MessageBoxA(NULL, "Error opening DarkEdif.ini.", "DarkEdif SDK - Error", MB_OK);
			
			return *DefaultLanguageIndex();
		}
	}

	// TODO: Change to WinAPI?
	// Open DarkEdif.ini settings file in read binary, and deny other apps writing permissions.
	FILE * F = _fsopen(FileToLookup, "rb", _SH_DENYWR);
	
	// Could not open; abort (should report error)
	if (!F)
		return *DefaultLanguageIndex();

	fseek(F, 0, SEEK_END);
	long S = ftell(F);
	fseek(F, 0, SEEK_SET);

	// Copy file contents into buffer
	char * temp2 = (char *)malloc(S);
	// Out of memory, no buffer allocated	
	if (!temp2)
	{
		fclose(F);
		return *DefaultLanguageIndex();
	}
	
	// Could not read all of the file properly
	if (S != fread_s(temp2, S, sizeof(char), S, F))
	{
		fclose(F);
		free(temp2);
		return *DefaultLanguageIndex();
	}

	// Load entire file into a std::string for searches
	std::string FullFile(temp2, S);
	free(temp2);
	fclose(F);
	size_t Reading;
	
	// Look for two strings (one with space before =)
	if (FullFile.find("Languages=") != std::string::npos)
		Reading = FullFile.find("Languages=")+sizeof("Languages=")-1;
	else
	{
		if (FullFile.find("Languages =") != std::string::npos)
			Reading = FullFile.find("Languages =")+sizeof("Languages =")-1;
		else
		{
			MessageBoxA(NULL, "Languages not found in .ini file.", "DarkEdif Debug CurrentLanguage()", MB_OK);
			return *DefaultLanguageIndex();
		}
	}

	// If there's a space after the =
	if (FullFile[Reading] == ' ')
		++Reading;

	FullFile.append("\r");
	
	// Read string until newline. Expect ";" or "; "-delimited list of languages.
	while (FullFile.find_first_of(";\r", Reading) != std::string::npos) // Is the semi-colon after an end-of-line character?
	{
		// Read individual token
		std::string Language(FullFile.substr(Reading, FullFile.find_first_of(";\r", Reading)-Reading));

		// If languages are separated by "; " not ";"
		if (Language.front() == ' ')
		{
			++Reading;
			Language.erase(Language.begin());
		}

		// Language matched, get index of language in JSON
		for (unsigned int i = 0; i < ::SDK->json.u.object.length; ++i)
		{
			// Return index
			if ((*::SDK->json.u.object.values[i].value).type == json_object && 
				!_stricmp(::SDK->json.u.object.values[i].name, Language.c_str()))
			{
				StoredCurrentLanguage = SDK->json.u.object.values[i].value;
				return *StoredCurrentLanguage;
			}
		}
		Reading += Language.size()+1;
		if (FullFile.at(Reading-1) == '\r')
			break;
	}

	return *DefaultLanguageIndex();
#endif
}
#endif // !RUN_ONLY

inline ACEInfo * ACEInfoAlloc(unsigned int NumParams)
{
	// Allocate space for ACEInfo struct, plus Parameter[NumParams] so it has valid memory
	return (ACEInfo *)calloc(sizeof(ACEInfo) + (NumParams * sizeof(short) * 2), 1);	// The *2 is for reserved variables
}
ExpReturnType ReadExpressionReturnType(const char * Text);

bool CreateNewActionInfo(void)
{
	// Get ID and thus properties by counting currently existing actions.
	const json_value & Action = CurLang["Actions"][::SDK->ActionInfos.size()];
	
	// Invalid JSON reference
	if (Action.type != json_object)
	{
		MessageBoxA(NULL, "Invalid JSON reference, expected object.", "DarkEdif - Error reading action", MB_OK);
		return false;
	}

	const json_value & Param = Action["Parameters"];
	
	// Num of parameters is beyond number of bits in FloatFlags
	if (sizeof(short)*8 < Param.u.object.length)
	{
		MessageBoxA(NULL, "Too many parameters in action.", "DarkEdif - Error reading action", MB_OK);
		return false;
	}

	// Parameters checked; allocate new info
	ACEInfo * ActInfo = ACEInfoAlloc(Param.u.object.length);

	// Could not allocate memory
	if (!ActInfo)
	{
		MessageBoxA(NULL, "Could not allocate memory for action return.", "DarkEdif - Error creating action info", MB_OK);
		return false;
	}

	ActInfo->ID = (short)SDK->ActionInfos.size();
	ActInfo->NumOfParams = Param.u.object.length;

	if (ActInfo->NumOfParams > 0)
	{
		// Set up each parameter
		bool IsFloat;
		for (int c = 0; c < ActInfo->NumOfParams; ++c)
		{
			IsFloat = false;
			ActInfo->Parameter[c].p = ReadParameterType(Param[c][0], IsFloat);	// Store parameter type
			ActInfo->FloatFlags |= (IsFloat << c);								// Store whether it is a float or not with a single bit
		}

		// For some reason in EDIF an extra short is provided, initialised to 0, so duplicate that
		memset(&ActInfo->Parameter[ActInfo->NumOfParams], 0, ActInfo->NumOfParams * sizeof(short));
	}

	// Add to table
	SDK->ActionInfos.push_back(ActInfo);
	return true;
}

bool CreateNewConditionInfo(void)
{
	// Get ID and thus properties by counting currently existing conditions.
	const json_value & Condition = CurLang["Conditions"][::SDK->ConditionInfos.size()];
	
	// Invalid JSON reference
	if (Condition.type != json_object)
	{
		MessageBoxA(NULL, "Invalid JSON reference, expected object.", "DarkEdif - Error reading condition", MB_OK);
		return false;
	}

	const json_value & Param = Condition["Parameters"];
	
	// Num of parameters is beyond size of FloatFlags
	if (sizeof(short)*8 < Param.u.object.length)
	{
		MessageBoxA(NULL, "Too many parameters in condition.", "DarkEdif - Error reading condition", MB_OK);
		return false;
	}

	// Parameters checked; allocate new info
	ACEInfo * CondInfo = ACEInfoAlloc(Param.u.object.length);

	// Could not allocate memory
	if (!CondInfo)
	{
		MessageBoxA(NULL, "Could not allocate memory for condition return.", "DarkEdif - Error creating condition info", MB_OK);
		return false;
	}

	// If a non-triggered condition, set the correct flags
	CondInfo->ID = (short)::SDK->ConditionInfos.size();
	CondInfo->NumOfParams = Param.u.object.length;
#if 0
	CondInfo->Flags.ev = bool (Condition["Triggered"]) ? EVFLAGS::NONE : (EVFLAGS::ALWAYS | EVFLAGS::NOTABLE);
#else
	CondInfo->Flags.ev = bool(Condition["Triggered"]) ? EVFLAGS::NONE : (EVFLAGS)((int)(EVFLAGS::ALWAYS) | ((int)(EVFLAGS::NOTABLE)));
#endif

	if (CondInfo->NumOfParams > 0)
	{
		// Set up each parameter
		bool IsFloat;
		for (int c = 0; c < CondInfo->NumOfParams; ++c)
		{
			IsFloat = false;
			CondInfo->Parameter[c].p = ReadParameterType(Param[c][0], IsFloat);	// Store parameter type
			CondInfo->FloatFlags |= (IsFloat << c);								// Store whether it is a float or not with a single bit
		}

		// For some reason in EDIF an extra short is provided, initialised to 0, so duplicate that
		memset(&CondInfo->Parameter[CondInfo->NumOfParams], 0, CondInfo->NumOfParams * sizeof(short));
	}

	// Add to table
	::SDK->ConditionInfos.push_back(CondInfo);
	return true;
}

bool CreateNewExpressionInfo(void)
{
	// Get ID and thus properties by counting currently existing conditions.
	const json_value & Expression = CurLang["Expressions"][::SDK->ExpressionInfos.size()];
	
	// Invalid JSON reference
	if (Expression.type != json_object)
	{
		MessageBoxA(NULL, "Invalid JSON reference, expected object.", "DarkEdif - Error reading expression", MB_OK);
		return false;
	}

	const json_value & Param = Expression["Parameters"];
	
	// Num of parameters is beyond size of FloatFlags
	if (sizeof(short)*8 < Param.u.object.length)
	{
		MessageBoxA(NULL, "Too many parameters in expression.", "DarkEdif - Error reading expression", MB_OK);
		return false;
	}

	// Parameters checked; allocate new info
	ACEInfo * ExpInfo = ACEInfoAlloc(Param.u.object.length);

	// Could not allocate memory
	if (!ExpInfo)
	{
		MessageBoxA(NULL, "Could not allocate memory for expression return.", "DarkEdif - Error creating expression info", MB_OK);
		return false;
	}

	// If a non-triggered condition, set the correct flags
	ExpInfo->ID = (short)::SDK->ExpressionInfos.size();
	ExpInfo->NumOfParams = Param.u.object.length;
	ExpInfo->Flags.ef = ReadExpressionReturnType(Expression["Returns"]);
	
	if (ExpInfo->NumOfParams > 0)
	{
		// Set up each parameter
		bool IsFloat;
		for (int c = 0; c < ExpInfo->NumOfParams; ++c)
		{
			IsFloat = false;
			ExpInfo->Parameter[c].ep = ReadExpressionParameterType(Param[c][0], IsFloat);	// Store parameter type
			ExpInfo->FloatFlags |= (IsFloat << c);										// Store whether it is a float or not with a single bit
		}

		// For some reason in EDIF an extra short is provided, initialised to 0, so duplicate that
		memset(&ExpInfo->Parameter[ExpInfo->NumOfParams], 0, ExpInfo->NumOfParams * sizeof(short));
	}

	// Add to table
	::SDK->ExpressionInfos.push_back(ExpInfo);
	return true;
}

#ifndef NOPROPS
// DarkEdif - automatic property setup
using namespace Edif::Properties;

#include <sstream>

#ifndef RUN_ONLY
void InitialisePropertiesFromJSON(mv * mV, EDITDATA * edPtr)
{
	std::stringstream mystr;
	size_t chkboxesSize = (size_t)(ceil(CurLang["Properties"].u.array.length / 8.0f));
	char * chkboxes = (char *)calloc(chkboxesSize, 1);
	
	// Set default object settings from DefaultState.
	for (unsigned int i = 0; i < CurLang["Properties"].u.array.length; ++i)
	{
		const json_value & JProp = CurLang["Properties"][i];
		
		// TODO: If default state is missing, say the name of the property for easy repair by dev
		switch (::SDK->EdittimeProperties[i].Type_ID)
		{
			case PROPTYPE_LEFTCHECKBOX:
			{
				if (JProp["DefaultState"].type != json_boolean)
					MessageBoxA(NULL, "Invalid or no default checkbox value specified.", "DarkEdif setup warning", MB_OK);

				if (JProp["DefaultState"])
					chkboxes[i >> 3] |= 1 << (i % 8);

				break;
			}

			case PROPTYPE_COLOR:
			case PROPTYPE_EDIT_NUMBER:
			{
				if (JProp["DefaultState"].type != json_integer)
					MessageBoxA(NULL, "Invalid or no default integer value specified.", "DarkEdif setup warning", MB_OK);
				
				unsigned int i = (unsigned int)(((long long)(JProp["DefaultState"])) & 0xFFFFFFFF);
				mystr.write((char *)&i, sizeof(unsigned int)); // embedded nulls upset the << operator
				
				if (JProp["ChkDefault"])
					chkboxes[i >> 3] |= 1 << (i % 8);

				break;
			}
				
			case PROPTYPE_STATIC:
			case PROPTYPE_FOLDER:
			case PROPTYPE_FOLDER_END:
			case PROPTYPE_EDITBUTTON:
				break; // do not store
			

			case PROPTYPE_EDIT_STRING:
			{
				if (JProp["DefaultState"].type != json_string)
					MessageBoxA(NULL, "Invalid or no default string value specified.", "DarkEdif - setup warning", MB_OK);
				
				// No casing change necessary				
				if (_stricmp(JProp["Case"], "Upper") && _stricmp(JProp["Case"], "Lower"))
					mystr << (const char *)(JProp["DefaultState"]) << char(0);
				else
				{
					std::string dup(JProp["DefaultState"]);
					std::transform(dup.begin(), dup.end(), dup.begin(), !_stricmp(JProp["Case"], "Upper") ? ::toupper : ::tolower);
					mystr << dup << char(0);
				}
				
				if (JProp["ChkDefault"])
					chkboxes[i >> 3] |= 1 << (i % 8);

				break;
			}
				

			case PROPTYPE_COMBOBOX:
			{
				unsigned int i = 0U;
				if (JProp["DefaultState"].type != json_string)
					MessageBoxA(NULL, "Invalid or no default string specified.", "DarkEdif - setup warning", MB_OK);
				else
				{
					for (size_t j = 0; j < JProp["Items"].u.array.length; j++)
					{
						if (!_stricmp((const char *)JProp["DefaultState"], JProp["Items"][j]))
						{
							i = j;
							goto ok;
						}
					}

					MessageBoxA(NULL, "Specified a default string in a combobox property that does not exist in items list.", "DarkEdif - setup warning", MB_OK);
				}
			ok:
				mystr.write((char *)&i, sizeof(unsigned int)); // embedded nulls upset the << operator

				if (JProp["ChkDefault"])
					chkboxes[i >> 3] |= 1 << (i % 8);

				break;
			}

			// These have no ID or property that can be changed
			default:
				break;
		}
	}

	std::string mystr2(chkboxes, chkboxesSize);
	mystr2 += mystr.str();

	free(chkboxes);
	mystr.clear();
	
	edPtr = (EDITDATA *) mvReAllocEditData(mV, edPtr, sizeof(EDITDATA) + mystr2.size());
	if (!edPtr)
	{
		MessageBoxA(NULL, "Could not reallocate EDITDATA.\n\n*cough* MMF2's fault.", "DarkEdif - setup warning", MB_OK);
		return;
	}

	edPtr->DarkEdif_Prop_Size = sizeof(EDITDATA) + mystr2.size();
	
	memset(edPtr->DarkEdif_Props, 0, mystr2.size());
	memcpy(edPtr->DarkEdif_Props, mystr2.data(), mystr2.size());
}

Prop * GetProperty(EDITDATA * edPtr, size_t ID)
{
	// Premature call
	if (edPtr->DarkEdif_Prop_Size == 0)
	{
		char msgTitle [128] = {0};
		Edif::GetExtensionName(msgTitle);
		strcat_s(msgTitle, " - DarkEdif error");
		MessageBoxA(NULL, "Premature function call!\n  GetProperty() called without edPtr->DarkEdif_Props being valid.", msgTitle, MB_OK);
		return nullptr;
	}

	const char * curStr = (const char *)CurLang["Properties"][ID]["Type"]; 

	if (!_stricmp(curStr, "Text") || !_stricmp(curStr, "Edit button"))
	{
#ifndef _UNICODE
		return new Prop_Str((const char *)CurLang["Properties"][ID]["DefaultState"]);
#else
		const char * c = (const char *)CurLang["Properties"][ID]["DefaultState"];
		return new Prop_Str((char *)c);
#endif
	}


	unsigned int size;
	char * Current = PropIndex(edPtr, ID, &size);
	
	Prop * ret = nullptr;

	if (!_stricmp(curStr, "Editbox String"))
		ret = new Prop_Str(Current);
	else if (!_stricmp(curStr, "Editbox Number") || !_stricmp(curStr, "Combo Box"))
		ret = new Prop_UInt(*(unsigned int *)Current);
	else if (_stricmp(curStr, "Checkbox"))
		MessageBoxA(NULL, "Don't understand JSON property type, can't return Prop.", "DarkEdif Fatal Erroz", MB_OK);

	return ret;
}

void PropChangeChkbox(EDITDATA * edPtr, unsigned int PropID, bool newValue)
{
	// The DarkEdif_Props consists of a set of chars, whereby each bit in the char is the "checked"
	// value for the Prop ID specified. Thus each char supports 8 properties.
	int byteIndex = PropID >> 3, bitIndex = PropID % 8;

	if (newValue)
		edPtr->DarkEdif_Props[byteIndex] |= 1 << bitIndex;
	else
		edPtr->DarkEdif_Props[byteIndex] &= ~(1 << bitIndex);
}
void PropChange(mv * mV, EDITDATA * &edPtr, unsigned int PropID, const void * newData, size_t newSize)
{
	unsigned int oldSize; // Set by PropIndex
	const char * curStr = CurLang["Properties"][PropID]["Type"];
	char * oldData = PropIndex(edPtr, PropID, &oldSize);
	bool rearrangementRequired = false;

	if (!_stricmp(curStr, "Editbox String"))
		rearrangementRequired = newSize != oldSize; // May need resizing
	else if (!_stricmp(curStr, "Editbox Number"))
		rearrangementRequired = false; //
	else if (!_stricmp(curStr, "Combo Box"))
		rearrangementRequired = false; // Index of combo box Item
	else if (_stricmp(curStr, "Checkbox"))
		MessageBoxA(NULL, "Don't understand JSON property type, can't return Prop.", "DarkEdif Fatal Error", MB_OK);

	if (!rearrangementRequired)
	{
		memcpy(oldData, newData, newSize);
		return;
	}


	if (oldSize == 0)
		MessageBoxA(NULL, "Editbox String size is 0!", "DarkEdif - Debug info", MB_OK);

	size_t beforeOldSize = oldData - edPtr->DarkEdif_Props; // Pointer to O|<P|O
	size_t afterOldSize = beforeOldSize + oldSize;			// Pointer to O|P>|O

	// Duplicate memory to another buffer (if new arragement is smaller - we can't just copy from old buffer after realloc)
	char * newEdPtr = (char *)malloc(edPtr->DarkEdif_Prop_Size + (newSize - oldSize));

	if (!newEdPtr)
	{
		MessageBoxA(NULL, "Out of memory attempting to rewrite properties!", "DarkEdif - Property Error", MB_OK);
		return;
	}

	char * oldEdPtr = (char *)edPtr;
	EDITDATA * oldEdPtr_ = edPtr;

	// Before data
	memcpy(newEdPtr, oldEdPtr, oldData - oldEdPtr);

	// New data
	memcpy(newEdPtr + (oldData - oldEdPtr), newData, newSize);

	// After data
	memcpy(newEdPtr + (oldData - oldEdPtr) + newSize, 
		oldData + oldSize,
		oldEdPtr_->DarkEdif_Prop_Size - ((oldData - oldEdPtr) + oldSize));
	
	// Reallocate edPtr
	EDITDATA * v = (EDITDATA *)mvReAllocEditData(mV, edPtr, _msize(newEdPtr));
	if (!v)
	{
		MessageBoxA(NULL, "NULL returned from EDITDATA reallocation. Property changed cancelled.", "DarkEdif - Propery Error", MB_OK);
		free(newEdPtr);
		return;
	}

	// Copy into edPtr
	memcpy(v, newEdPtr, _msize(newEdPtr));
	v->DarkEdif_Prop_Size = _msize(newEdPtr);
	free(newEdPtr);
}

#endif // not RUN_ONLY

char * PropIndex(EDITDATA * edPtr, unsigned int ID, unsigned int * size)
{
	char * Current = &edPtr->DarkEdif_Props[(size_t)ceil(CurLang["Properties"].u.array.length / 8.0f)], * StartPos, * EndPos;
	
	json_value j = CurLang["Properties"];

	if (j.type != json_array)
	{
		char msgTitle [128] = {0};
		Edif::GetExtensionName(msgTitle);
		strcat_s(msgTitle, " - DarkEdif error");
		MessageBoxA(NULL, "Premature function call!\n  GetProperty() called without edPtr->DarkEdif_Props being valid.", msgTitle, MB_OK);
		return nullptr;
	}
	
	const char * curStr = (const char *)j[ID]["Type"];
	// Read unchangable properties
	if (!_stricmp(curStr, "Text") || !_stricmp(curStr, "Checkbox"))
		return nullptr;
	// if (curStr == "other stuff")
	//	return new Prop_XXX();

	// Read changable properties
	unsigned int i = 0;
	while (i <= ID)
	{
		curStr = (const char *)j[i]["Type"];
		
		if (!_stricmp(curStr, "Editbox String"))
			Current += (_tcslen((TCHAR *)Current) + 1) * sizeof(TCHAR);
		else if (!_stricmp(curStr, "Editbox Number") || !_stricmp(curStr, "Combo Box"))
			Current += sizeof(unsigned int);

		if (i == ID - 1)
			StartPos = Current;

		++i;
	}
	
	EndPos = Current;

	*size = EndPos - StartPos;
	return StartPos;
}


bool EDITDATA::IsPropChecked(int propID)
{
	return (DarkEdif_Props[propID >> 3] >> (propID % 8) & 1);
}
const char * EDITDATA::GetPropertyStr(const char * propName)
{
	const json_value &props = CurLang["Properties"];
	for (size_t i = 0; i < props.u.array.length; i++)
	{
		if (!_stricmp(props[i]["Title"], propName))
			return GetPropertyStr(i);
	}
	return "Property name not found.";
}
const char * EDITDATA::GetPropertyStr(int propID)
{
	if (propID < 0 || (size_t)propID > CurLang["Properties"].u.array.length)
		return "Property ID not found.";

	const json_value &prop = CurLang["Properties"][propID];
	unsigned int size;
	if (!_stricmp(prop["Type"], "Combo Box"))
		return prop["Items"][*(unsigned int *)PropIndex(this, propID, &size)];
	else if (!_stricmp(prop["Type"], "Editbox String"))
		return PropIndex(this, propID, &size);
	else
		return "Property not textual.";
}

#endif // not NOPROPS

#ifdef __ANDROID__
int MessageBoxA(HWND hwnd, const TCHAR * caption, const TCHAR * text, int iconAndButtons)
{
	__android_log_print(iconAndButtons, "MMFRuntimeNative", "Msg Box swallowed: %s.", text);
	return 0;
}

void DarkEdif::BreakIfDebuggerAttached()
{
	raise(SIGINT);
}
#elif defined(_WIN32)

void DarkEdif::BreakIfDebuggerAttached()
{
	if (IsDebuggerPresent())
		DebugBreak();
}
#else
void DarkEdif::BreakIfDebuggerAttached()
{
	__builtin_trap();
}
#endif

// =====
// Get event number
// =====

/// <summary> Returns currently executing Fusion event. </summary>
DarkEdif::FusionLocation DarkEdif::GetFusionEventLocation(const Extension * const ext)
{
	FusionLocation f;
#ifdef _WIN32
	f.frameNum = ext->rhPtr->App ? (ext->rhPtr->App->nCurrentFrame + 1) : -1;

	// Can we read current event?
	if (!ext->rhPtr->EventGroup)
		return f;

	static int ver = 0;
	if (ver == 0)
		ver = ::SDK->mV->GetVersion() & MMFVERSION_MASK;

	if (ver == CFVERSION_25)
		f.eventNum = ext->rhPtr->EventGroup->evgInhibit;
	else
		f.eventNum = ext->rhPtr->EventGroup->evgIdentifier;
#elif defined(__ANDROID__)
	f.frameNum = -1;
#endif

	if (f.eventNum == 0)
		f.eventNum = -1;
	return f;
}


// =====
// Text conversion - definitions
// =====


#include <assert.h>

#ifdef _WIN32
// For Windows, TString can be Wide or ANSI.
// ANSI function calls are internally converted to Wide by Windows.
// ANSI is not UTF-8, the earliest OS version that *can* use UTF-8 for OS function calls is
// Windows 10 Insider Preview Build 17035, and even that is non-default and in beta.

std::tstring DarkEdif::ANSIToTString(const std::string input) {
	return WideToTString(ANSIToWide(input));
}
std::string DarkEdif::ANSIToUTF8(const std::string input) {
	return WideToUTF8(ANSIToWide(input));
}
std::wstring DarkEdif::ANSIToWide(const std::string input) {
	if (input.empty())
		return std::wstring();

	// First call WideCharToMultiByte() to get output size to reserve
	size_t length = MultiByteToWideChar(CP_ACP, 0, input.c_str(), input.size(), NULL, 0);
	assert(length > 0 && "Failed to convert between string encodings, input string is broken.");
#if _HAS_CXX17
	std::wstring outputStr(length, L'\0');

	// Actually convert
	length = MultiByteToWideChar(CP_ACP, 0, input.c_str(), input.size(), outputStr.data(), outputStr.size());
	assert(length > 0 && "Failed to convert between string encodings.");
#else
	wchar_t * outputBuf = (wchar_t *)_malloca((length + 1) * sizeof(wchar_t));
	// Actually convert
	length = MultiByteToWideChar(CP_ACP, 0, input.c_str(), input.size(), outputBuf, length + 1);
	assert(length > 0 && "Failed to convert between string encodings.");
	std::wstring outputStr(outputBuf, length);
	_freea(outputBuf);
#endif
	if constexpr (DebugConversions)
	{
		OutputDebugStringW(L"Converted ANSI To Wide: [");
		OutputDebugStringA(input.c_str());
		OutputDebugStringW(L"] to [");
		OutputDebugStringW(outputStr.c_str());
		OutputDebugStringW(L"]\n");
		assert(input.back() != '\0' && "Input ends with null.");
		assert(outputStr.back() != L'\0' && "Output ends with null.");
	}

	return outputStr;
}
std::string DarkEdif::UTF8ToANSI(const std::string input, bool * const allValidChars /* = nullptr */) {
	return WideToANSI(UTF8ToWide(input), allValidChars);
}
std::tstring DarkEdif::UTF8ToTString(const std::string input, bool * const allValidChars /* = nullptr */) {
#ifdef _UNICODE
	if (allValidChars)
		*allValidChars = true; // UTF-8 and UTF-16 share all chars
	return UTF8ToWide(input);
#else
	return UTF8ToANSI(input, allValidChars);
#endif
}
std::wstring DarkEdif::UTF8ToWide(const std::string input)
{
	if (input.empty())
		return std::wstring();

	// First call WideCharToMultiByte() to get output size to reserve
	size_t length = MultiByteToWideChar(CP_UTF8, 0, input.c_str(), input.size(), NULL, 0);
	assert(length > 0 && "Failed to convert between string encodings, input string is broken.");
#if _HAS_CXX17
	std::wstring outputStr(length, L'\0');

	// Actually convert
	length = MultiByteToWideChar(CP_UTF8, 0, input.c_str(), input.size(), outputStr.data(), outputStr.size());
	assert(length > 0 && "Failed to convert between string encodings.");
#else
	wchar_t * outputBuf = (wchar_t*)_malloca((length + 1) * sizeof(wchar_t));
	// Actually convert
	length = MultiByteToWideChar(CP_UTF8, 0, input.c_str(), input.size(), outputBuf, length + 1);
	assert(length > 0 && "Failed to convert between string encodings.");
	std::wstring outputStr(outputBuf, length);
	_freea(outputBuf);
#endif

	if constexpr (DebugConversions)
	{
		OutputDebugStringW(L"Converted UTF8 To Wide: [");
		OutputDebugStringA(input.c_str());
		OutputDebugStringW(L"] to [");
		OutputDebugStringW(outputStr.c_str());
		OutputDebugStringW(L"]\n");
		assert(input.back() != '\0' && "Input ends with null.");
		assert(outputStr.back() != L'\0' && "Output ends with null.");
	}
	return outputStr;
}
std::string DarkEdif::WideToANSI(const std::wstring input, bool * const allValidChars /* = nullptr */) {
	if (input.empty())
		return std::string();

	BOOL someFailed;

	// First call WideCharToMultiByte() to get output size to reserve
	size_t length = WideCharToMultiByte(CP_ACP, 0, input.c_str(), input.size(), NULL, 0, 0, allValidChars ? &someFailed : NULL);
	assert(length > 0 && "Failed to convert between string encodings, input string is broken.");

	if (allValidChars)
		*allValidChars = (someFailed == FALSE);

#if _HAS_CXX17
	std::string outputStr(length, '\0');

	// Actually convert
	length = WideCharToMultiByte(CP_ACP, 0, input.c_str(), input.size(), outputStr.data(), outputStr.size(), 0, NULL);
	assert(length > 0 && "Failed to convert between string encodings.");
#else
	char * outputBuf = (char *)_malloca(length + 1);
	// Actually convert
	length = WideCharToMultiByte(CP_ACP, 0, input.c_str(), input.size(), outputBuf, length, 0, 0);
	assert(length > 0 && "Failed to convert between string encodings.");
	std::string outputStr(outputBuf, length);
	_freea(outputBuf);
#endif

	if constexpr (DebugConversions)
	{
		OutputDebugStringW(L"Converted Wide To ANSI: [");
		OutputDebugStringW(input.c_str());
		OutputDebugStringW(L"] to [");
		OutputDebugStringA(outputStr.c_str());
		OutputDebugStringW(L"]\n");
		assert(input.back() != L'\0' && "Input ends with null.");
		assert(outputStr.back() != '\0' && "Output ends with null.");
	}
	return outputStr;
}
std::tstring DarkEdif::WideToTString(const std::wstring input, bool * const allValidChars /* = nullptr */) {
#ifdef _UNICODE
	if (allValidChars)
		*allValidChars = true;
	return input;
#else
	return WideToANSI(input, allValidChars);
#endif
}
std::string DarkEdif::WideToUTF8(const std::wstring input)
{
	if (input.empty())
		return std::string();

	// First call WideCharToMultiByte() to get output size to reserve
	size_t length = WideCharToMultiByte(CP_UTF8, 0, input.c_str(), input.size(), NULL, 0, 0, 0);
	assert(length > 0 && "Failed to convert between string encodings, input string is broken.");
#if _HAS_CXX17
	std::string outputStr(length, '\0');

	// Actually convert
	length = WideCharToMultiByte(CP_UTF8, 0, input.c_str(), input.size(), outputStr.data(), outputStr.size(), 0, 0);
	assert(length > 0 && "Failed to convert between string encodings.");
#else
	char * outputBuf = (char *)_malloca(length + 1);
	// Actually convert
	length = WideCharToMultiByte(CP_UTF8, 0, input.c_str(), input.size(), outputBuf, length, 0, 0);
	assert(length > 0 && "Failed to convert between string encodings.");
	std::string outputStr(outputBuf, length);
	_freea(outputBuf);
#endif

	if constexpr (DebugConversions)
	{
		OutputDebugStringW(L"Converted Wide To UTF8: [");
		OutputDebugStringW(input.c_str());
		OutputDebugStringW(L"] to [");
		OutputDebugStringA(outputStr.c_str());
		OutputDebugStringW(L"]\n");
		assert(input.back() != L'\0' && "Input ends with null.");
		assert(outputStr.back() != '\0' && "Output ends with null.");
	}
	return outputStr;
}
std::string DarkEdif::TStringToANSI(const std::tstring input, bool * const allValidChars /* = nullptr */) {
#ifdef _UNICODE
	return WideToANSI(input, allValidChars);
#else
	if (allValidChars)
		*allValidChars = true;
	return input;
#endif
}
std::string DarkEdif::TStringToUTF8(const std::tstring input) {
#ifdef _UNICODE
	return WideToUTF8(input);
#else
	return ANSIToUTF8(input);
#endif
}
std::wstring DarkEdif::TStringToWide(const std::tstring input) {
#ifdef _UNICODE
	return input;
#else
	return ANSIToWide(input);
#endif
}

#else // !_WIN32

// Linux-based OSes including Android uses UTF-8 by default.
// ANSI and UTF-8 can be considered equivalent.
// Wide-char is barely used at all in Linux, but when it is, it's UTF-32.
// iconv() would be needed, and it's beyond the scope of a regular extension.
// Instead, this code merely returns back.

std::tstring DarkEdif::ANSIToTString(const std::string input) {
	return UTF8ToTString(input);
}
std::string DarkEdif::ANSIToUTF8(const std::string input) {
	return input;
}
std::wstring DarkEdif::ANSIToWide(const std::string input) {
	assert(false && "Linux-based Wide not programmed yet."); return std::wstring();
}
std::string DarkEdif::UTF8ToANSI(const std::string input, bool * const allValidChars /* = nullptr */) {
	return input;
}
std::tstring DarkEdif::UTF8ToTString(const std::string input, bool * const allValidChars /* = nullptr */) {
	return input;
}
std::wstring DarkEdif::UTF8ToWide(const std::string input) {
	assert(false && "Linux-based Wide not programmed yet."); return std::wstring();
}
std::string DarkEdif::WideToANSI(const std::wstring input, bool * const allValidChars /* = nullptr */) {
	assert(false && "Linux-based Wide not programmed yet."); return std::string();
}
std::tstring DarkEdif::WideToTString(const std::wstring input, bool * const allValidChars /* = nullptr */) {
	assert(false && "Linux-based Wide not programmed yet."); return std::tstring();
}
std::string DarkEdif::WideToUTF8(const std::wstring input) {
	assert(false && "Linux-based Wide not programmed yet."); return std::string();
}
std::string DarkEdif::TStringToANSI(const std::tstring input, bool * const allValidChars /* = nullptr */) {
	return TStringToUTF8(input);
}
std::string DarkEdif::TStringToUTF8(const std::tstring input) {
	return input;
}
std::wstring DarkEdif::TStringToWide(const std::tstring input) {
	assert(false && "Linux-based Wide not programmed yet."); return std::wstring();
}

#endif


// Define it
std::tstring DarkEdif::ExtensionName;
DWORD DarkEdif::MainThreadID;
HWND DarkEdif::Internal_WindowHandle;

//
//
//
static int Internal_MessageBox(const TCHAR * titlePrefix, PrintFHint const TCHAR * msgFormat, va_list v, int flags)
{
	static std::tstring titleSuffix = _T(" - ") + DarkEdif::ExtensionName;

	std::tstring title = titlePrefix + titleSuffix;
	TCHAR msgData[4096];
	int numChars = _vstprintf_s(msgData, sizeof(msgData) / sizeof(*msgData), msgFormat, v);
	if (numChars <= 0)
	{
		MessageBox(DarkEdif::Internal_WindowHandle, _T("Failed to format a message box."), title.c_str(), MB_OK | MB_ICONERROR);
		DarkEdif::BreakIfDebuggerAttached();
		return IDCANCEL;
	}
	return MessageBox(DarkEdif::Internal_WindowHandle, msgData, title.c_str(), flags | MB_DEFBUTTON1);
}
void DarkEdif::MsgBox::WarningOK(const TCHAR * titlePrefix, PrintFHint const TCHAR *msgFormat, ...)
{
	va_list v;
	va_start(v, msgFormat);
	Internal_MessageBox(titlePrefix, msgFormat, v, MB_OK | MB_ICONWARNING);
	va_end(v);
}
int DarkEdif::MsgBox::WarningYesNo(const TCHAR * titlePrefix, PrintFHint const TCHAR *msgFormat, ...)
{
	va_list v;
	va_start(v, msgFormat);
	int ret = Internal_MessageBox(titlePrefix, msgFormat, v, MB_YESNO | MB_ICONWARNING);
	va_end(v);
	return ret;
}
int DarkEdif::MsgBox::WarningYesNoCancel(const TCHAR * titlePrefix, PrintFHint const TCHAR *msgFormat, ...)
{
	va_list v;
	va_start(v, msgFormat);
	int ret = Internal_MessageBox(titlePrefix, msgFormat, v, MB_YESNOCANCEL | MB_ICONWARNING);
	va_end(v);
	return ret;
}
void DarkEdif::MsgBox::Error(const TCHAR * titlePrefix, PrintFHint const TCHAR *msgFormat, ...)
{
	va_list v;
	va_start(v, msgFormat);
	Internal_MessageBox(titlePrefix, msgFormat, v, MB_OK | MB_ICONERROR);
	va_end(v);
}
void DarkEdif::MsgBox::Info(const TCHAR * titlePrefix, PrintFHint const TCHAR *msgFormat, ...)
{
	va_list v;
	va_start(v, msgFormat);
	Internal_MessageBox(titlePrefix, msgFormat, v, MB_OK | MB_ICONINFORMATION);
	va_end(v);
}




#ifdef __ANDROID__
// Generated from VS's New Project for Android Dynamic Library.
ProjectFunc jstring getABI(JNIEnv * global_env, jobject javaObj)
{
#if defined(__arm__)
#if defined(__ARM_ARCH_7A__)
#if defined(__ARM_NEON__)
#define ABI "armeabi-v7a/NEON"
#else
#define ABI "armeabi-v7a"
#endif
#else
#define ABI "armeabi"
#endif
#elif defined(__i386__)
#define ABI "x86"
#else
#define ABI "unknown"
#endif
	return global_env->NewStringUTF(ABI);
}

#endif

// Causes the produced extension to include DarkExt.PostMinify.json.
// Hat tip: https://stackoverflow.com/a/4910421
// Also note https://github.com/graphitemaster/incbin/blob/master/incbin.h

#ifdef __ANDROID__
__asm__(".section .rodata				\n\
	.global darkExtJSON					\n\
	.type   darkExtJSON, %object		\n\
	.align  4							\n\
darkExtJSON:							\n\
	.incbin \"DarkExt.PostMinify.json\"	\n\
darkExtJSON_end:						\n\
	.global darkExtJSONSize				\n\
	.type   darkExtJSONSize, %object	\n\
	.align  4							\n\
darkExtJSONSize:						\n\
	.int	darkExtJSON_end - darkExtJSON");
#elif defined(__APPLE__)
// See https://stackoverflow.com/a/19725269
__asm__(".const_data					\n\
	.global darkExtJSON					\n\
	.align  4							\n\
darkExtJSON:							\n\
	.incbin \"DarkExt.PostMinify.json\"	\n\
darkExtJSON_end:						\n\
	.global darkExtJSONSize				\n\
	.align  4							\n\
darkExtJSONSize:						\n\
	.int	darkExtJSON_end - darkExtJSON");
// These are caused by the above ASM block. (these are also declared in the Android master header)
// char darkExtJSON[];
// unsigned darkExtJSONSize;
#endif
