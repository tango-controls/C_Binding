// ============================================================================
//
// = CONTEXT
//   XDK
//
// = FILENAME
//   XDK_Operation.h
//
// = AUTHOR
//   Nicolas Leclercq
//
// ============================================================================

#ifndef _EXTERNAL_OPERATIONS_H_
#define _EXTERNAL_OPERATIONS_H_

//========================================================================================
// DEFINEs
//========================================================================================
#define XDK_Operations XOP

//========================================================================================
// EXTERNAL OPERATIONS (SINGLETON)
//========================================================================================
class XDK_Operations
{
public:
    //----------------------------------------------------------
    // XDK - Eexec des operations externes
    //----------------------------------------------------------
    static int exec (long _xop_id); 
    // Execute l'operation externe <_xop_id>.

private:
    static int sort_and_compact (int _order);
    //----------------------------------------------------------
    // METHODES INTERDITES
    //----------------------------------------------------------
    XDK_Operations ();
    XDK_Operations (const XDK_Operations&);
    void operator= (const XDK_Operations&); 
    virtual ~XDK_Operations ();
};

#endif // _EXTERNAL_OPERATIONS_H_
   
   