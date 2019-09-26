
// MainFrm.cpp : CMainFrame Ŭ������ ����
//

#include "stdafx.h"
#include "Staff_DronSimulator.h"

#include "MainFrm.h"

#include "Staff_DronSimulatorDoc.h"
#include "Staff_DronSimulatorView.h"
#include "Staff_DronController.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CFixedSplitterWnd, CSplitterWnd)
	ON_WM_NCHITTEST()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // ���� �� ǥ�ñ�
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


CFixedSplitterWnd::CFixedSplitterWnd()
{}

CFixedSplitterWnd::~CFixedSplitterWnd()
{}

LRESULT CFixedSplitterWnd::OnNcHitTest(CPoint point)
{
	return HTNOWHERE;
}

// CMainFrame ����/�Ҹ�

CMainFrame::CMainFrame()
{
	// TODO: ���⿡ ��� �ʱ�ȭ �ڵ带 �߰��մϴ�.
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("���� ������ ������ ���߽��ϴ�.\n");
		return -1;      // ������ ���߽��ϴ�.
	}

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("���� ǥ������ ������ ���߽��ϴ�.\n");
		return -1;      // ������ ���߽��ϴ�.
	}

	

	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// TODO: ���� ������ ��ŷ�� �� ���� �Ϸ��� �� �� ���� �����Ͻʽÿ�.
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);


	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	cs.style &= ~(WS_THICKFRAME | WS_MAXIMIZEBOX); //������ ũ�����
	cs.cx = 1529;	//���α���
	cs.cy = 807;	//���α���

	return TRUE;
}

// CMainFrame ����

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame �޽��� ó����



BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	if (!m_wndSplitter.CreateStatic(this, 1, 2))
	{
		TRACE0("Fail to Create Splitter.\r");
		return FALSE;
	}

	//int m_IDpane = m_wndSplitter.IdFromRowCol(0, 0);
	//int m_IDpane2 = m_wndSplitter.IdFromRowCol(0, 1);

	//if (!m_wndSplitter2.CreateStatic(&m_wndSplitter, 2, 1, WS_CHILD | WS_VISIBLE, m_IDpane2))
	//{
	//	TRACE0("Fail to Create Splitter.\r");
	//	return FALSE;
	//}

	//CTopView�� CBottomView�� ���� ���� �ش�.
	m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CStaff_DronSimulatorView), CSize(1001, 300), pContext);
	m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(CStaff_DronController), CSize(300, 300), pContext);
	//m_wndSplitter2.CreateView(0, 0, RUNTIME_CLASS(CStaff_DronController), CSize(300, 275), pContext);
	//m_wndSplitter2.CreateView(1, 0, RUNTIME_CLASS(MyFormView), CSize(300, 10), pContext);
	return TRUE;

	//return CFrameWnd::OnCreateClient(lpcs, pContext);
}
