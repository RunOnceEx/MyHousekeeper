#pragma once

// CDIALOG1 �Ի���

class CDIALOG1 : public CDialogEx
{
	DECLARE_DYNAMIC(CDIALOG1)

public:
	CDIALOG1(CWnd* pParent = nullptr);   // ��׼���캯��
	virtual ~CDIALOG1();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);
	CListCtrl m_ListCtrl;
	void ShowProcess();
	afx_msg void OnRclickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnRClickMenu(UINT id);
	INT m_RowNum;
};
