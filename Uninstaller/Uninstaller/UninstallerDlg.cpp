
// UninstallerDlg.cpp : file di implementazione
//

#include "stdafx.h"
#include "Uninstaller.h"
#include "UninstallerDlg.h"
#include "afxdialogex.h"
#include "ServiceUninstall.h"
#include <thread>
#include "StringPath.h"
#include "GlobVar.h"
#include "ServicePath.h"
#include "ServiceSystem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// finestra di dialogo CUninstallerDlg



CUninstallerDlg::CUninstallerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_UNISTALLER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
}

void CUninstallerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON1, Bottone);
	DDX_Control(pDX, IDC_COMBO1, ComboSelezione);
}

BEGIN_MESSAGE_MAP(CUninstallerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()

	ON_BN_CLICKED(IDC_BUTTON1, &CUninstallerDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// gestori di messaggi di CUninstallerDlg

BOOL CUninstallerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Impostare l'icona per questa finestra di dialogo.  Il framework non esegue questa operazione automaticamente
	//  se la finestra principale dell'applicazione non è una finestra di dialogo.
	SetIcon(m_hIcon, TRUE);			// Impostare icona grande.
	SetIcon(m_hIcon, FALSE);		// Impostare icona piccola.

	CStatic * m_Label;
	CFont *m_Font1 = new CFont;
	m_Font1->CreatePointFont(140, _T("Arial Bold"));
	m_Label = (CStatic *)GetDlgItem(IDC_STATIC_TITOLO);
	m_Label->SetFont(m_Font1);


	SetDlgItemText(IDC_STATIC_TITOLO, _T("Uninstaller"));
	SetWindowText(_T("Uninstaller"));




	su = new  ServiceUninstall();
	su->dialog = this;
	su->CleanScene();

	StringPath sp = ServicePath::ComprimePath(ServiceSystem::GetCurrentExecutiblePath().GetDirectory(), GlobVar::DefaultNameUninstallIni);
	su->UninstallIniPath = *sp.RemoveDuplicateSlash();

	
	thread* t = new thread(&ServiceUninstall::PreUninstall, su);
	return TRUE;  // restituisce TRUE a meno che non venga impostato lo stato attivo su un controllo.
}

// Se si aggiunge alla finestra di dialogo un pulsante di riduzione a icona, per trascinare l'icona sarà necessario
//  il codice sottostante.  Per le applicazioni MFC che utilizzano il modello documento/visualizzazione,
//  questa operazione viene eseguita automaticamente dal framework.

void CUninstallerDlg::OnPaint()
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
HCURSOR CUninstallerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}





void CUninstallerDlg::OnBnClickedButton1()
{
	if (su->CurrentScene == 1)
	{
		CString s;
		ComboSelezione.GetLBText(ComboSelezione.GetCurSel(), s);
		String ss = s.GetBuffer();

		list<int> l = su->ip->TipiDiDisistallazione.GetAllID();
		if (l.size() == 0)
		{
			String s = "Nessuna informazione trovata per procedere con la disistallazione";
			MessageBox(s.lpcwstr(), _T("Errore info per disistallazione"), 0);
			EndDialog(-1);
			return;
		}

		bool trovato = false;
		for each (int i in l)
		{
			UninstallType* ut = nullptr;
			 ut = su->ip->TipiDiDisistallazione.Find(i);
			if (ut)
				if (ut->ToString() == ss)
				{
					trovato = true;
					su->SelectedUninstallType = ut->GetID();
					break;
				}
		}
		if (!trovato)
		{
			String s = "Impossibile recuperare le informazioni per la disistallazione";
			MessageBox(s.lpcwstr(), _T("Errore info per disistallazione"), 0);
			EndDialog(-1);
			return;
		}
		new thread(&ServiceUninstall::Uninstall, su);

	}
	else if (su->CurrentScene == 2)
	{
		if (MessageBox(_T("ATTENZIONE ALCUNI FILE POSSO GIA ESSER STATI CANCELLATI!\r\nSicuro di voler annullare?"), _T("Errore info per disistallazione"), MB_YESNO) == IDYES)
		{
			EndDialog(-1);
		}
	}
	else if (su->CurrentScene ==3)
	{
		EndDialog(-1);
	}
	
}
