
// MyDlg.cpp: файл реализации
//

#include "pch.h"
#include "framework.h"
#include "cpl_loader.h"
#include "MyDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Диалоговое окно MyDlg



MyDlg::MyDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CPL_LOADER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void MyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, PathEdit);
	DDX_Control(pDX, IDC_EDIT2, NameEdit);
	DDX_Control(pDX, IDC_EDIT3, DescriptionEdit);
}

BEGIN_MESSAGE_MAP(MyDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_PATH_BUTTON, &MyDlg::OnBnClickedPathButton)
	ON_BN_CLICKED(IDC_BUTTON2, &MyDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// Обработчики сообщений MyDlg

BOOL MyDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	// TODO: добавьте дополнительную инициализацию

	InitIconDC(GetDlgItem(IDC_ICON_PICTURE)->GetSafeHwnd());

	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок.  Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void MyDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства для рисования

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Выравнивание значка по центру клиентского прямоугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Нарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR MyDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void MyDlg::OnBnClickedPathButton()
{
	// TODO: добавьте свой код обработчика уведомлений
	FilePath = GetFilePath();
	if (FilePath.GetLength() == 0)
	{
		PathEdit.SetWindowTextW(L"Путь не указан");
		return;
	}
	PathEdit.SetWindowTextW(FilePath);


	cplhInst = LoadDll(FilePath, cplfunc);
	cplinfo = GetAplletInfo(cplfunc);


	HICON icon;
	LoadIconWithScaleDown(cplhInst, MAKEINTRESOURCE(cplinfo.idIcon), frame.Width(), frame.Height(), &icon);
	memDC.DrawIcon(0, 0, icon);
	dc->StretchBlt(0, 0, frame.Width(), frame.Height(), &memDC, 0, 0, 32, 32, SRCCOPY);

	wchar_t name[255];
	LoadString(cplhInst, cplinfo.idName, name, 255);
	NameEdit.SetWindowTextW(name);

	wchar_t description[1024];
	LoadString(cplhInst, cplinfo.idInfo, description, 1024);
	DescriptionEdit.SetWindowTextW(description);
}


CString MyDlg::GetFilePath()
{
	CString path;

	CFileDialog Fdlg(TRUE);
	Fdlg.m_ofn.lpstrFilter = L"Апплеты\0*.cpl\0Все файлы\0*.*\0\0";
	Fdlg.DoModal();

	path = Fdlg.GetPathName();
	
	return path;
}
CPLINFO MyDlg::GetAplletInfo(CPL&func)
{
	CPLINFO ci;

	DWORD count = func(this->GetSafeHwnd(), CPL_GETCOUNT, NULL, NULL);

	func(this->GetSafeHwnd(), CPL_INQUIRE, count - 1, LPARAM(&ci));
	return ci;
}
HMODULE MyDlg::LoadDll(CString path, CPL& CPlApplet)
{
	auto hInt = LoadLibraryW(path);
	CPlApplet = (CPL)GetProcAddress(hInt, "CPlApplet");
	return hInt;
}

BOOL MyDlg::StartApplet()
{
	if (FilePath.IsEmpty())return FALSE;

	DWORD count = cplfunc(this->GetSafeHwnd(), CPL_GETCOUNT, NULL, NULL);
	cplfunc(this->GetSafeHwnd(), CPL_DBLCLK, count - 1, cplinfo.lData);
	return TRUE;

}


BOOL MyDlg::InitIconDC(HWND hWnd)
{
	// Получаем указатель на окно.
	wnd = CWnd::FromHandle(hWnd);
	// Получаем прямоугольник окна.
	wnd->GetClientRect(frame);
	// Получаем контекст для рисования в этом окне.
	dc = wnd->GetDC();

	// Создаем буфер-контекст.
	memDC.CreateCompatibleDC(dc);
	// Создаем растр для контекста рисования.
	bmp.CreateCompatibleBitmap(dc, frame.Width(), frame.Height());
	// Выбираем растр для использования буфер-контекстом.
	memDC.SelectObject(&bmp);

	memDC.FillSolidRect(frame, RGB(240, 240, 240));

	return TRUE;
}

void MyDlg::OnBnClickedButton2()
{
	// TODO: добавьте свой код обработчика уведомлений
	StartApplet();
}
