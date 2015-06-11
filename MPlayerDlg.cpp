// MPlayerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MPlayer.h"
#include "MPlayerDlg.h"
#include "MThumbnail.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define SLIDERLONG 10000

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CMPlayerDlg �Ի���




CMPlayerDlg::CMPlayerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMPlayerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

}

void CMPlayerDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_SLIDER1, m_slider);
    DDX_Control(pDX, IDC_PATH, m_path);
}

BEGIN_MESSAGE_MAP(CMPlayerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_OPEN, &CMPlayerDlg::OnBnClickedOpen)
    ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER1, &CMPlayerDlg::OnNMReleasedcaptureSlider1)

//    ON_WM_HSCROLL()
//    ON_WM_LBUTTONDOWN()
ON_WM_HSCROLL()
ON_BN_CLICKED(IDC_BTNPLAY, &CMPlayerDlg::OnBnClickedBtnplay)
ON_BN_CLICKED(IDC_BTNSTOP, &CMPlayerDlg::OnBnClickedBtnstop)
//ON_WM_ERASEBKGND()
ON_WM_TIMER()
END_MESSAGE_MAP()


// CMPlayerDlg ��Ϣ�������

BOOL CMPlayerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
    m_slider.SetRange(0,SLIDERLONG);
    m_slider.SetPageSize(0);
    SetDlgItemText(IDC_PATH,_T(""));

    CSliderCtrl *pSlider = (CSliderCtrl *)GetDlgItem(IDC_SLIDER1);  
    m_objToolTip.Create(pSlider);  
    
    CString strText;  
    strText.Format(_T("%d"), pSlider->GetPos());  
    m_objToolTip.AddTool(pSlider, strText);


    vlc_ins=NULL;
    vlc_player=NULL;
    vlc_media=NULL;

    m_IsThumd=FALSE;
    m_OldPiont.SetPoint(0,0);
    m_pmtb=new MThumbnail;
    m_IsPlay=FALSE;
    m_IsPlay=FALSE;
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CMPlayerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMPlayerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{      
		 CDialog::OnPaint();
	}
            
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMPlayerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMPlayerDlg::OnBnClickedOpen()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    CString strfile(L"");
    CFileDialog dlgFile(TRUE);
    if(dlgFile.DoModal())
    {
    strfile=dlgFile.GetPathName();
    }

    SetDlgItemText(IDC_PATH,strfile);
}

void CMPlayerDlg::OnNMReleasedcaptureSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    if(!m_bFlag)
        return;
        CPoint point;
        if(::GetCursorPos(&point))
        {
            CRect rect;
            CRect rectChannel;
            ::GetClientRect(m_slider.GetSafeHwnd(),&rect);
            m_slider.GetChannelRect(&rectChannel);
            ::ScreenToClient(m_slider.GetSafeHwnd(),&point);
            
            if(rect.PtInRect(point))
            {
                int posRate=static_cast<int>((float)(point.x - rectChannel.left) * 100 / rectChannel.Width()+0.5);
                //int posRate = (point.x - rectChannel.left) * 100 / rectChannel.Width();
                int newPos = (SLIDERLONG - 0) * posRate / 100;
                m_slider.SetPos(newPos);
                int pos=0;
 /*               pos=(int)(((DOUBLE)point.x/(DOUBLE)rect.right)*SLIDERLONG);
                m_slider.SetPos(pos);*/
            }
            
        }  


    CSliderCtrl *pSlider = (CSliderCtrl *)GetDlgItem(IDC_SLIDER1);  
    CString strText;  
    strText.Format(_T("%d"), m_slider.GetPos());  
    m_objToolTip.UpdateTipText(strText, pSlider);


    *pResult = 0;
}

BOOL CMPlayerDlg::PreTranslateMessage(MSG *pMsg)
{
   
    m_objToolTip.RelayEvent(pMsg);
    return FALSE;
}

