
// MyDlg.h: файл заголовка
//

#pragma once
#include<Cpl.h>

typedef LONG(*CPL)(HWND hwndCpl, UINT msg, LPARAM lParam1, LPARAM lParam2);

// Диалоговое окно MyDlg

class MyDlg : public CDialogEx
{
// Создание
public:
	MyDlg(CWnd* pParent = nullptr);	// стандартный конструктор

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CPL_LOADER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV


// Реализация
protected:
	HICON m_hIcon;

	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CEdit PathEdit;
	CEdit NameEdit;
	CEdit DescriptionEdit;
	afx_msg void OnBnClickedPathButton();

	CString FilePath;
	CPL cplfunc;
	CPLINFO cplinfo;
	HMODULE cplhInst;

	CString GetFilePath();
	HMODULE MyDlg::LoadDll(CString path, CPL& APlApplet);
	CPLINFO MyDlg::GetAplletInfo(CPL& func);
	BOOL StartApplet();

	


	//все для значка иконки
	// Прямоугольник области рисования.
	CRect frame;
	// Указатель на объект окна, обалсти рисования.
	CWnd* wnd;
	// Контекст рисования, привязанный к области рисования.
	CDC* dc;
	// Контекст рисования, привязанный к памяти.
	CDC memDC;
	// Память для контекста рисования memDC.
	CBitmap bmp;

	BOOL InitIconDC(HWND hWnd);
	afx_msg void OnBnClickedButton2();
};
