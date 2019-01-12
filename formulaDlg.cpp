// formulaDlg.cpp : implementation file
//

#include "stdafx.h"
#include "formula.h"
#include "formulaDlg.h"
#include"comdef.h"
#include"atlbase.h"
#include "msword.h" 

#include "core.h"
#include <iostream>
#include <map>
#include <string>

#define FT_EXCEL 0
#define FT_WORD  1
#define FT_TXT   2
#define FT_HTML  3


using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFormulaDlg dialog

CFormulaDlg::CFormulaDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFormulaDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFormulaDlg)
	m_maxNumStr = _T("20");
	m_numOfFormula = _T("1000");
	m_numOfOperatorStr = _T("2");
	m_isAdd = TRUE;
	m_isDevide = FALSE;
	m_isMinus = FALSE;
	m_isMultiple = FALSE;
	m_isNumberNeeded = FALSE;
	m_MaxNumOfDiv = _T("10 ");
	m_MaxNumOfMinus = _T("20 ");
	m_MaxNumOfMultiple = _T("5 ");
	m_numOfFormulaPerLine = _T("4");
	m_numOfFormulaPerPage = _T("100");
	m_isAddShiftIncluded = FALSE;
	m_isIncludeAbdicate = FALSE;
	m_isNumRestartPerPage = FALSE;
	m_isVertical = FALSE;
	m_fileFormat = FT_HTML;
	m_whichNumBlank = 0;
	m_divReminderNum = 0;
	m_isBracketPriority = FALSE;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFormulaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFormulaDlg)
	DDX_CBString(pDX, IDC_COMBO_MAX_NUM, m_maxNumStr);
	DDV_MaxChars(pDX, m_maxNumStr, 4);
	DDX_Text(pDX, IDC_NUM_OF_FORMULA, m_numOfFormula);
	DDV_MaxChars(pDX, m_numOfFormula, 4);
	DDX_CBString(pDX, IDC_COMBO_NUM_OF_OPERATORS, m_numOfOperatorStr);
	DDV_MaxChars(pDX, m_numOfOperatorStr, 1);
	DDX_Check(pDX, IDC_CHECK_ADD, m_isAdd);
	DDX_Check(pDX, IDC_CHECK_DEVIDE, m_isDevide);
	DDX_Check(pDX, IDC_CHECK_MINUS, m_isMinus);
	DDX_Check(pDX, IDC_CHECK_MULTIPLE, m_isMultiple);
	DDX_Check(pDX, IDC_CHECK_NUMBER_NEEDED, m_isNumberNeeded);
	DDX_CBString(pDX, IDC_COMBO_MAX_NUM_DIV, m_MaxNumOfDiv);
	DDX_CBString(pDX, IDC_COMBO_MAX_NUM_MINUS, m_MaxNumOfMinus);
	DDX_CBString(pDX, IDC_COMBO_MAX_NUM_MULTIPLE, m_MaxNumOfMultiple);
	DDX_Text(pDX, IDC_FORMULA_PER_LINE, m_numOfFormulaPerLine);
	DDX_Text(pDX, IDC_FORMULA_PER_PAGE, m_numOfFormulaPerPage);
	DDX_Check(pDX, IDC_IS_ADD_SHIFT_INCLUDED, m_isAddShiftIncluded);
	DDX_Check(pDX, IDC_IS_INCLUDE_ABDICATE, m_isIncludeAbdicate);
	DDX_Check(pDX, IDC_IS_NUM_RESTART, m_isNumRestartPerPage);
	DDX_Check(pDX, IDC_IS_VERTICAL, m_isVertical);
	DDX_Radio(pDX, IDC_IS_EXCEL, m_fileFormat);
	DDX_Radio(pDX, IDC_IS_EQUATION, m_whichNumBlank);
	DDX_Radio(pDX, IDC_IS_NO_REMINDER, m_divReminderNum);
	DDX_Check(pDX, IDC_CHECK_BRACKET, m_isBracketPriority);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFormulaDlg, CDialog)
	//{{AFX_MSG_MAP(CFormulaDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CHECK_ADD, OnCheckAdd)
	ON_BN_CLICKED(IDC_CHECK_MINUS, OnCheckMinus)
	ON_BN_CLICKED(IDC_CHECK_MULTIPLE, OnCheckMultiple)
	ON_BN_CLICKED(IDC_CHECK_DEVIDE, OnCheckDevide)
	ON_BN_CLICKED(IDC_CHECK_NUMBER_NEEDED, OnCheckNumberNeeded)
	ON_CBN_SELCHANGE(IDC_COMBO_NUM_OF_OPERATORS, OnSelchangeComboNumOfOperators)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFormulaDlg message handlers

