// ============================================================================
//
// = CONTEXT
//   Xop Dev Kit
//
// = FILENAME
//   XDK_Utils_MacOS.cpp
//
// = AUTHOR
//   Nicolas Leclercq
//
// ============================================================================

//==============================================================================
// DEPENDENCIES
//==============================================================================
#include <stdio.h>
#include <ctype.h>
#include <Timer.h>
#include "XDK_StandardHeaders.h"
#include "XDK_Utils.h"
#include "XDK_Utils_MacOS.h"

//----------------------------------------------------------------------------------------
// #DEFINEs                                                 
//---------------------------------------------------------------------------------------- 
#define OK_BUT                1
#define N0_CANCEL_BUT         2
#define CANCEL_BUT            3
#define TXT                   4

#define USER_VALUE_OK_BUT     1
#define USER_VALUE_CANCEL_BUT 2
#define USER_VALUE_EDIT_TXT   3
#define USER_VALUE_STAT_TXT   4

//========================================================================================
// LOCAL PROTOTYPES
//========================================================================================
#pragma mark @ Local Prototypes

static Rect GetDRect (DialogPtr theDialog, short itemNr);
static Handle ControlItem_Handle (short	itemNr);
static Boolean ControlItem_CanHilite (short item_type);
static int XOP_GetWaveListFromHost (char *theList , short *numWaveResult);

static DOUBLE usec_to_double (register const UnsignedWide *epochPtr);

static void usec_elapsed (register const UnsignedWide	*start,
								          register const UnsignedWide	*end,
								          register       UnsignedWide	*result);
								          

//========================================================================================
// LOCAL GLOBALS
//========================================================================================	
#define kMIN_TIMER 0
#define kMAX_TIMER 9			
static const DOUBLE k2Power32 = 4294967296.0;		          
static UnsignedWide UWide_MicroSecCountersStartTime[kMAX_TIMER + 1];
								  							      
#pragma mark -

//========================================================================================
// LIST ROUTINES
//========================================================================================
//-----------------------------------------------------------------------------------------
// Routine : List_New (1 column list)
//-----------------------------------------------------------------------------------------
ListHandle List_New (DialogPtr ParentWindow,short Item)
{
    Rect			item_rect;
	Rect			listBounds;
	Rect			dataBounds;
	Point			cSize;
	ListHandle		list;
	

	item_rect   = GetDRect (ParentWindow,Item);
	listBounds = item_rect;
	listBounds.right = listBounds.right - 15;	/*for scroll bar*/
	SetRect (&dataBounds, 0, 0, 1, 0);	        /*one column, no rows*/
	SetPt (&cSize, listBounds.right - listBounds.left, 0);
	list = LNew	(&listBounds,	/*dialog item's box*/
				 &dataBounds,	/*one column, no rows*/
				 cSize,			/*cell size: full width, standard height*/
				 0,				/*procid - standard text list*/
				 ParentWindow,	/*parent window*/
				 false,			/*draw it*/
				 false,			/*has no grow*/
				 false,			/*no horizontal scroll*/
				 true);			/*vertical scroll*/
	(**list).selFlags = lOnlyOne + lNoNilHilite;
	return (list);
}

//------------------------------------------------------------------------------------
// Routine : List_GetChoice
//------------------------------------------------------------------------------------
Boolean	List_GetChoice (short *choice,ListHandle list)
{
	Boolean			result;
	Cell			selectedCell;

	SetPt (&selectedCell, 0, 0);
	if (LGetSelect (true, &selectedCell, list)) {
		*choice = selectedCell.v;
		result = true;
	} else {
		List_SetChoice (*choice,list);
		result = false;
	}
	return (result);
} 

//------------------------------------------------------------------------------------
// Routine : List_SetChoice
//------------------------------------------------------------------------------------
void List_SetChoice (short choice,ListHandle list)
{
	Cell			selectedCell;

	SetPt (&selectedCell, 0, choice);
	LSetSelect (true, selectedCell, list);
} 

