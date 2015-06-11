#pragma once


// MThumbnail 对话框

class MThumbnail : public CDialog
{
	DECLARE_DYNAMIC(MThumbnail)

public:
	MThumbnail(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~MThumbnail();

// 对话框数据
	enum { IDD = IDD_FORMVIEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
