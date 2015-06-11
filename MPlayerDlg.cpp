// MPlayerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MPlayer.h"
#include "MPlayerDlg.h"
#include "MThumbnail.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define SLIDERLONG 10000

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CMPlayerDlg 对话框




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


// CMPlayerDlg 消息处理程序

BOOL CMPlayerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
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
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMPlayerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{      
		 CDialog::OnPaint();
	}
            
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMPlayerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMPlayerDlg::OnBnClickedOpen()
{
    // TODO: 在此添加控件通知处理程序代码
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
    // TODO: 在此添加控件通知处理程序代码
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
//    // TODO: 在此添加消息处理程序代码和/或调用默认值
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
    // TODO: 在此添加消息处理程序代码和/或调用默认值
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
                    case SB_THUMBPOSITION://鼠标拖拽滑块并释放时触发，此时nPos有效  
                    case SB_THUMBTRACK://鼠标拖拽滑块时触发，此时nPos有效 
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
    // TODO: 在此添加控件通知处理程序代码

    Play(GetDlgItem(IDC_STATIC_V)->GetSafeHwnd(),NULL);

    /*
    const char * vlc_args[] = 
    {
    "-I",
    "--ignore-config",
    };

    // 创建一个VLC实例
    vlc_ins = libvlc_new(sizeof(vlc_args)/sizeof(vlc_args[0]), vlc_args);
   // vlc_ins = libvlc_new(0,NULL);
    if(vlc_ins != NULL)
    {
        // 创建一个VLC播放器
       // vlc_player = libvlc_media_player_new(vlc_ins);
       // if(vlc_player != NULL)
        if(vlc_player == NULL)
        {
            // 通过文件路径创建一个媒体实例,这里是我的测试文件
            CString strPath(L"");
            GetDlgItemText(IDC_PATH,strPath);
            vlc_media = libvlc_media_new_path(vlc_ins, UnicodeToAnsi(strPath.GetBuffer()));
            if(vlc_media != NULL)
            {
                // 解析媒体实例
            //    libvlc_media_parse(vlc_media);
                // 获取媒体文件的播放长度,  返回 ms
             //   libvlc_time_t duration = libvlc_media_get_duration(vlc_media);

                // 此处是获取媒体包含多个的视频和音频轨以及其他类型的轨道信息
               // libvlc_media_track_info_t *media_tracks = NULL;
              //  int trackCount = libvlc_media_get_tracks_info(vlc_media, &media_tracks);
                // 这里是释放内存，但我测试的时候会有问题，还没仔细研究是为何
                // free(media_tracks);  // crash?

                // 把打开的媒体文件设置给播放器
                //libvlc_media_player_set_media(vlc_player, vlc_media);
                vlc_player=libvlc_media_player_new_from_media(vlc_media);
                libvlc_media_release(vlc_media);
                // 因为是windows系统，所以需要设置一个HWND给播放器作为窗口,这里就直接使用桌面窗口,这里仅是测试
                
                //HWND hDisplay=GetDlgItem(IDC_STATIC_PIC)->GetSafeHwnd();
               // HWND hDisplay=::GetDesktopWindow();
             //   libvlc_media_player_set_hwnd(vlc_player, GetDlgItem(IDC_STATIC_PIC)->GetSafeHwnd());
               
                // 开始播放视频
                libvlc_media_player_play(vlc_player);
              //  libvlc_media_player_set_time(vlc_player,(__int64)60000*2.5);
                // 这里仅是为了打印一些信息出来,20秒后退出
                time_t last_time = time(NULL);
                while((time(NULL) < (last_time + 60)))
                {
                    Sleep(500);
                    // 获取当前播放位置
                    libvlc_time_t play_time = libvlc_media_player_get_time(vlc_player);
                    CString strTime;
                    strTime.Format(_T("%lld ms"),(__int64)(play_time));
                    SetDlgItemText(IDC_STATIC2,strTime.GetBuffer());
                    printf("playing time : %lld ms ", (__int64)(play_time));
                    printf("playing postion: %f \r",libvlc_media_player_get_position(vlc_player));
                    // 获取媒体的状态
                    //libvlc_state_t media_state = libvlc_media_get_state(vlc_media);
                    //printf("\nmedia state : %d\n", (int)(media_state));
                }
                // 停止
                libvlc_media_player_stop(vlc_player);
                // 释放
                libvlc_media_release(vlc_media);
            }
            // 释放
            libvlc_media_player_release(vlc_player);
        }
        // 释放
        libvlc_release(vlc_ins);
    }
    */
}

//将宽字节wchar_t*转化为单字节char*  
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
    // TODO: 在此添加控件通知处理程序代码

        // TODO: 在此添加控件通知处理程序代码
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

    // 创建一个VLC实例
    vlc_ins = libvlc_new(sizeof(vlc_args)/sizeof(vlc_args[0]), vlc_args);
   // vlc_ins = libvlc_new(0,NULL);
    if(vlc_ins != NULL)
    {
        // 创建一个VLC播放器
        vlc_player = libvlc_media_player_new(vlc_ins);
        if(vlc_player != NULL)
        {
            // 通过文件路径创建一个媒体实例,这里是我的测试文件
            CString strPath(L"");
            GetDlgItemText(IDC_PATH,strPath);
            m_pResult=UnicodeToAnsi(strPath.GetBuffer());
            vlc_media = libvlc_media_new_path(vlc_ins, m_pResult);
            if(vlc_media != NULL)
            {
                vlc_player = libvlc_media_player_new_from_media(vlc_media);
                // 把打开的媒体文件设置给播放器
                libvlc_media_player_set_hwnd(vlc_player,hWnd);
                libvlc_media_release(vlc_media);
                // 开始播放视频
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
            //关闭预览
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
    // TODO: 在此添加消息处理程序代码和/或调用默认值

     switch (nIDEvent)
    {
    case 1: ///处理ID为1的定时器
        if(m_IsPlay)
     GetVLCTime();
     break; 
    }
    CDialog::OnTimer(nIDEvent);
}
