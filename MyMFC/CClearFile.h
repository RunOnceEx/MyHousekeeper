#pragma once


// CClearFile �Ի���

class CClearFile : public CDialogEx
{
	DECLARE_DYNAMIC(CClearFile)

public:
	CClearFile(CWnd* pParent = nullptr);   // ��׼���캯��
	virtual ~CClearFile();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG5 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	void OnClickBtn(UINT id);
	void EnumFoldAndDeleteFile(LPCWSTR szFoldPath);
	CString m_editStr;
};
