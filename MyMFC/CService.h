#pragma once

#include <Winsvc.h>
#define ID_SUBMENU						2000
#define ID_SUBMENU1						2001
// CService �Ի���

class CService : public CDialogEx
{
	DECLARE_DYNAMIC(CService)

public:
	CService(CWnd* pParent = nullptr);   // ��׼���캯��
	virtual ~CService();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SERVICE_DLG
	};
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	LPCTSTR GetStringByCurrentState(DWORD state);
	CListCtrl m_ListCtrl;
	afx_msg void OnRclickList2(NMHDR *pNMHDR, LRESULT *pResult);
	void OnRClickMenu(UINT id);
	int m_nItem;
	SC_HANDLE hSCM;
};
