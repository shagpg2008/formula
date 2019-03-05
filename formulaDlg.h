// formulaDlg.h : header file
//

#if !defined(AFX_FORMULADLG_H__4F3D44B3_204A_4F8D_B88E_0D9641CF6B06__INCLUDED_)
#define AFX_FORMULADLG_H__4F3D44B3_204A_4F8D_B88E_0D9641CF6B06__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CFormulaDlg dialog

class CFormulaDlg : public CDialog
{
// Construction
public:
	CFormulaDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CFormulaDlg)
	enum { IDD = IDD_FORMULA_DIALOG };
	CString	m_maxNumStr;
	CString	m_numOfFormula;
	CString	m_numOfOperatorStr;
	BOOL	m_isAdd;
	BOOL	m_isDevide;
	BOOL	m_isMinus;
	BOOL	m_isMultiple;
	BOOL	m_isNumberNeeded;
	int	m_MaxNumOfDiv;
	int	m_MaxNumOfMultiple;
	CString	m_MaxNumOfMinus;	
	CString	m_numOfFormulaPerLine;
	CString	m_numOfFormulaPerPage;
	BOOL	m_isAddShiftIncluded;
	BOOL	m_isIncludeAbdicate;
	BOOL	m_isNumRestartPerPage;
	BOOL	m_isVertical;
	int		m_fileFormat;
	int		m_whichNumBlank;
	int		m_divReminderNum;
	BOOL	m_isBracketPriority;
	int		m_isAdicateOnly;
	int		m_isShiftOnly;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFormulaDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	void writeFormulaToTxtFile(int maxOfFormula, LPSTR lpszFileName, int maxFileNameLen);
	void writeFormulaToExcelFile(int maxOfFormula, LPSTR lpszFileName, int maxFileNameLen);
	void writeFormulaToWordFile(int maxOfFormula, LPSTR lpszFileName, int maxFileNameLen);
	void writeFormulaToHtmlFile(int maxOfFormula, LPSTR lpszFileName, int maxFileNameLen);

	// Generated message map functions
	//{{AFX_MSG(CFormulaDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnCheckAdd();
	afx_msg void OnCheckMinus();
	afx_msg void OnCheckMultiple();
	afx_msg void OnCheckDevide();
	virtual void OnOK();
	afx_msg void OnCheckNumberNeeded();
	afx_msg void OnSelchangeComboNumOfOperators();
	afx_msg void OnSelchangeComboMaxNumOfAdd();
	virtual void OnCancel();
	afx_msg void OnClose();
	afx_msg void OnIsAddShiftIncluded();
	afx_msg void OnIsIncludeAbdicate();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FORMULADLG_H__4F3D44B3_204A_4F8D_B88E_0D9641CF6B06__INCLUDED_)
