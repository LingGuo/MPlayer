// MThumbnail.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MPlayer.h"
#include "MThumbnail.h"


// MThumbnail �Ի���

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


// MThumbnail ��Ϣ�������
