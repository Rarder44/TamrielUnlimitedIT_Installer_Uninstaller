
// UninstallerDlg.h : file di intestazione
//

#pragma once
#include "ServiceUninstall.h"
#include "afxwin.h"

// finestra di dialogo CUninstallerDlg
class CUninstallerDlg : public CDialogEx
{
// Costruzione
public:
	CUninstallerDlg(CWnd* pParent = NULL);	// costruttore standard
	ServiceUninstall* su;

// Dati della finestra di dialogo
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_UNINSTALLER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// supporto DDX/DDV


// Implementazione
protected:
	HICON m_hIcon;

	// Funzioni generate per la mappa dei messaggi
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()


public:
	CButton Bottone;
	CComboBox ComboSelezione;

	afx_msg void OnBnClickedButton1();
};
