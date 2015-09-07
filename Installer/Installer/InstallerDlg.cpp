
#include "stdafx.h"
#include "InstallerDlg.h"
#include <thread>
#include "resource.h"
#include "FolderDlg.h"
#include "ServicePath.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// finestra di dialogo CAboutDlg utilizzata per visualizzare le informazioni sull'applicazione.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dati della finestra di dialogo
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // supporto DDX/DDV

// Implementazione
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// finestra di dialogo CtestDlg



CtestDlg::CtestDlg(CWnd* pParent /*=NULL*/,bool NoGUI)
	: CDialogEx(IDD_TEST_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
	this->NoGUI = NoGUI;
}

void CtestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, TextBoxPath);
	DDX_Control(pDX, IDC_CHECK1, LinkDesktopChek);
	DDX_Control(pDX, IDC_CHECK2, LinkStartCheck);
}

BEGIN_MESSAGE_MAP(CtestDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON2, &CtestDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, &CtestDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, &CtestDlg::OnBnClickedButton3)
END_MESSAGE_MAP()


// gestori di messaggi di CtestDlg

BOOL CtestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();


	
	/*ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
	*/


	SetIcon(m_hIcon, TRUE);			// Impostare icona grande.
	SetIcon(m_hIcon, FALSE);		// Impostare icona piccola.


	CStatic * m_Label;
	CFont *m_Font1 = new CFont;
	m_Font1->CreatePointFont(140, _T("Arial Bold"));
	m_Label = (CStatic *)GetDlgItem(IDC_STATIC_titolo);
	m_Label->SetFont(m_Font1);


	SetDlgItemText(IDC_STATIC_titolo, _T("Installer"));
	SetWindowText(_T("Installer"));


	StringPath t = GlobVar::DefaultInstallFolder;
	t.ResolveEnviromentPath();
	TextBoxPath.SetWindowTextW(ServicePath::ComprimePath(t, GlobVar::InstallFolderName).lpwstr());


	si= new  ServiceInstall();
	
	si->InstallFolder = ServicePath::ComprimePath(GlobVar::DefaultInstallFolder, GlobVar::InstallFolderName);
	si->TempConfFileName = GlobVar::TempConfFileName;
	si->dialog = this;
	
	si->Clean();
	

	new thread(&ServiceInstall::CheckAndInstall, si);

	return TRUE;  // restituisce TRUE a meno che non venga impostato lo stato attivo su un controllo.
}

void CtestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// Se si aggiunge alla finestra di dialogo un pulsante di riduzione a icona, per trascinare l'icona sarà necessario
//  il codice sottostante.  Per le applicazioni MFC che utilizzano il modello documento/visualizzazione,
//  questa operazione viene eseguita automaticamente dal framework.

void CtestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // contesto di dispositivo per il disegno

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Centrare l'icona nel rettangolo client.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Disegnare l'icona
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// Il sistema chiama questa funzione per ottenere la visualizzazione del cursore durante il trascinamento
//  della finestra ridotta a icona.
HCURSOR CtestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}






void CtestDlg::OnBnClickedButton2()
{
	EndDialog(-1);
}


void CtestDlg::OnBnClickedButton1()
{
	
	if (si->CurrentScene == 2)
	{
		CString temp;
		TextBoxPath.GetWindowTextW(temp);

		String t = temp.GetBuffer();
		t.Trim();

		if (t == "")
		{
			MessageBox(_T("Selezionare una cartella per l'installazione"), _T("Selezionare una cartella"));
		}
		else
		{
			if (PathFileExists(t.lpcwstr()))
			{
				if (MessageBox(("Esiste già la cartella " + t + "\r\nContinuare?").lpwstr(), _T("Selezionare una cartella"), MB_YESNO) == IDYES)
				{
					si->InstallFolder = t;
				}
				else
					return;
			}
			else
			{
				CreateDirectory(t.lpwstr(), NULL);
				si->InstallFolder = t;
			}

			if (si->ip->DesktopLink == 1)
			{
				if (LinkDesktopChek.GetCheck() == 0)
					si->ip->DesktopLink = 0;
				else
					si->ip->DesktopLink = 2;
			}

			if (si->ip->StartMenuLink == 1)
			{
				if (LinkStartCheck.GetCheck() == 0)
					si->ip->StartMenuLink = 0;
				else
					si->ip->StartMenuLink = 2;
			}
			si->Install();
		}
	}
	else if (si->CurrentScene == 4)
	{
		EndDialog(-1);
	}
	
}


void CtestDlg::OnBnClickedButton3()
{
	
	StringPath s= GlobVar::DefaultInstallFolder;
	s.ResolveEnviromentPath();
	wstring title = _T("Seleziona la cartella di installazione");     
	CFolderDialog dlg(title.c_str(), s.lpwstr(), this);
	if (dlg.DoModal() == IDOK)
	{
		TextBoxPath.SetWindowTextW(ServicePath::ComprimePath(dlg.GetFolderPath(), GlobVar::InstallFolderName).lpcwstr());
	}
	
}

