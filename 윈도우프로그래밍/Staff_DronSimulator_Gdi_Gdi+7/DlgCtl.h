#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "afxdtctl.h"
#include "CustTime.h"

// CDlgCtl ��ȭ �����Դϴ�.

class CDlgCtl : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgCtl)

public:
	CDlgCtl(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgCtl();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CSliderCtrl m_sliderGen;
	CSliderCtrl m_sliderSpd;
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	int m_iGen;
	int m_iSpd;
	CListBox m_listBox;
	CDateTimeCtrl m_DTPDate;
	CDateTimeCtrl m_DTPTime;
	afx_msg void OnBnClickedBadd();
	afx_msg void OnBnClickedBdel();
	int m_iYear;
	int m_iMonth;
	int m_iDay;
	int m_iHour;
	int m_iMinute;
	int m_iSec;
	int m_iCheck;
	int m_iInx;
	CCustTime m_ctTime;
	CString m_strFirst;
};
