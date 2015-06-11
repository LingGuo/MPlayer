// MThumbnail.cpp : 实现文件
//

#include "stdafx.h"
#include "MPlayer.h"
#include "MThumbnail.h"


// MThumbnail 对话框

IMPLEMENT_DYNAMIC(MThumbnail, CDialog)

MThumbnail::MThumbnail(CWnd* pParent /*=NULL*/)
	: CDialog(MThumbnail::IDD, pParent)
{

}

MThumbnail::~MThumbnail()
{
}

void MThumbnail::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(MThumbnail, CDialog)
END_MESSAGE_MAP()


// MThumbnail 消息处理程序
