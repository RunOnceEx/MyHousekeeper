#pragma once


// CDIALOG2 �Ի���

class CDIALOG2 : public CDialogEx
{
	DECLARE_DYNAMIC(CDIALOG2)

public:
	CDIALOG2(CWnd* pParent = nullptr);   // ��׼���캯��
	virtual ~CDIALOG2();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_ListCtrl;
	
};

