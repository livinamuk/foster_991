#pragma once

	#define TGFEXT	// TGF2, MMF2, MMF2 Dev
// #define MMFEXT		// MMF2, MMF2 Dev
// #define PROEXT	// MMF2 Dev only

#ifdef RUN_ONLY
	#define CurLang (*::SDK->json.u.object.values[::SDK->json.u.object.length - 1].value)
#else
	const extern struct _json_value & CurrentLanguage();
	#define CurLang CurrentLanguage()
#endif
#define JSON_COMMENT_MACRO Extension::Version

#include "DarkEdif.h"

// edPtr : Used at edittime and saved in the MFA/CCN/EXE files

struct EDITDATA
{
	// Header - required
	extHeader		eHeader;

	// Object's data
//	short			swidth;
//	short			sheight;



	// Give menu items a tag (first parameter of a menu item in the
	// JSON) and you'll be able to show/hide them with these functions.
	// This is useful when the properties enable/disable features.

	void ShowMenuItems(const char * Tag);
	void HideMenuItems(const char * Tag);
	
	// Keep as last or risk overwriting by functions accessing this address
	size_t DarkEdif_Prop_Size;
	char DarkEdif_Props[];

	// DarkEdif functions, use within Extension ctor.
	bool IsPropChecked(int propID);
	const char* GetPropertyStr(const char* propName);
	const char* GetPropertyStr(int propID);
};

class Extension;

struct RUNDATA
{
	// Main header - required
	HeaderObject rHo;

	// Optional headers - depend on the OEFLAGS value, see documentation and examples for more info
//	rCom			rc;				// Common structure for movements & animations
//	rMvt			rm;				// Movements
//	rSpr			rs;				// Sprite (displayable objects)
//	rVal			rv;				// Alterable values

	// Required
	Extension * pExtension;

	/*
		You shouldn't add any variables or anything here. Add them as members
		of the Extension class (Extension.h) instead.
	*/
};

#include "Extension.h"