BOOL CFormulaDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CFormulaDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CFormulaDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CFormulaDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CFormulaDlg::OnCheckAdd() 
{
	UpdateData();
	GetDlgItem(IDC_COMBO_MAX_NUM)->EnableWindow(m_isAdd);
	GetDlgItem(IDC_IS_ADD_SHIFT_INCLUDED)->EnableWindow(m_isAdd);
}

void CFormulaDlg::OnCheckMinus() 
{
	UpdateData();
	GetDlgItem(IDC_COMBO_MAX_NUM_MINUS)->EnableWindow(m_isMinus);
	GetDlgItem(IDC_IS_INCLUDE_ABDICATE)->EnableWindow(m_isMinus);
}

void CFormulaDlg::OnCheckMultiple() 
{
	UpdateData();
	GetDlgItem(IDC_COMBO_MAX_NUM_MULTIPLE)->EnableWindow(m_isMultiple);
}

void CFormulaDlg::OnCheckDevide() 
{
	UpdateData();
	GetDlgItem(IDC_COMBO_MAX_NUM_DIV)->EnableWindow(m_isDevide);
	GetDlgItem(IDC_IS_NO_REMINDER)->EnableWindow(m_isDevide);
	GetDlgItem(IDC_IS_HAS_REMINDER)->EnableWindow(m_isDevide && (atoi(m_numOfOperatorStr)>2?FALSE:TRUE));
}

void CFormulaDlg::OnCheckNumberNeeded() 
{
	UpdateData();
	GetDlgItem(IDC_IS_NUM_RESTART)->EnableWindow(m_isNumberNeeded);
}

void CFormulaDlg::OnSelchangeComboNumOfOperators() 
{
	UpdateData();
	BOOL isTrue = atoi(m_numOfOperatorStr)>2?TRUE:FALSE;
	GetDlgItem(IDC_IS_FILL_THIRD_NUM)->EnableWindow(isTrue);
	GetDlgItem(IDC_CHECK_BRACKET)->EnableWindow(isTrue);
	GetDlgItem(IDC_IS_HAS_REMINDER)->EnableWindow(m_isDevide && !isTrue);

	if(m_divReminderNum == 1) {	
		m_divReminderNum = 0;
		UpdateData(FALSE);
	}
}

void CFormulaDlg::OnClose() 
{
	CDialog::OnOK();
}

void CFormulaDlg::OnCancel() 
{
	CDialog::OnOK();
}

void CFormulaDlg::writeFormulaToTxtFile(int maxOfFormula, LPSTR lpszFileName, int maxFileNameLen)
{
	char buff[256] = {0};
	CString msg = _T("");
	map<string, int> formulaMap;
	map<string, int>::iterator it;
	int numOfFormulaPerLine = atoi(m_numOfFormulaPerLine);
	int numOfFormulaPerPage = atoi(m_numOfFormulaPerPage);
	int index = 0;

	strncat(lpszFileName, ".txt", maxFileNameLen);

	FILE *fp = fopen(lpszFileName, "wb");
	getTitle(buff, sizeof(buff));

	msg += buff;
	msg += "\r\n\r\n";

	for(index = 0; index < maxOfFormula; index ++) {
		BOOL isNeedPageBreak = numOfFormulaPerPage == 0 || (index % numOfFormulaPerPage) != (numOfFormulaPerPage - 1)?FALSE:TRUE;
		int lineNum = index + 1;
		genAnFormula(buff, sizeof(buff), m_whichNumBlank);

		if((it=formulaMap.find(buff)) != formulaMap.end()) {
			if(index - (it->second) <= 20) {
				index--;
				continue;
			}
			it->second = index;
		} else {
			formulaMap[buff] = index;
		}

		if(m_isNumberNeeded ) {
			if(m_isNumRestartPerPage) {
				lineNum = (index % numOfFormulaPerPage) + 1;
			}
			char lineBuff[8] = {0};
			msg += itoa(lineNum, lineBuff, 10);
			msg += _T(". ");
		}

		msg += buff;
		if((index % numOfFormulaPerLine) == (numOfFormulaPerLine - 1)) {
			msg += _T("\r\n");
		} else {
			msg += _T("    ");
		}
		
		if(isNeedPageBreak) {
			msg += _T("\x0C");
			fwrite((const char*)msg, msg.GetLength(), 1, fp);
			msg = _T("");
			formulaMap.clear();
		}
	}
	
	fwrite((const char*)msg, msg.GetLength(), 1, fp);
	fclose(fp);
}