//------------------------------------------------------------------------------------
// Routine : List_GetRow
//------------------------------------------------------------------------------------
void  List_GetRow (Str255 data,short index,ListHandle list)
{
	Cell			selectedCell;
	short			dataLen;

	SetPt (&selectedCell, 0, index);
	dataLen = 255;		/*var parameter to LGetCell*/
	LGetCell (&data [1], &dataLen, selectedCell, list);
	data [0] = (char) dataLen;
}

//------------------------------------------------------------------------------------
// Routine : List_SetRow
//------------------------------------------------------------------------------------
void	List_SetRow (Str255 data,short index,ListHandle list)
{
	Cell			selectedCell;

	SetPt (&selectedCell, 0, index);
	LSetCell (&data [1], data [0], selectedCell, list);
} /*SetListRow*/

//------------------------------------------------------------------------------------
// Routine : List_Add
//------------------------------------------------------------------------------------
void List_Add (Str255 data,ListHandle list)
{
	short			newRow;
	
	#define MAX_INT	32767

	newRow = LAddRow (1,MAX_INT,list);
	List_SetRow (data, newRow, list);
	
	#undef MAX_INT
} 

//------------------------------------------------------------------------------------
// Routine : List_Draw
//------------------------------------------------------------------------------------
void List_Draw (ListHandle list)
{
	PenState		savePen;
	Rect			frame;

	GetPenState (&savePen);
	PenNormal ();
	frame = (**list).rView;
	InsetRect (&frame, -1, -1);
	FrameRect (&frame);
	SetPenState (&savePen);
	LUpdate ((**list).port->visRgn, list);
} 

//------------------------------------------------------------------------------------
// Routine : List_Filter
//------------------------------------------------------------------------------------
Boolean List_Filter	(DialogPtr theDialog,EventRecord *event,ListHandle list,
                     short listItem,short dblClickItem,short *itemHit)
{
	Boolean filtered;
	Point   mousePos;

	filtered = false;
	if (event->what == mouseDown) 
	{
		mousePos = event->where;
		GlobalToLocal (&mousePos);
		if ( FindDialogItem (theDialog, mousePos) + 1 == listItem )
		{
			if (LClick (mousePos, event->modifiers, list)) {
				*itemHit = dblClickItem;
			}
			else {
				*itemHit = listItem;
			}
			filtered = true;
		}
	}
	return filtered;
}

#pragma mark -

//========================================================================================
//  SPECIAL TOOLS AND OTHER STRANGE STUFFs
//========================================================================================
// ---------------------------------------------------------------------------------------
// Routine : ControlItem_Handle
// ---------------------------------------------------------------------------------------
static Handle ControlItem_Handle (short itemNr)
{
	short item_type;
	Handle item_handle;
	Rect item_rect;

	GetDialogItem (QDPointer()->thePort, itemNr, &item_type, &item_handle, &item_rect);

	return (item_handle);
} 

//----------------------------------------------------------------------------------------
// Routine :  ControlItem_CanHilite
//----------------------------------------------------------------------------------------

static Boolean  ControlItem_CanHilite(short item_type)
{

  if  (    ( item_type == (btnCtrl+ctrlItem) )
        || ( item_type == (chkCtrl+ctrlItem) )
        || ( item_type == (radCtrl+ctrlItem) )
        || ( item_type == (resCtrl+ctrlItem) ) 
      )
      return(true); 
        
  return(false);
  
}

//------------------------------------------------------------------------------------
// Routine :  ControlItem_Hilite
//------------------------------------------------------------------------------------ 
void ControlItem_Hilite(DialogPtr theDialog, short theItem, short ctrlsState)
{
  short  item_type; 
  Handle item_handle;
  Rect	 item_rect;
     
  GetDialogItem (theDialog,theItem, &item_type, &item_handle, &item_rect);
  if (item_handle && ControlItem_CanHilite(item_type)) 
    HiliteControl((ControlHandle)item_handle,ctrlsState);

}
	 
