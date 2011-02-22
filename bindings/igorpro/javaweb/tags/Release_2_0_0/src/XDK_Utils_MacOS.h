// ============================================================================
//
// = CONTEXT
//   Xop Dev Kit
//
// = FILENAME
//   XDK_Utils_MacOS.h
//
// = AUTHOR
//   Nicolas Leclercq
//
// ============================================================================
#ifndef _XDK_UTILS_MACOS_H_
#define _XDK_UTILS_MACOS_H_

//========================================================================================
// DEPENDENCIES
//========================================================================================
#include <Lists.h>
#include <TextUtils.h>

#ifdef __cplusplus
  extern "C" {
#endif

//========================================================================================
// ALERT  
//========================================================================================
int XOP_Alert (short AlertType, short IconType, Str255 Str1, Str255 Str2);

//========================================================================================
// ALERT MACROs  
//========================================================================================
#define XOP_STOP_ALERT(_s1, _s2) \
	::XOP_Alert(ALRT_OK, ICN_STOP, ::CtoPStr(_s1),  ::CtoPStr(_s2))	\
	
//========================================================================================
// CUSTOM GET DOUBLE VALUE FROM USER
//========================================================================================
int XOP_GetUserValue (DOUBLE *theValuePtr, Str255 aPStr);

//========================================================================================
// LIST ROUTINES
//========================================================================================
void List_Draw (ListHandle list);
void List_Add (Str255 data, ListHandle list);
void List_SetRow (Str255 data, short index, ListHandle list);
void List_GetRow (Str255 data, short index, ListHandle list);
void List_SetChoice (short  choice, ListHandle list);
Boolean List_GetChoice (short *choice, ListHandle list);
ListHandle List_New (DialogPtr ParentWindow,short Item);
Boolean List_Filter (DialogPtr d,EventRecord *e,ListHandle l,short i,short di,short *ih);

//========================================================================================
// DIALOG ITEMS CUSTOM  ROUTINES (FOR MODELESS DIALOG ONLY ! )
//========================================================================================
void TestDText   (DialogPtr d, short itm);
void TestDDouble (DialogPtr d, short itm);
void TestDInt    (DialogPtr d, short itm);

void DlogItem_SetText   (DialogPtr d, short itm, const char *txt);
void DlogItem_SetShort  (DialogPtr d, short itm, short   val, const char *unit);
void DlogItem_SetLong   (DialogPtr d, short itm, long    val, const char *unit);
void DlogItem_SetDouble (DialogPtr d, short itm, DOUBLE *val, const char *unit);

//========================================================================================
//  CONTROL ITEM STUFFS
//========================================================================================
void ControlItem_Hilite (DialogPtr d, short itm, short ctrl_state);
int  ControlItem_RefCon (DialogPtr d, short itm, long ref_con);
int  ControlItem_Value  (DialogPtr d, short itm, short val);
int  ControlItem_MinMax (DialogPtr d, short itm, short min_val, short max_val);

void ControlItem_UpdateItemRect(DialogPtr d, Rect* itm_rect);

void OutlineButton (DialogPtr d, short item);

//========================================================================================
// KEYBOARD 
//========================================================================================
short OptionKey_Down (void);
short ShiftKey_Down (void);
short ControlKey_Down (void);
short AppleKey_Down (void);
short Arrow_Up (void);
short Arrow_Down (void);
short Arrow_Right (void);
short Arrow_Left (void);

//========================================================================================
//  ROUDING ROUTINES
//========================================================================================
short sRound(DOUBLE theDouble);
long lRound(DOUBLE theDouble);

//========================================================================================
// QD ROUTINES
//========================================================================================
void GlobalRectRgn (RgnHandle destRegion, Rect sourceRect);

//========================================================================================
// MISC.
//========================================================================================
Boolean XOP_LowMemory (void);
int XOP_GetWavesFromHost (char *theList);

//========================================================================================
// C to Pascal string convertion
//========================================================================================
unsigned char *CtoPStr(const char *_str);

#ifdef __cplusplus
  }
#endif

#endif // _XDK_UTILS_MACOS_H_


