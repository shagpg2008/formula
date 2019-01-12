// formula.h : main header file for the FORMULA application
//

#if !defined(AFX_FORMULA_H__8CA2E956_1A47_4101_9154_6C945F8477BF__INCLUDED_)
#define AFX_FORMULA_H__8CA2E956_1A47_4101_9154_6C945F8477BF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CFormulaApp:
// See formula.cpp for the implementation of this class
//

class CFormulaApp : public CWinApp
{
public:
	CFormulaApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFormulaApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CFormulaApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FORMULA_H__8CA2E956_1A47_4101_9154_6C945F8477BF__INCLUDED_)
