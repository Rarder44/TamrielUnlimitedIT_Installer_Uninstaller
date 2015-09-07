

// testDlg.h : file di intestazione


#include "afxdialogex.h"
#include "afxwin.h"
#include "ServiceInstall.h"


using namespace std;



// finestra di dialogo CtestDlg
class CtestDlg : public CDialogEx
{
// Costruzione
public:
	bool NoGUI;
	CtestDlg(CWnd* pParent = NULL,bool NoGUI=false);	// costruttore standard

// Dati della finestra di dialogo
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TEST_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// supporto DDX/DDV


// Implementazione
protected:
	HICON m_hIcon;

	// Funzioni generate per la mappa dei messaggi
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:


	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton3();


	ServiceInstall *si;
	CEdit TextBoxPath;
	CButton LinkDesktopChek;
	CButton LinkStartCheck;
};


