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
	m_fileFormat = FT_WORD;
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
	GetDlgItem(IDC_IS_HAS_REMINDER)->EnableWindow(m_isDevide);
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
	//GetDlgItem(IDC_IS_HAS_REMINDER)->EnableWindow(m_isDevide && !isTrue);

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

	char buff[256] = {0};
	map<string, int> formulaMap;
	map<string, int>::iterator it;
	int numOfFormulaPerLine = atoi(m_numOfFormulaPerLine);
	int numOfFormulaPerPage = atoi(m_numOfFormulaPerPage);
	strncat(lpszFileName, ".doc", maxFileNameLen);
	CString msg = _T("");

	getTitle(buff, sizeof(buff));

    _Application app;  
	COleVariant vTrue((short)TRUE), vFalse((short)FALSE),vOpt((long)DISP_E_PARAMNOTFOUND, VT_ERROR);  
	app.CreateDispatch(_T("Word.Application"));  
	app.SetVisible(FALSE);  
	//Create New Doc  
	Documents docs=app.GetDocuments();  
	CComVariant tpl(_T("")),Visble,DocType(0),NewTemplate(false);  
	docs.Add(&tpl,&NewTemplate,&DocType,&Visble);  
	Selection sel=app.GetSelection();  

	//Add Table  
	_Document saveDoc=app.GetActiveDocument();  
	Tables tables=saveDoc.GetTables();  
	CComVariant defaultBehavior(1),AutoFitBehavior(1);  
	Table table = tables.Add(sel.GetRange(),(numOfFormulaPerPage+numOfFormulaPerLine-1)/numOfFormulaPerLine,numOfFormulaPerLine,&defaultBehavior,&AutoFitBehavior);  
	
	//Table table=tables.Item(1);  
	Borders borders = table.GetBorders();
	borders.SetEnable(FALSE);
	Rows rows = table.GetRows();
	rows.SetHeight(app.InchesToPoints((float)0.35), 2);
	rows.ReleaseDispatch();

	for(int index = 0, row = 1, col = 1; index < maxOfFormula; index ++, msg = "") {
		BOOL isNeedPageBreak = (index == (maxOfFormula - 1)) || (numOfFormulaPerPage == 0) || (index % numOfFormulaPerPage) != (numOfFormulaPerPage - 1)?FALSE:TRUE;
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
		sel.TypeText(msg);
		sel.MoveRight(COleVariant((short)1),COleVariant(short(1)),COleVariant(short(0)));
		
		if(isNeedPageBreak) {
			borders.ReleaseDispatch();
			table.ReleaseDispatch();
			sel.TypeText(" ");
			sel.InsertBreak(vTrue); 
			table=tables.Add(sel.GetRange(),(numOfFormulaPerPage+numOfFormulaPerLine-1)/numOfFormulaPerLine,numOfFormulaPerLine,&defaultBehavior,&AutoFitBehavior);
			borders = table.GetBorders();
			borders.SetEnable(FALSE);
			Rows rows = table.GetRows();
			rows.SetHeight(app.InchesToPoints((float)0.35), 2);
			rows.ReleaseDispatch();
			formulaMap.clear();
		}
	}

	Paragraph para = sel.GetParagraphs();
//	Fields fields;

	Window   mWindowActive = app.GetActiveWindow();;
	Pane   mPane = mWindowActive.GetActivePane();
	View   mViewActive = mPane.GetView(); 
	mViewActive.SetSeekView(9);   //����ҳü��ͼ
	getTitle(buff, sizeof(buff));
	
	_Font font(sel.GetFont());
	font.SetSize(15);
	font.SetColor(RGB(0,0,0));
	sel.TypeText(buff);
	para.SetAlignment(1);   //����
	sel.TypeParagraph();
	font.SetSize(9);
	sel.TypeText("��ʱ_______ �÷�_______");
	para.SetAlignment(2);   //����
	font.SetSize(7);

	mViewActive.SetSeekView(10);   //����ҳ����ͼ
	//sel.TypeText("- ");
	//fields.Add(sel.GetRange(),COleVariant(short(33)),COleVariant("PAGE  "),COleVariant(short(1))); //����ҳ����
	//sel.TypeText(" -");
	//font.SetSize(6);
	//font.SetColor(RGB(80,80,80));
	//para.SetAlignment(1);   //����
	sel.TypeText("�� �����            �� ������            �� Ҫ����"); 
	para.SetAlignment(1);   //����
	mViewActive.SetSeekView(0); //�ص�������ͼ
	font.ReleaseDispatch();
	para.ReleaseDispatch();
	mViewActive.ReleaseDispatch();
	mPane.ReleaseDispatch();
	mWindowActive.ReleaseDispatch();

	::GetModuleFileName(NULL,buff,sizeof(buff));
	msg = buff;

	int nPos=msg.ReverseFind('\\')+1;
	msg=msg.Left(nPos)+lpszFileName;
 
	saveDoc.SaveAs(COleVariant(msg),COleVariant((short)0),vOpt,vOpt,vOpt,vOpt,vOpt,vOpt
              ,vOpt,vOpt,vOpt,vOpt,vOpt,vOpt,vOpt,vOpt);

	//app.SetVisible(TRUE);  
	borders.ReleaseDispatch();
	table.ReleaseDispatch();  
	tables.ReleaseDispatch();  
	sel.ReleaseDispatch();  
	docs.ReleaseDispatch();  
	saveDoc.ReleaseDispatch();  
	app.ReleaseDispatch();  
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
		AfxMessageBox(_T("����ѡһ������(+,-,x���)"));
		return ;
	}

	if(numOfFormulaPerPage < 20 || numOfFormulaPerPage > 300) {
		AfxMessageBox(_T("ÿҳ����Ҫ����20��С��300!"));
		return ;
	}

	if(numOfFormulaPerLine < 3 || numOfFormulaPerLine > 5) {
		AfxMessageBox(_T("ÿ������Ҫ����2��С��6!"));
		return ;
	}

	core_init_operator(m_isAdd, m_isMinus, m_isMultiple, m_isDevide);
	core_init_maxnum(atoi(m_maxNumStr), atoi(m_MaxNumOfMinus), atoi(m_MaxNumOfMultiple), atoi(m_MaxNumOfDiv));
	core_init_misc(atoi(m_numOfOperatorStr)==3, m_isAddShiftIncluded, m_isIncludeAbdicate, m_divReminderNum == 1, m_isBracketPriority);
	
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