//void CMPlayerDlg::OnLButtonDown(UINT nFlags, CPoint point)
//{
//    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
//   
//
//            CRect rect;
//            ::GetClientRect(m_slider.GetSafeHwnd(),&rect);
//            ::ScreenToClient(m_slider.GetSafeHwnd(),&point);
//            
//            if(rect.PtInRect(point))
//            {
//                 SetDlgItemText(IDC_STATIC,L"2");
//            }
//            
// 
//    CDialog::OnLButtonDown(nFlags, point);
//}

void CMPlayerDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
    if(GetDlgItem(IDC_SLIDER1)==pScrollBar)
    {
        CPoint point;
        if(::GetCursorPos(&point))
        {
            CRect rect;
            ::GetClientRect(m_slider.GetSafeHwnd(),&rect);
            ::ScreenToClient(m_slider.GetSafeHwnd(),&point);
            
            if(rect.PtInRect(point))
            {
                
                switch(nSBCode)
                {
                    case SB_PAGERIGHT:
                    case SB_PAGELEFT:
                    case SB_LEFT:
                    case SB_RIGHT:
                    case SB_LINELEFT:
                    case SB_LINERIGHT:
                        m_bFlag=TRUE;
                        break;
                    case SB_THUMBPOSITION://�����ק���鲢�ͷ�ʱ��������ʱnPos��Ч  
                    case SB_THUMBTRACK://�����ק����ʱ��������ʱnPos��Ч 
                        CString strText;  
                        strText.Format(_T("%d"), m_slider.GetPos());  
                        m_objToolTip.UpdateTipText(strText, (CSliderCtrl *)pScrollBar);
                        m_bFlag=FALSE;
                        break;
                }
                
            }
            
        }  
        
    }
    CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CMPlayerDlg::OnBnClickedBtnplay()
{  
    // TODO: �ڴ���ӿؼ�֪ͨ����������

    Play(GetDlgItem(IDC_STATIC_V)->GetSafeHwnd(),NULL);

    /*
    const char * vlc_args[] = 
    {
    "-I",
    "--ignore-config",
    };

    // ����һ��VLCʵ��
    vlc_ins = libvlc_new(sizeof(vlc_args)/sizeof(vlc_args[0]), vlc_args);
   // vlc_ins = libvlc_new(0,NULL);
    if(vlc_ins != NULL)
    {
        // ����һ��VLC������
       // vlc_player = libvlc_media_player_new(vlc_ins);
       // if(vlc_player != NULL)
        if(vlc_player == NULL)
        {
            // ͨ���ļ�·������һ��ý��ʵ��,�������ҵĲ����ļ�
            CString strPath(L"");
            GetDlgItemText(IDC_PATH,strPath);
            vlc_media = libvlc_media_new_path(vlc_ins, UnicodeToAnsi(strPath.GetBuffer()));
            if(vlc_media != NULL)
            {
                // ����ý��ʵ��
            //    libvlc_media_parse(vlc_media);
                // ��ȡý���ļ��Ĳ��ų���,  ���� ms
             //   libvlc_time_t duration = libvlc_media_get_duration(vlc_media);

                // �˴��ǻ�ȡý������������Ƶ����Ƶ���Լ��������͵Ĺ����Ϣ
               // libvlc_media_track_info_t *media_tracks = NULL;
              //  int trackCount = libvlc_media_get_tracks_info(vlc_media, &media_tracks);
                // �������ͷ��ڴ棬���Ҳ��Ե�ʱ��������⣬��û��ϸ�о���Ϊ��
                // free(media_tracks);  // crash?

                // �Ѵ򿪵�ý���ļ����ø�������
                //libvlc_media_player_set_media(vlc_player, vlc_media);
                vlc_player=libvlc_media_player_new_from_media(vlc_media);
                libvlc_media_release(vlc_media);
                // ��Ϊ��windowsϵͳ��������Ҫ����һ��HWND����������Ϊ����,�����ֱ��ʹ�����洰��,������ǲ���
                
                //HWND hDisplay=GetDlgItem(IDC_STATIC_PIC)->GetSafeHwnd();
               // HWND hDisplay=::GetDesktopWindow();
             //   libvlc_media_player_set_hwnd(vlc_player, GetDlgItem(IDC_STATIC_PIC)->GetSafeHwnd());
               
                // ��ʼ������Ƶ
                libvlc_media_player_play(vlc_player);
              //  libvlc_media_player_set_time(vlc_player,(__int64)60000*2.5);
                // �������Ϊ�˴�ӡһЩ��Ϣ����,20����˳�
                time_t last_time = time(NULL);
                while((time(NULL) < (last_time + 60)))
                {
                    Sleep(500);
                    // ��ȡ��ǰ����λ��
                    libvlc_time_t play_time = libvlc_media_player_get_time(vlc_player);
                    CString strTime;
                    strTime.Format(_T("%lld ms"),(__int64)(play_time));
                    SetDlgItemText(IDC_STATIC2,strTime.GetBuffer());
                    printf("playing time : %lld ms ", (__int64)(play_time));
                    printf("playing postion: %f \r",libvlc_media_player_get_position(vlc_player));
                    // ��ȡý���״̬
                    //libvlc_state_t media_state = libvlc_media_get_state(vlc_media);
                    //printf("\nmedia state : %d\n", (int)(media_state));
                }
                // ֹͣ
                libvlc_media_player_stop(vlc_player);
                // �ͷ�
                libvlc_media_release(vlc_media);
            }
            // �ͷ�
            libvlc_media_player_release(vlc_player);
        }
        // �ͷ�
        libvlc_release(vlc_ins);
    }
    */
}

