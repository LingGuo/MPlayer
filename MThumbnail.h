#pragma once


// MThumbnail �Ի���

class MThumbnail : public CDialog
{
	DECLARE_DYNAMIC(MThumbnail)

public:
	MThumbnail(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~MThumbnail();

// �Ի�������
	enum { IDD = IDD_FORMVIEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