//------------------------------------------------------------------------------------
// Routine : ControlItem_RefCon
//------------------------------------------------------------------------------------ 
int ControlItem_RefCon(DialogPtr _dlog, short _item, long _val)
{
  short   item_type;
  Handle  item_handle;
  Rect    item_rect;

	::GetDialogItem(_dlog, _item, &item_type,&item_handle, &item_rect);
	if (item_handle == 0) 
	  return kError;
  	  
	::SetControlReference((ControlHandle)item_handle, _val);
	
	return kNoError;
}

//------------------------------------------------------------------------------------
// Routine : ControlItem_Value
//------------------------------------------------------------------------------------ 
int ControlItem_Value(DialogPtr theWindow,short theItem,short theValue)
{

 short          item_type; 
 Handle	        item_handle;
 Rect	        item_rect;

 GetDialogItem (theWindow,theItem,&item_type,&item_handle,&item_rect);
 if ( item_handle == NIL ) return kError;
 SetControlValue((ControlHandle)item_handle,theValue);
 
 return kNoError;
 
}

//------------------------------------------------------------------------------------
// Routine : ControlItem_MinMax
//------------------------------------------------------------------------------------ 
int ControlItem_MinMax(DialogPtr theWindow, short theItem, short min, short max)
{
 short item_type; 
 Handle item_handle;
 Rect item_rect;

 GetDialogItem(theWindow, theItem, &item_type, &item_handle, &item_rect);
 if (item_handle == 0) return kError;
 SetControlMinimum((ControlHandle)item_handle, min);
 SetControlMaximum((ControlHandle)item_handle, max);
 return kNoError;
}

// ----------------------------------------------------------------------------------------
// Routine : ControlItem_UpdateItemRect
// ----------------------------------------------------------------------------------------
void ControlItem_UpdateItemRect(DialogPtr dlog, Rect* item_rect)
{ 
  RgnHandle aRgn;
  
  aRgn = NewRgn();
  if (aRgn == NIL) return;
  RectRgn(aRgn, item_rect);
  UpdateDialog(dlog, aRgn);
  if (aRgn) DisposeRgn(aRgn);
}

#pragma mark -
//========================================================================================
//  ROUDING ROUTINES
//========================================================================================

short sRound(DOUBLE theDouble)
{
	double_t integer,fraction;

	fraction = modf(theDouble, &integer );
	if (fraction > 0.5) integer++;
	return((short)integer);
}

long lRound(DOUBLE theDouble)
{
	double_t L_integer,fraction;

	fraction = modf(theDouble, &L_integer );
	if (fraction > 0.5) L_integer++;
	return(roundtol(L_integer));
}

#pragma mark -

//========================================================================================
//  DIALOG EDIT TEXT ITEM ROUTINES
//========================================================================================
//========================================================================================
// EDIT TEXT ROUTINES
//========================================================================================
// ---------------------------------------------------------------------------------------
// Routine : Delete
// ---------------------------------------------------------------------------------------
static void  Delete(Str255 dest, short index, short count)
{
  short   j;
  for (j = index; j<= dest[0] - count; j++ )
  {
    dest[j]=dest[j+count];
  }
  dest[0] -= count;
}

// ----------------------------------------------------------------------------------------
// Routine : TestDText
// ----------------------------------------------------------------------------------------
void  TestDText (DialogPtr dlog,short item)
 {
   Handle       item_handle;
   short        item_type, i, j, l;
   Rect         item_rect;
   Str255       s;

  GetDialogItem(dlog, item, &item_type, &item_handle, &item_rect);
  GetDialogItemText(item_handle, s);
  l = s[0];
  if ((s[1] == '-') || (s[1] == '+'))
   j = 2;                               
  else
   j = 1;
  for ( i = j; i<= l; i++ )
  {
    if ( s[i] == ',' )
    {
      s[i] = '.';
      SetDialogItemText(item_handle, s);
      ControlItem_UpdateItemRect(dlog,&item_rect);
    }
    if ( ( isdigit(s[i]) == 0 ) && ( s[i] != '.' ))
    {
      Delete(s, i, 1);           
      i = l + 1;                       
      SetDialogItemText(item_handle, s);
       ControlItem_UpdateItemRect(dlog,&item_rect);
    }
  }
}
  