void CFormulaDlg::writeFormulaToHtmlFile(int maxOfFormula, LPSTR lpszFileName, int maxFileNameLen)
{
	char buff[256] = {0};
	map<string, int> formulaMap;
	map<string, int>::iterator it;
	int numOfFormulaPerLine = atoi(m_numOfFormulaPerLine);
	int numOfFormulaPerPage = atoi(m_numOfFormulaPerPage);
	strncat(lpszFileName, ".html", maxFileNameLen);

	FILE *fp = fopen(lpszFileName, "wb");
	getTitle(buff, sizeof(buff));

	CString msg = _T("<html><head><title>");
	msg += buff;
	msg += "</title></head><body><table><caption>";
	msg += buff;
	msg += "</caption><tr>";

	for(int index = 0; index < maxOfFormula; index ++) {
		BOOL isNeedPageBreak = numOfFormulaPerPage == 0 || (index % numOfFormulaPerPage) != (numOfFormulaPerPage - 1)?FALSE:TRUE;
		int lineNum = index + 1;
		genAnFormula(buff, sizeof(buff), m_whichNumBlank);

		if((it=formulaMap.find(buff)) != formulaMap.end()) {
			if(index - (it->second) <= 20) {
				index--;
				continue;
			}
			it->second = index;
		} else {
			formulaMap[buff] = index;
		}

		if(m_isNumberNeeded ) {
			if(m_isNumRestartPerPage) {
				lineNum = (index % numOfFormulaPerPage) + 1;
			}
			char lineBuff[8] = {0};
			msg += itoa(lineNum, lineBuff, 10);
			msg += _T(". ");
		}

		msg += "<td>";
		msg += buff;
		msg += "</td>";
		if((index % numOfFormulaPerLine) == (numOfFormulaPerLine - 1)) {
			msg += _T("</tr><tr>");
		} /*else {
			msg += _T("    ");
		}*/
		
		if(isNeedPageBreak) {
			CString blank = "";
			int tmp = numOfFormulaPerLine;
			while(tmp > 0) {
				blank += "<td>&nbsp;</td>";
				tmp --;
			}

			blank += "</tr>\x0C<tr>";
			msg += blank;
			msg.Replace(" ", "&nbsp;");
			fwrite((const char*)msg, msg.GetLength(), 1, fp);
			msg = _T("");
			formulaMap.clear();
		}
	}

	if(msg != _T("")) {
		msg.Replace(" ", "&nbsp;");
	}

	msg += "</tr></table></body>";
	fwrite((const char*)msg, msg.GetLength(), 1, fp);
	fclose(fp);
}

void CFormulaDlg::writeFormulaToExcelFile(int maxOfFormula, LPSTR lpszFileName, int maxFileNameLen)
{
}