//�����ֽ�wchar_t*ת��Ϊ���ֽ�char*  
inline char* UnicodeToAnsi( const wchar_t* szStr )  
{  
    int nLen = WideCharToMultiByte( CP_ACP, 0, szStr, -1, NULL, 0, NULL, NULL );  
    if (nLen == 0)  
    {  
        return NULL;  
    }  
    char* pResult = new char[nLen];  
    WideCharToMultiByte( CP_ACP, 0, szStr, -1, pResult, nLen, NULL, NULL );  
    return pResult;  
} 

void CMPlayerDlg::OnBnClickedBtnstop()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������

        // TODO: �ڴ���ӿؼ�֪ͨ����������
    Stop();
    ////Invalidate();
}

void CMPlayerDlg::Play(HWND hWnd,int intin=22)
{
    const char * vlc_args[] = 
    {
    "-I",
    "--ignore-config",
    };

    // ����һ��VLCʵ��
    vlc_ins = libvlc_new(sizeof(vlc_args)/sizeof(vlc_args[0]), vlc_args);
   // vlc_ins = libvlc_new(0,NULL);
    if(vlc_ins != NULL)
    {
        // ����һ��VLC������
        vlc_player = libvlc_media_player_new(vlc_ins);
        if(vlc_player != NULL)
        {
            // ͨ���ļ�·������һ��ý��ʵ��,�������ҵĲ����ļ�
            CString strPath(L"");
            GetDlgItemText(IDC_PATH,strPath);
            m_pResult=UnicodeToAnsi(strPath.GetBuffer());
            vlc_media = libvlc_media_new_path(vlc_ins, m_pResult);
            if(vlc_media != NULL)
            {
                vlc_player = libvlc_media_player_new_from_media(vlc_media);
                // �Ѵ򿪵�ý���ļ����ø�������
                libvlc_media_player_set_hwnd(vlc_player,hWnd);
                libvlc_media_release(vlc_media);
                // ��ʼ������Ƶ
                libvlc_media_player_play(vlc_player);
                m_IsPlay=TRUE;
                if(intin==0)
                {
                    SetTimer(1,1000,NULL);   
                    //::SetTimer(this->GetSafeHwnd(),2,1000,GetVLCTime2);
                }

            }
      
    }
}
void CMPlayerDlg::GetVLCTime()
{
    libvlc_time_t play_time = libvlc_media_player_get_time(vlc_player);
    CString strTime;
    __int64 a=play_time/(1000*60*60);
    __int64 b=play_time/(1000*60)-a*60;
    __int64 c=play_time/1000-b*60-a*60*60;
    strTime.Format(_T("%.2d:%.2d:%.2d"),(int)a,(int)b,(int)c);
    SetDlgItemText(IDC_STATIC2,strTime.GetBuffer());

    CPoint point;
    if(::GetCursorPos(&point))
    {
        CRect rect;
        CRect rectChannel;
        ::GetClientRect(m_slider.GetSafeHwnd(),&rect);
        m_slider.GetChannelRect(&rectChannel);
        ::ScreenToClient(m_slider.GetSafeHwnd(),&point);
        
        if(rect.PtInRect(point))
        {
//            int posRate=static_cast<int>((float)(point.x - rectChannel.left) * 100 / rectChannel.Width()+0.5);
//            //int posRate = (point.x - rectChannel.left) * 100 / rectChannel.Width();
//            int newPos = (SLIDERLONG - 0) * posRate / 100;
//            m_slider.SetPos(newPos);
            int pos=0;
///*               pos=(int)(((DOUBLE)point.x/(DOUBLE)rect.right)*SLIDERLONG);
//            m_slider.SetPos(pos);*/
            if(m_IsThumd)
            {
                if(point.x==m_OldPiont.x&&point.y==m_OldPiont.y)
                {
                    return;
                }
                else
                {
                    libvlc_media_player_stop (vlc_player);      /* Stop playing */    
                    libvlc_media_player_release (vlc_player);   /* Free the media_player */
                    Play(::GetDlgItem(m_pmtb->GetSafeHwnd(),IDC_STATIC_V2),NULL);
                    m_IsThumd=TRUE;
                }
            }
            else
            {                
                    //MThumbnail *m_pmtb=new MThumbnail;
                    m_pmtb->Create(IDD_FORMVIEW);
                    m_pmtb->ShowWindow(SW_SHOW);
                    //::SetWindowPos( m_pmtb->GetSafeHwnd(), HWND_TOPMOST, 10, 10, 0, 0, SWP_NOMOVE||SWP_NOSIZE);
                    //::MoveWindow(m_pmtb->GetSafeHwnd(),10,10,0,0,NULL);
                  //  pMth->CloseWindow();
                    Play(::GetDlgItem(m_pmtb->GetSafeHwnd(),IDC_STATIC_V2),NULL);
                    m_IsThumd=TRUE;
            }
        }
        else
        {
            if(m_IsThumd)
            {
            //�ر�Ԥ��
                //libvlc_media_player_stop (vlc_player);      /* Stop playing */    
                //libvlc_media_player_release (vlc_player);   /* Free the media_player */
                Stop();
                m_pmtb->CloseWindow();
                m_IsThumd=FALSE;
            }
        }
        
    }  


    //CSliderCtrl *pSlider = (CSliderCtrl *)GetDlgItem(IDC_SLIDER1);  
    //CString strText;  
    //strText.Format(_T("%d"), m_slider.GetPos());  
    //m_objToolTip.UpdateTipText(strText, pSlider);



    
}
void WINAPI GetVLCTime2(HWND hWnd,UINT uMsg,UINT_PTR uTimer,DWORD dwTime)
{
    //libvlc_time_t play_time = libvlc_media_player_get_time(((CMPlayerDlg *)GetWindow(hWnd,SW_SHOW))->vlc_player);
    //CString strTime;
    //strTime.Format(_T("%lld ms"),(__int64)(play_time));
    //SetDlgItemText(hWnd,IDC_STATIC2,strTime.GetBuffer());
 
}

void CMPlayerDlg::Stop()
{
    if (vlc_player)
    {
        m_IsPlay=FALSE;
        libvlc_media_player_stop (vlc_player);      /* Stop playing */    
        libvlc_media_player_release (vlc_player);   /* Free the media_player */
        //vlc_player = NULL;
        libvlc_release(vlc_ins);
        if(m_pResult)
        {
            delete [] m_pResult;
            m_pResult=NULL;
        }
    }
}


void CMPlayerDlg::OnTimer(UINT_PTR nIDEvent)
{
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

     switch (nIDEvent)
    {
    case 1: ///����IDΪ1�Ķ�ʱ��
        if(m_IsPlay)
     GetVLCTime();
     break; 
    }
    CDialog::OnTimer(nIDEvent);
}