// ----------------------------------------------------------------------------------------
// Routine : TestDDouble
// ----------------------------------------------------------------------------------------
void  TestDDouble (DialogPtr dlog,short item)
 {
   Handle       item_handle;
   short        item_type, i, j, l;
   Rect         item_rect;
   Str255       s;

  GetDialogItem(dlog, item, &item_type, &item_handle, &item_rect);
  GetDialogItemText(item_handle, s);
  l = s[0];
  j = 1;
  for ( i = j; i<= l; i++ )
  {
    if ( s[i] == ',' )
    {
      s[i] = '.';
      SetDialogItemText(item_handle, s);
      ControlItem_UpdateItemRect(dlog,&item_rect);
    }
    if ( s[i] == 'e' )
    {
      s[i] = 'E';
      SetDialogItemText(item_handle, s);
      ControlItem_UpdateItemRect(dlog,&item_rect);
    }
    if (      (isdigit(s[i]) == 0) 
          &&  (s[i] != 'E') 
          &&  (s[i] != '.') 
          &&  (s[i] != '-') 
          &&  (s[1] != '+')
       )
    {
      Delete(s, i, 1);           
      i = l + 1;                       
      SetDialogItemText(item_handle, s);
      ControlItem_UpdateItemRect(dlog,&item_rect);
    }
  }
}

// ----------------------------------------------------------------------------------------
// Routine : TestDInt
// ----------------------------------------------------------------------------------------
void  TestDInt (DialogPtr dlog,short item)
 {
   Handle       item_handle;
   short        item_type, i, j, l;
   Rect         item_rect;
   Str255       s;

   GetDialogItem(dlog, item, &item_type, &item_handle, &item_rect);
   GetDialogItemText(item_handle, s);
   l = s[0];
   if ((s[1] == '-') || (s[1] == '+'))
    j = 2;                               
   else
    j = 1;
   for ( i = j; i<= l; i++ )
   {
     if ( isdigit(s[i]) == 0 )
     {
       Delete(s, i, 1);          
       i = l + 1;                       
       SetDialogItemText(item_handle, s);
       ControlItem_UpdateItemRect(dlog,&item_rect);
     }
   }
}

#pragma mark -

//-----------------------------------------------------------------------------
// Routine : DlogItem_SetText
// ----------------------------------------------------------------------------
void DlogItem_SetText(DialogPtr theDialog, short itemNr, const char *theText)
{
    short		type;
	Handle		item;
	Rect		box;
	
	GetDialogItem(theDialog,itemNr, &type, &item, &box);
  SetDText(theDialog,itemNr,theText);
	ControlItem_UpdateItemRect(theDialog, &box);
}
//-----------------------------------------------------------------------------
// Routine :DlogItem_SetShort
// ----------------------------------------------------------------------------
void DlogItem_SetShort(DialogPtr theDialog, short itemNr,short theValue,const char *Unit)
{
  char    Txt[256];

  if ( Unit == NIL)
     sprintf(Txt,"%d\0",theValue);
  else 
     sprintf(Txt,"%d %.3s\0",theValue,Unit);
     
  DlogItem_SetText(theDialog,itemNr,Txt);
}

//-----------------------------------------------------------------------------
// Routine : DlogItem_SetLong
// ----------------------------------------------------------------------------
void DlogItem_SetLong(DialogPtr theDialog, short itemNr,long theValue, const char *Unit)
{
  char    Txt[256];

  if ( Unit == NIL)
     sprintf(Txt,"%ld\0",theValue);
  else 
     sprintf(Txt,"%ld %.3s\0",theValue,Unit);
     
  DlogItem_SetText(theDialog,itemNr,Txt);

 }

//-----------------------------------------------------------------------------
// Routine : DlogItem_SetDouble
// ----------------------------------------------------------------------------
void DlogItem_SetDouble(DialogPtr theDialog, short itemNr,DOUBLE *theValue,const char *Unit)
{
  char    Txt[256];

  if ( Unit == NIL)
     sprintf(Txt,"%.2lf\0",*theValue);
  else 
     sprintf(Txt,"%.2lf %s\0",*theValue,Unit); 
  DlogItem_SetText(theDialog,itemNr,Txt);
 }
 
 #pragma mark -

//========================================================================================
// CHECK KEYBOARD ROUTINES
//========================================================================================
// ----------------------------------------------------------------------------------------
// Routine : Arrow_Up
// ----------------------------------------------------------------------------------------
short Arrow_Up (void)
{
  unsigned char km[16];
  short k = 0x7E;

  GetKeys((unsigned long*)km);
  return ( ( km[k>>3] >> (k & 7) ) & 1);
}
// ---------------------------------------------------------------------------------------
// Routine : Arrow_Down
// ---------------------------------------------------------------------------------------
short Arrow_Down(void)
{
  unsigned char km[16];
  short k = 0x7D;

  GetKeys((unsigned long*)km);
  return ( ( km[k>>3] >> (k & 7) ) & 1);
}
// ---------------------------------------------------------------------------------------
// Routine : Arrow_Right
// ---------------------------------------------------------------------------------------
short Arrow_Right(void)
{
  unsigned char km[16];
  short k = 0x7C;

  GetKeys((unsigned long*)km);
  return ( ( km[k>>3] >> (k & 7) ) & 1);
}
// ---------------------------------------------------------------------------------------
// Routine : Arrow_Left
// ---------------------------------------------------------------------------------------
short Arrow_Left(void)
{
  unsigned char km[16];
  short k = 0x7B;

  GetKeys((unsigned long*)km);
  return ( ( km[k>>3] >> (k & 7) ) & 1);
}
// ---------------------------------------------------------------------------------------
// Routine : OptionKey_Down
// ---------------------------------------------------------------------------------------
short OptionKey_Down(void)
{
  unsigned char km[16];
  short         k = 0x3A;

  GetKeys((unsigned long*)km);
  return ( ( km[k>>3] >> (k & 7) ) & 1);
}
// ---------------------------------------------------------------------------------------
// Routine : ShiftKey_Down
// ---------------------------------------------------------------------------------------
short ShiftKey_Down(void)
{
  unsigned char km[16];
  short       k = 0x38;

  GetKeys((unsigned long*)km);
  return ( ( km[k>>3] >> (k & 7) ) & 1);
}
// ---------------------------------------------------------------------------------------
// Routine : ControlKey_Down
// ---------------------------------------------------------------------------------------
short ControlKey_Down(void)
{
  unsigned char km[16];
  short       k = 0x3B;

  GetKeys((unsigned long*)km);
  return ( ( km[k>>3] >> (k & 7) ) & 1);
}
// ---------------------------------------------------------------------------------------
// Routine : AppleKey_Down
// ---------------------------------------------------------------------------------------
short AppleKey_Down(void)
{
  unsigned char km[16];
  short       k = 0x37;

  GetKeys((unsigned long*)km);
  return ( ( km[k>>3] >> (k & 7) ) & 1);
}

#pragma mark -

