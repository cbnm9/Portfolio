// DlgCtl.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Staff_DronSimulator.h"
#include "DlgCtl.h"
#include "afxdialogex.h"

// CDlgCtl ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CDlgCtl, CDialogEx)

CDlgCtl::CDlgCtl(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG1, pParent)
	, m_iGen(6000)
	, m_iSpd(8)
	, m_iYear(0)
	, m_iMonth(0)
	, m_iDay(0)
	, m_iHour(0)
	, m_iMinute(0)
	, m_iSec(0)
	, m_iCheck(0)
	, m_iInx(0)
	, m_strFirst(_T(""))
{
	m_ctTime.ins(NULL);
}

CDlgCtl::~CDlgCtl()
{
}

void CDlgCtl::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDERGEN, m_sliderGen);
	DDX_Control(pDX, IDC_SLIDERSPD, m_sliderSpd);
	DDX_Control(pDX, IDC_LIST1, m_listBox);
	DDX_Control(pDX, IDC_DTPDATE, m_DTPDate);
	DDX_Control(pDX, IDC_DTPTIME, m_DTPTime);
}


BEGIN_MESSAGE_MAP(CDlgCtl, CDialogEx)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BADD, &CDlgCtl::OnBnClickedBadd)
	ON_BN_CLICKED(IDC_BDEL, &CDlgCtl::OnBnClickedBdel)
END_MESSAGE_MAP()

// CDlgCtl �޽��� ó�����Դϴ�.


BOOL CDlgCtl::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	m_sliderGen.SetRange(3000, 9000);
	m_sliderGen.SetPos(m_iGen);
	m_sliderSpd.SetRange(4, 12);
	m_sliderSpd.SetPos(m_iSpd);

	CCustTime *p;
	p = m_ctTime.fp;
	while (p != &(m_ctTime))
	{
		m_listBox.AddString(p->m_time);
		p = p->fp;
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CDlgCtl::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)	//�ӵ����� ��Ʈ��
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	int nGen = m_sliderGen.GetPos();
	int nSpd = m_sliderSpd.GetPos();

	if (pScrollBar->GetSafeHwnd() == m_sliderGen.m_hWnd)
	{
		m_iGen = nGen;
	}
	else if (pScrollBar->GetSafeHwnd() == m_sliderSpd.m_hWnd)
	{
		m_iSpd = nSpd;
	}
	else
		return;

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CDlgCtl::OnBnClickedBadd()	//������ ��¥�߰�
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	COleDateTime tpd;
	COleDateTime tpt;
	m_DTPDate.GetTime(tpd);
	m_DTPTime.GetTime(tpt);

	m_iYear = tpd.GetYear();
	m_iMonth = tpd.GetMonth();
	m_iDay = tpd.GetDay();
	m_iHour = tpt.GetHour();
	m_iMinute = tpt.GetMinute();
	m_iSec = tpt.GetSecond();

	COleDateTime nw;
	nw = COleDateTime::GetCurrentTime();

	if (m_iYear > nw.GetYear())
		m_iCheck = 1;
	else if (m_iYear == nw.GetYear())
	{
		if (m_iMonth > nw.GetMonth())
			m_iCheck = 1;
		else if (m_iMonth == nw.GetMonth())
		{
			if (m_iDay > nw.GetDay())
				m_iCheck = 1;
			else if (m_iDay == nw.GetDay())
			{
				if (m_iHour > nw.GetHour())
					m_iCheck = 1;
				else if (m_iHour == nw.GetHour())
				{
					if (m_iMinute > nw.GetMinute())
						m_iCheck = 1;
					else if (m_iMinute == nw.GetMinute())
					{

					}
				}
			}
		}
	}

	if (m_iCheck == 1)
	{
		CString str;

		str.Format(_T("%d-%d-%d %d:%d"), m_iYear, m_iMonth, m_iDay, m_iHour, m_iMinute);

		CCustTime *tm;
		tm = new CCustTime(str);
		m_ctTime.ins(tm);

		m_listBox.AddString(str);
		UpdateData(FALSE);
		m_iCheck = 0;
	}
	else
	{
		AfxMessageBox(_T("�߸��� ��¥�Է��Դϴ�."));
	}

	m_iInx = m_listBox.GetCount();

	if (m_iInx > 0)
	{
		m_listBox.GetText(0, m_strFirst);
	}
}


void CDlgCtl::OnBnClickedBdel()	//������ ��¥����
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	int index = m_listBox.GetCurSel();
	if (index != -1)
	{
		CString str;
		m_listBox.GetText(index, str);
		m_ctTime.del(str);
		m_listBox.DeleteString(index);
	}

	m_iInx = m_listBox.GetCount();

	if (m_iInx > 0)
	{
		m_listBox.GetText(0, m_strFirst);
	}
}
