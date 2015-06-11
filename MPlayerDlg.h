// MPlayerDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"

#include "vlc\libvlc.h"
#include "vlc\libvlc_media.h"
#include "vlc\libvlc_media_player.h"
#include "time.h"
#include "windows.h"
#pragma comment(lib, "libvlc.lib")
#pragma comment(lib, "libvlccore.lib")
#include "MThumbnail.h"
// CMPlayerDlg 对话框
class CMPlayerDlg : public CDialog
{
// 构造
public:
	CMPlayerDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MPLAYER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
    // 滑动条
    CSliderCtrl m_slider;
    // 路径
    CEdit m_path;
    afx_msg void OnBnClickedOpen();
    afx_msg void OnNMReleasedcaptureSlider1(NMHDR *pNMHDR, LRESULT *pResult);
    CToolTipCtrl m_objToolTip;
    virtual BOOL PreTranslateMessage(MSG *pMsg);
 
//    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    
    //VLC实例
    libvlc_instance_t *     vlc_ins    ;
    //VLC播放器
    libvlc_media_player_t * vlc_player ;
    //VLC视频对象
    libvlc_media_t *        vlc_media  ;
    afx_msg void OnBnClickedBtnplay();

    afx_msg void OnBnClickedBtnstop();
    void GetVLCTime();
//    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
private:
    void Play(HWND,int);
    void Stop();
    char* m_pResult;
    BOOL m_IsThumd;
    BOOL m_IsPlay;
    BOOL m_bFlag;//进度条点击或拖地
    CPoint m_OldPiont;
    MThumbnail *m_pmtb;
public:
    afx_msg void OnTimer(UINT_PTR nIDEvent);
};

inline char* UnicodeToAnsi( const wchar_t* szStr )  ;
void WINAPI GetVLCTime2(HWND,UINT,UINT_PTR,DWORD);