void CFormulaDlg::writeFormulaToWordFile(int maxOfFormula, LPSTR lpszFileName, int maxFileNameLen)
{
	if (FAILED(::CoInitialize(NULL)))
    {
         AfxMessageBox("Initialize COM context failed!");
         return ;
    }
    // TODO: Add your control notification handler code here
	_Application wordApp;

	Documents wordDocs;//创建一个新的word文档
	_Document wordDoc;

	Selection wordSelection;

	Range wordRange;//

	Tables wordTables;
	Table wordTable; 

	Cell wordCell;    
	Cells wordCells;
	//Paragraphs wordParagraphs;
	//Paragraph wordParagraph;
	_Font wordFont;
	Shading wordShading;
	//
	int nRow=0;
	//
	COleVariant  vTrue((short)TRUE),
				vFalse((short)FALSE),
				vOpt((long)DISP_E_PARAMNOTFOUND, VT_ERROR);

	CComVariant defaultBehavior(1),AutoFitBehavior(0);

	//创建word application实例
	if (!wordApp.CreateDispatch(_T("Word.Application")))
	{
		  AfxMessageBox("Word CreateDispatch Failed!");
		  return ;
	}    

	wordApp.SetVisible(true);//make visible
	wordApp.Activate();//激活
	wordDocs=wordApp.GetDocuments();//get documents object

	//
	CString strDocTemplate;
	CString strPath;

	char szFullPath[_MAX_PATH];
	::GetModuleFileName(NULL,szFullPath,_MAX_PATH);
	strDocTemplate.Format("%s",szFullPath);

	int nPos=strDocTemplate.ReverseFind('""');
	strDocTemplate=strDocTemplate.Left(nPos);
	strPath=strDocTemplate;
	TRACE1("%s\n",strDocTemplate);
	CComVariant tpl(_T("")),Visble,DocType(0),NewTemplate(false);
	wordDoc=wordDocs.Add(&tpl,&NewTemplate,&DocType,&Visble);
	wordSelection=wordApp.GetSelection();
	wordTables=wordDoc.GetTables();

	//move to end of story
	wordSelection.EndOf(COleVariant((short)6),COleVariant((short)0));
	//1.1 RxLev Full 
	wordSelection.TypeText("1. 统计报告");
	wordSelection.HomeKey(&CComVariant(5),&CComVariant(1));
	//Format the line with selection
	wordFont = wordSelection.GetFont();
	wordFont.SetBold(9999998);//wdToggle
	wordSelection.EndOf(&CComVariant(5),&CComVariant(0));
	wordSelection.TypeParagraph();   
	wordSelection.TypeText("(1.1). 分段统计");
	wordSelection.TypeParagraph();
	wordFont.SetBold(9999998);//wdToggle
	wordRange=wordSelection.GetRange();
	//add table
	//nRow=m_nRange1+1;
	wordTable=wordTables.Add(wordRange,5/*row*/,4/*column*/,&defaultBehavior,&AutoFitBehavior);
	wordRange=wordTable.GetRange();
	//wordRange.MoveEnd(COleVariant((short)15),COleVariant((short)1));
	//wordRange.Select();
	//move end of table
	//wordSelection.EndOf(COleVariant((short)15),COleVariant((short)0));
	//insert rows
	//wordSelection.InsertRowsBelow(COleVariant((short)5));
	//选择第一个单元,进而选择第一行进行格式化
	wordCell=wordTable.Cell(1,1);
	wordCell.Select();
	//select the row with current selection
	wordSelection.EndKey(&CComVariant(10),&CComVariant(1));
	//Format the row with selection
	//wordFont = wordSelection.GetFont();
	wordFont.SetBold(9999998);//wdToggle
	wordCells=wordSelection.GetCells();
	wordShading = wordCells.GetShading();
	wordShading.SetTexture(0);
	wordShading.SetBackgroundPatternColor(14737632);//15987699 14737632 adColorBlue
	wordShading.SetForegroundPatternColor(-16777216);//-16777216 wdColorAutomatic
	//move to end of table
	//wordSelection.EndOf(COleVariant((short)15),COleVariant((short)0));
	//wordParagraph=wordParagraphs.GetLast();
	//wordRange=wordParagraph.GetRange();
	//wordRange.MoveEnd(COleVariant((short)4),COleVariant((short)1));     
	//wordRange.SetText("Test");
	//wordSelection=wordApp.GetSelection();
	//wordSelection.MoveEnd(COleVariant((short)6),COleVariant((short)1));
	wordCell=wordTable.Cell(1,1);
	wordCell.Select();
	wordSelection.TypeText("统计项目");
	wordSelection.MoveRight(&CComVariant(12),&CComVariant(1),&CComVariant(0));
	wordSelection.TypeText("采样");
	wordSelection.MoveRight(&CComVariant(12),&CComVariant(1),&CComVariant(0));
	wordSelection.TypeText("百分比");
	wordSelection.MoveRight(&CComVariant(12),&CComVariant(1),&CComVariant(0));
	wordSelection.TypeText("累计百分比");
	//
	//
	wordSelection.EndOf(COleVariant((short)6),COleVariant((short)0));
	wordSelection.TypeParagraph();   
	wordSelection.TypeText("(1.2). 分段统计");
	wordSelection.HomeKey(&CComVariant(5),&CComVariant(1));
	//Format the line with selection
	wordFont = wordSelection.GetFont();
	wordFont.SetBold(9999998);//wdToggle
	wordSelection.EndOf(&CComVariant(5),&CComVariant(0));
	wordSelection.TypeParagraph();
	wordFont.SetBold(9999998);//wdToggle
	wordRange=wordSelection.GetRange();
	//add table
	//nRow=m_nRange1+1;
	wordTable=wordTables.Add(wordRange,5/*row*/,4/*column*/,&defaultBehavior,&AutoFitBehavior);
	wordRange=wordTable.GetRange();
	//选择第一个单元,进而选择第一行进行格式化
	wordCell=wordTable.Cell(1,1);
	wordCell.Select();
	//select the row with current selection
	wordSelection.EndKey(&CComVariant(10),&CComVariant(1));
	//Format the row with selection
	wordFont = wordSelection.GetFont();
	wordFont.SetBold(9999998);
	wordCells=wordSelection.GetCells();
	wordShading = wordCells.GetShading();
	wordShading.SetTexture(0);
	wordShading.SetBackgroundPatternColor(14737632);//15987699
	wordShading.SetForegroundPatternColor(-16777216);
	wordCell=wordTable.Cell(1,1);
	wordCell.Select();
	wordSelection.TypeText("Range");
	wordSelection.MoveRight(&CComVariant(12),&CComVariant(1),&CComVariant(0));
	wordSelection.TypeText("Samples");
	wordSelection.MoveRight(&CComVariant(12),&CComVariant(1),&CComVariant(0));
	wordSelection.TypeText("Percentage");
	wordSelection.MoveRight(&CComVariant(12),&CComVariant(1),&CComVariant(0));
	wordSelection.TypeText("Calculation");
	//
	//Save document as report
	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);
	CString strReport;
	strReport.Format("%d年-%d月-%d日 %d-%d-%d 报告",sysTime.wYear,sysTime.wMonth,
		  sysTime.wDay,sysTime.wHour,sysTime.wMinute,sysTime.wSecond);
	strReport=strPath+""""+strReport;
	wordDoc.SaveAs(COleVariant(strReport),COleVariant((short)0),vOpt,vOpt,vOpt,vOpt,vOpt,vOpt
		  ,vOpt,vOpt,vOpt,vOpt,vOpt,vOpt,vOpt,vOpt);
	//Release com
	wordFont.ReleaseDispatch();
	wordCells.ReleaseDispatch();
	wordShading.ReleaseDispatch();
	wordTable.ReleaseDispatch();
	wordTables.ReleaseDispatch();
	wordRange.ReleaseDispatch();
	wordSelection.ReleaseDispatch();
	wordDoc.RecheckSmartTags();
	wordDocs.ReleaseDispatch();
	wordApp.ReleaseDispatch();
	::CoUninitialize();
}


void CFormulaDlg::OnOK() 
{
	UpdateData();	
	TCHAR szFileName[MAX_PATH+1] = {0};
	int numOfFormulaPerLine = atoi(m_numOfFormulaPerLine);
	int numOfFormulaPerPage = atoi(m_numOfFormulaPerPage);
	int maxNumOfFormula = atoi(m_numOfFormula);
	
	if(!(m_isAdd || m_isMinus || m_isMultiple || m_isDevide)){
		AfxMessageBox(_T("至少选一个方法(+,-,x或÷)"));
		return ;
	}

	if(numOfFormulaPerPage < 20 || numOfFormulaPerPage > 300) {
		AfxMessageBox(_T("每页题数要大于20，小于300!"));
		return ;
	}

	if(numOfFormulaPerLine < 3 || numOfFormulaPerLine > 5) {
		AfxMessageBox(_T("每行题数要大于2，小于6!"));
		return ;
	}

	core_init_operator(m_isAdd, m_isMinus, m_isMultiple, m_isDevide);
	core_init_maxnum(atoi(m_maxNumStr), atoi(m_MaxNumOfMinus), atoi(m_MaxNumOfMultiple), atoi(m_MaxNumOfDiv));
	core_init_misc(atoi(m_numOfOperatorStr)==3, m_isAddShiftIncluded, m_isIncludeAbdicate, m_divReminderNum == 1);
	
	getFileName(szFileName, MAX_PATH);

	switch(m_fileFormat) {
	case FT_EXCEL: 
		writeFormulaToExcelFile(maxNumOfFormula, szFileName, MAX_PATH);
		break;
	case FT_WORD: 
		writeFormulaToWordFile(maxNumOfFormula, szFileName, MAX_PATH);
		break;
	case FT_TXT: 
		writeFormulaToTxtFile(maxNumOfFormula, szFileName, MAX_PATH);
		break;
	case FT_HTML: 
		writeFormulaToHtmlFile(maxNumOfFormula, szFileName, MAX_PATH);
		break;
	default:
		break;
	}

	ShellExecute (NULL,"open",szFileName,NULL,NULL,SW_SHOW);
}