//========================================================================================
// CUSTOM ALERT ROUTINE(S)
//========================================================================================
// ---------------------------------------------------------------------------------------
// Routine : XOP_Alert
// ---------------------------------------------------------------------------------------
int XOP_Alert(short AlertType, short IconType, Str255 Str1, Str255 Str2)
{
    Boolean    done=false;
    short      itemHit;
    DialogPtr  theAlert;
    GrafPtr    LastPort ;
    short      userAnswer=0;

    SetCursor(&QDPointer()->arrow);
     
   	GetPort(&LastPort);
   	
   	theAlert = GetNewDialog(AlertType,NIL,(WindowPtr)-1);
	  if (theAlert==NIL) return(0);
	  SetPort(theAlert);
	
	  ShowWindow (theAlert);
    ParamText(Str1,Str2,NIL,NIL);
    ShowDialogItem(theAlert,IconType);
    OutlineButton(theAlert,OK_BUT);
    
    while (!userAnswer) {
  	  XOPDialog((ModalFilterProcPtr)NIL,&itemHit);
  	  switch ( itemHit ) {
  	    case OK_BUT : 
  	         switch (AlertType) {
  	            case ALRT_YES_NO        :
                  case ALRT_YES_NO_CANCEL :
                       userAnswer = kUSER_CLICKED_YES;
  	                 break;
  	            case ALRT_OK            :
  	            case ALRT_OK_CANCEL     :
                       userAnswer = kUSER_CLICKED_OK;
  	                 break;	
  	         } 
  	         break;
  	    case N0_CANCEL_BUT : 
  	         switch (AlertType) {
  	            case ALRT_YES_NO        :
                  case ALRT_YES_NO_CANCEL :
                       userAnswer = kUSER_CLICKED_NO;
  	                 break;
  	            case ALRT_OK            :
  	            case ALRT_OK_CANCEL     :
                       userAnswer = kUSER_CLICKED_CANCEL;
  	                 break;	
  	         } 
  	         break;   
  	    case CANCEL_BUT : 
  	         switch (AlertType) {
                  case ALRT_YES_NO_CANCEL :    
  	            case ALRT_OK_CANCEL     :
                       userAnswer = kUSER_CLICKED_CANCEL;
  	                 break;	
  	         } 
  	         break; 
  	  }
  	  //::YieldToAnyThread();
    }
   
    DisposeDialog(theAlert);
    SetPort(LastPort);  // MUST BE RESTORED
    return(userAnswer);
}

#pragma mark -

//========================================================================================
// CUSTOM GET INT/DOUBLE/... ROUTINE(S)
//========================================================================================
// ---------------------------------------------------------------------------------------
// Routine : XOP_GetUserValue
// ---------------------------------------------------------------------------------------
int XOP_GetUserValue(DOUBLE *theValuePtr,Str255 aPStr)
{
    Boolean    done=false;
    Boolean    aborted=false;
    short      itemHit;
    DialogPtr  theDialog;
    GrafPtr    LastPort ;
    short      userAnswer=0;
    DOUBLE     aDouble;
    
    aDouble = *theValuePtr;
    
    SetCursor(&QDPointer()->arrow);
     
   	GetPort(&LastPort);
   	
   	theDialog = GetNewDialog(USER_VALUE_DLOG_ID,NIL,(WindowPtr)-1);
	  if (theDialog == 0) 
      return kRSRC_MISSING_ERROR;
	  SetPort(theDialog);
	
	  ParamText(aPStr,NIL,NIL,NIL);
    SetDDouble(theDialog,USER_VALUE_EDIT_TXT,&aDouble);
	  ShowWindow(theDialog);

    //¥¥¥OutlineButton(theDialog,USER_VALUE_OK_BUT);
    
    while (!done) {
	  XOPDialog((ModalFilterProcPtr)NIL,&itemHit);
	  switch ( itemHit ) {
	    case USER_VALUE_OK_BUT   : 
	         done    = true;
	         aborted = false;
	         break;
	    case USER_VALUE_CANCEL_BUT   : 
	         done    = true;
	         aborted = true;
	         break;
	    case USER_VALUE_EDIT_TXT : 
	         TestDDouble(theDialog,USER_VALUE_EDIT_TXT);
	         GetDDouble(theDialog,USER_VALUE_EDIT_TXT,&aDouble);
	         break;
	  }
    }
    
    if ( !aborted ) 
    	 *theValuePtr = aDouble;
    	
    DisposeDialog(theDialog);
    SetPort(LastPort);  
    return kNoError;
}

#pragma mark -

