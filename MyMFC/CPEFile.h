#pragma once


// CPEFile �Ի���

class CPEFile : public CDialogEx
{
	DECLARE_DYNAMIC(CPEFile)

public:
	CPEFile(CWnd* pParent = nullptr);   // ��׼���캯��
	virtual ~CPEFile();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG7 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClickedButton1();
	CString m_strPath;
	afx_msg void OnClickedButton2();
};
