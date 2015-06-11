// MPlayerDlg.h : ͷ�ļ�
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
// CMPlayerDlg �Ի���
class CMPlayerDlg : public CDialog
{
// ����
public:
	CMPlayerDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MPLAYER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
    // ������
    CSliderCtrl m_slider;
    // ·��
    CEdit m_path;
    afx_msg void OnBnClickedOpen();
    afx_msg void OnNMReleasedcaptureSlider1(NMHDR *pNMHDR, LRESULT *pResult);
    CToolTipCtrl m_objToolTip;
    virtual BOOL PreTranslateMessage(MSG *pMsg);
 
//    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    
    //VLCʵ��
    libvlc_instance_t *     vlc_ins    ;
    //VLC������
    libvlc_media_player_t * vlc_player ;
    //VLC��Ƶ����
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
    BOOL m_bFlag;//������������ϵ�
    CPoint m_OldPiont;
    MThumbnail *m_pmtb;
public:
    afx_msg void OnTimer(UINT_PTR nIDEvent);
};

inline char* UnicodeToAnsi( const wchar_t* szStr )  ;
void WINAPI GetVLCTime2(HWND,UINT,UINT_PTR,DWORD);