// ---------------------------------------------------------------------------------------
// Routine : GetDRect
// ---------------------------------------------------------------------------------------
static Rect  GetDRect (DialogPtr theDialog, short itemNr)
{
	short		item_type;
	Handle		item_handle;
	Rect		item_rect;

	GetDialogItem (theDialog,itemNr,&item_type,&item_handle,&item_rect);
	return (item_rect);
} 

//----------------------------------------------------------------------------------------
// Routine : GlobalRectRgn
//----------------------------------------------------------------------------------------  
void GlobalRectRgn (RgnHandle destRegion, Rect sourceRect)
{
  LocalToGlobal (&topLeft (sourceRect)); 
  LocalToGlobal (&botRight (sourceRect));
  RectRgn (destRegion, &sourceRect);
} 

#pragma mark -

//------------------------------------------------------------------------------------
// Routine :XOP_LowMemory
//------------------------------------------------------------------------------------
Boolean XOP_LowMemory ( void )
{
  static Boolean        LowMemory   = false;
  static unsigned long  lastTicks   = 0L;  
                  long  CurrentMaxBlock;
                  
  CurrentMaxBlock = MaxBlock() / 1000L;
  
  return (CurrentMaxBlock <= MEMORY_WARNING_LIMIT) ? true : false;

}

#pragma mark -

//------------------------------------------------------------------------------------
// Routine : XOP_GetWavesFromHost
//------------------------------------------------------------------------------------ 
int XOP_GetWavesFromHost (char *theList)
{
  short     userAnswer;
  short     numWaveResult = 0;
   
  if (XOP_GetWaveListFromHost(theList,&numWaveResult)) {
    userAnswer = XOP_Alert(ALRT_OK,ICN_STOP,"\pCan't get waves from Igor ...","\p");
    return kError;
  }
  else {
    if (! numWaveResult) 
      return kError;
  }
  
  return kNoError;    
}

//------------------------------------------------------------------------------------
// Routine : XOP_GetWaveListFromHost
// NOTE : the caller must dispose of the returned Ptr (theListStr).
//------------------------------------------------------------------------------------ 
static int XOP_GetWaveListFromHost (char *theList, short *numWaveResult)
{  
   Handle    listHandle = 0;
   Size      listLen;
   int       result;
   char      *delims = ",";
   short     numWave,i;

   listHandle = NewHandle(0L);
   if (listHandle == 0) 
      return kError;
   
   result = WaveList(listHandle, "*", ",", "");
   if (result != kNoError) {
      Notify("XOP MSG : XOP_GetWaveListFromHost:WaveList:gave error.");
      DisposeHandle(listHandle);
      *numWaveResult = 0;
      return kError;
   }
   
   listLen = GetHandleSize(listHandle);
   
   if (listLen > 0) {
    #ifdef  DEBUG_MODE   
      Notify("XOP MSG : SetPtrSize to :: %ld.sizeof(char)", listLen + 1);
    #endif
    SetPtrSize((Ptr)theList, listLen + 1);
    if (MemError()) {
      if (listHandle) 
        DisposeHandle(listHandle);
      Notify("XOP MSG : XOP_GetWaveListFromHost : SetPtrSize : Out of memory.");
      *numWaveResult = 0;
      return kError;
    }
   }
   else {
    if (listHandle) 
      DisposeHandle(listHandle);
    #ifdef  DEBUG_MODE   
      Notify("XOP MSG : No waves in Igor's memory.");
    #endif
    *numWaveResult = 0;
    return kNoError;
   }
     
   HLock(listHandle);
    BlockMove(*listHandle, theList, listLen);
   HUnlock(listHandle);
   theList[listLen] = 0;
   DisposeHandle(listHandle);
   listHandle = 0;
   
   #ifdef  DEBUG_MODE   
   		Notify("theLocalList : %s", theList);
   #endif
   
   // count number of waves in list
   // number of waves = num of ","
   numWave = 0;
   for (i = 0; i < listLen; i++) {
      if (theList[i] == ',') 
        numWave++;
   }
   
   if (numWave <= 0)  { 
      #ifdef  DEBUG_MODE   
        Notify("XOP MSG : No waves in Igor's memory.");
      #endif
      *numWaveResult = 0;
      return kNoError;
   }
   
   #ifdef  DEBUG_MODE
      Notify("XOP MSG : numWave in Igor's memory : %d.",numWave);
   #endif

   *numWaveResult = numWave;

   return kNoError;
}


/*
 *	Frame or unframe a default dialog item (presumed a button) in given dialog.
 *	Note that you don't need to use an extra user item to do this unless you
 *	are doing some sort of non-standard default highlighting (not recommended).
 */
#pragma mark -

// ----------------------------------------------------------------------------------------
// Routine : OutlineButton
// ----------------------------------------------------------------------------------------
void OutlineButton (DialogPtr theDialog, short itemNr)
{
	Rect			itemRect;
	PenState		savePen;
   
	GetPenState (&savePen);
	PenNormal ();
	PenSize (3, 3);
	itemRect = GetDRect(theDialog,itemNr);
	InsetRect (&itemRect, -4, -4);
	FrameRoundRect (&itemRect, 16, 16);
	SetPenState (&savePen);
}

#pragma mark -

//========================================================================================
// C to Pascal string convertion
//========================================================================================
unsigned char* CtoPStr(const char *_str) 
{ 
	return c2pstr((char*)_str);
}

#pragma mark -

//------------------------------------------------------------------------------
// start_timer
//------------------------------------------------------------------------------
void start_timer (int _timer_id)
{
  if (_timer_id < kMIN_TIMER  || _timer_id  > kMAX_TIMER) 
    return;
    
  UnsignedWide UWide_StartTime;
  ::Microseconds(&UWide_StartTime);
  UWide_MicroSecCountersStartTime[_timer_id] = UWide_StartTime;
}

//------------------------------------------------------------------------------
// elapsed_usec
//------------------------------------------------------------------------------
DOUBLE elapsed_usec (int _timer_id)
{
  UnsignedWide UWide_StopTime;  
  ::Microseconds(&UWide_StopTime); 
  
  if (_timer_id < kMIN_TIMER  || _timer_id  > kMAX_TIMER) 
    return 0.;
  
  UnsignedWide UWide_StartTime;  
  UWide_StartTime = UWide_MicroSecCountersStartTime[_timer_id];
  
  UnsignedWide   UWide_ElapsedTime;
  ::usec_elapsed(&UWide_StartTime, &UWide_StopTime, &UWide_ElapsedTime);
   
  return ::usec_to_double(&UWide_ElapsedTime);
}

//------------------------------------------------------------------------------
// elapsed_sec
//------------------------------------------------------------------------------
DOUBLE elapsed_sec (int _timer_id)
{
  return elapsed_usec(_timer_id) * 1.E-6;
}

//------------------------------------------------------------------------------------
// Routine : usec_to_double
// Convert an epoch to microseconds (use floating-point operations).
// Note that 4294967296.0 is 2^32, which is represented accurately
// in double-precision floating point. Note that this can loose
// low-order bits. Returns the result in µs
//------------------------------------------------------------------------------------
static DOUBLE usec_to_double (register const UnsignedWide	*_uw)
{
  register DOUBLE result;
  result = (DOUBLE)_uw->hi * k2Power32 + _uw->lo;
  return result;
}

//------------------------------------------------------------------------------------
// Routine : usec_elapsed
// Return the difference between two Microsecond Trap values.
// Integer subtraction is used to preserve accuracy.
//------------------------------------------------------------------------------------
static void usec_elapsed (register const UnsignedWide	*_start,
                          register const UnsignedWide	*_end,
		                      register UnsignedWide *result_)
{
		if (_end->lo >= _start->lo)
			result_->hi = _end->hi - _start->hi;
		else {
			result_->hi = (_end->hi - 1) - _start->hi;
		}
		result_->lo = _end->lo - _start->lo;
}

