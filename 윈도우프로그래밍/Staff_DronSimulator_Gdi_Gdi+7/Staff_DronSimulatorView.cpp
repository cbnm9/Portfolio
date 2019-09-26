
// Staff_DronSimulatorView.cpp : CStaff_DronSimulatorView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
#ifndef SHARED_HANDLERS
#include "Staff_DronSimulator.h"
#endif

#include "Staff_DronSimulatorDoc.h"
#include "Staff_DronSimulatorView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "CustomDC.h"	//������۸������� ������ϻ��

/*
1. ���̾�α� ����
4. ������ �Ʒ� ĭ�� ����
*/

#define PI 3.14159265359

/* �⺻Ÿ�̸�, ���� �ڵ����� Ÿ�̸� ID�� */
#define BASIC_TIMER 10
#define ANIMAL_TIMER 20

/* DRON */

/* �ڵ�,���� ��尪 m_iCtrlMode, m_iCreateMode */
#define AUTO 0
#define NON_AUTO 1

/* ��� ��尪(����, ���, ����) m_iDronMode */
#define DRON_ANIMAL 0
#define DRON_PESTICIDE 1
#define DRON_BACK 2

/* DRON_ANIMAL �����϶� (������, ���ݾƴҶ�) m_iDronAttackMode */
#define DRON_MOVE 0
#define DRON_ATTACK 1

/* DRON_PESTICIDE �����϶� */


/* DRON_BACK �����϶�(�̵���, ������) m_iDronHomeMode */
//#define DRON_MOVE 0 - �̹� ���ǵǾ� ����
#define DRON_CHARGING 1


/* ANIMAL */

/* ���� ������, ����ġ���� ��尪 CAnim::Mode */
#define ANIMAL_COME 0
#define ANIMAL_RUN 1

/* ANIMAL_COME �� ������ �������� ������������ ��尪 CAnim::Mode2 */
#define ANIMAL_MOVE 0
#define ANIMAL_ARRIVE 1

/* ANIMAL_RUN �� ������ ȭ�鿡�� ��������� ��尪 CAnim::Mode2 */
//#define ANIMAL_MOVE 0 - �̹� ���ǵǾ�����
#define ANIMAL_DISAPPEAR 1

/* GDI+ ����� ���� �������� ���� */
ULONG_PTR g_GdiPlusTokenStaff;

//HCURSOR hCursor, mh_prev_cursor1;

// CStaff_DronSimulatorView

IMPLEMENT_DYNCREATE(CStaff_DronSimulatorView, CView)

BEGIN_MESSAGE_MAP(CStaff_DronSimulatorView, CView)
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_COMMAND(ID_CREATMODE, &CStaff_DronSimulatorView::OnCreatmode)
	ON_UPDATE_COMMAND_UI(ID_CREATMODE, &CStaff_DronSimulatorView::OnUpdateCreatmode)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_CTRLMODE, &CStaff_DronSimulatorView::OnCtrlmode)
	ON_UPDATE_COMMAND_UI(ID_CTRLMODE, &CStaff_DronSimulatorView::OnUpdateCtrlmode)
	ON_WM_KEYDOWN()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_PESMODE, &CStaff_DronSimulatorView::OnPesmode)
	ON_UPDATE_COMMAND_UI(ID_PESMODE, &CStaff_DronSimulatorView::OnUpdatePesmode)
	ON_COMMAND(ID_STFSET, &CStaff_DronSimulatorView::OnStfset)
END_MESSAGE_MAP()

// CStaff_DronSimulatorView ����/�Ҹ�

CStaff_DronSimulatorView::CStaff_DronSimulatorView()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

	/* Gdi+ Start */
	GdiplusStartupInput GdiplusStartupInput;
	GdiplusStartup(&g_GdiPlusTokenStaff, &GdiplusStartupInput, NULL);

	/* Ŀ�� �ʱ�ȭ */
	//hCursor = AfxGetApp()->LoadCursorW(IDC_CURSOR_ANIM);
	//mh_prev_cursor1 = SetCursor(mh_scope_cursor1);
}

CStaff_DronSimulatorView::~CStaff_DronSimulatorView()
{
	/* Gdi+ Shutdown */
	GdiplusShutdown(g_GdiPlusTokenStaff);
}

BOOL CStaff_DronSimulatorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CView::PreCreateWindow(cs);
}

// CStaff_DronSimulatorView �׸���

void CStaff_DronSimulatorView::OnDraw(CDC* nDC)
{
	CStaff_DronSimulatorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.
	CCustomDC pDC(nDC);
	Graphics G(pDC);

	CString str1, str2;
	

	//���
	Bitmap membm(1000, 700);
	Graphics mem(&membm);

	SolidBrush brs(Color(255, 255, 255));
	SolidBrush bra(Color(255, 0, 0));
	mem.FillRectangle(&brs, 0, 0, 1000, 700);

	Pen bpn(Color(0, 0, 0), 3);

	mem.DrawRectangle(&bpn, 40, 28, 920, 644);
	//pDC->Rectangle(m_rectBAT);	//��Ÿ��

	mem.DrawImage(pDoc->m_BitmapBat, 50, 38, 900, 624);

	//����

	ImageAttributes ImgAttr1;
	ColorMap cm;
	cm.oldColor = Gdiplus::Color(255, 255, 0, 0);   // �����
	cm.newColor = Gdiplus::Color(0, 0, 0, 0);
	ImgAttr1.SetRemapTable(1, &cm);

	CAnim *tp;
	tp = pDoc->m_Animal.GetFp();
	while (1)
	{
		if (tp == &(pDoc->m_Animal))
			break;
		else
		{
			Point Am_Point[3];

			if(tp->GetSD()>0)
			{
				Am_Point[0].X = tp->GetPos().x + 20; Am_Point[0].Y = tp->GetPos().y - 20;
				Am_Point[1].X = tp->GetPos().x - 20; Am_Point[1].Y = tp->GetPos().y - 20;
				Am_Point[2].X = tp->GetPos().x + 20; Am_Point[2].Y = tp->GetPos().y + 20;
			}
			else
			{
				Am_Point[0].X = tp->GetPos().x - 20; Am_Point[0].Y = tp->GetPos().y - 20;
				Am_Point[1].X = tp->GetPos().x + 20; Am_Point[1].Y = tp->GetPos().y - 20;
				Am_Point[2].X = tp->GetPos().x - 20; Am_Point[2].Y = tp->GetPos().y + 20;
			}
			PointF Am_PF[3];
			for (int Am_Cnt = 0; Am_Cnt < 3; Am_Cnt++)
			{
				Am_PF[Am_Cnt].X = Am_Point[Am_Cnt].X;
				Am_PF[Am_Cnt].Y = Am_Point[Am_Cnt].Y;
			}

			mem.DrawImage(pDoc->m_BitmapAnim, Am_PF, 3, 0, 0, pDoc->m_BitmapAnim->GetWidth(), pDoc->m_BitmapAnim->GetHeight(), Gdiplus::Unit::UnitPixel, &ImgAttr1, NULL);
			tp = tp->GetFp();
		}
	}

	G.DrawImage(&membm, 0, 0);
	//

	//��� ��Ʈ��

	if (pDoc->m_iDronPecMode == 1)
	{
		Point Bit_Point1[3] =
		{ Point(pDoc->m_Dron.GetPos().x - 50, pDoc->m_Dron.GetPos().y - 50), Point(pDoc->m_Dron.GetPos().x + 50, pDoc->m_Dron.GetPos().y - 50), Point(pDoc->m_Dron.GetPos().x - 50, pDoc->m_Dron.GetPos().y + 50) };
		PointF pf1[3];
		for (int cntn1 = 0; cntn1 < 3; cntn1++)
		{
			pf1[cntn1].X = Bit_Point1[cntn1].X;
			pf1[cntn1].Y = Bit_Point1[cntn1].Y;
		}
		

		G.DrawImage(pDoc->m_BitmapPes, pf1, 3, 0, 0, pDoc->m_BitmapPes->GetWidth(), pDoc->m_BitmapPes->GetHeight(), Gdiplus::Unit::UnitPixel, &ImgAttr1, NULL);
	}
	

	//���
	CPen *pOldPen, NewPen;
	NewPen.CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
	
	pOldPen = pDC->SelectObject(&NewPen);
	pDC->MoveTo(pDoc->m_Dron.GetLB());
	pDC->LineTo(pDoc->m_Dron.GetRT());
	pDC->MoveTo(pDoc->m_Dron.GetLT());
	pDC->LineTo(pDoc->m_Dron.GetRB());
	pDC->SelectObject(pOldPen);
	//�þߺκ�
	pDoc->m_Dron.SetSP();
	pDC->MoveTo(pDoc->m_Dron.GetPos());
	pDC->LineTo(pDoc->m_Dron.GetSl());
	pDC->MoveTo(pDoc->m_Dron.GetPos());
	pDC->LineTo(pDoc->m_Dron.GetSr());
	pDC->MoveTo(pDoc->m_Dron.GetPos());
	pDC->LineTo(pDoc->m_Dron.GetSlt());
	pDC->MoveTo(pDoc->m_Dron.GetPos());
	pDC->LineTo(pDoc->m_Dron.GetSrt());
	//�»󳯰�
	pDC->MoveTo(pDoc->m_Dron.GetLTW(1));
	pDC->LineTo(pDoc->m_Dron.GetLTW(-1));
	//��󳯰�
	pDC->MoveTo(pDoc->m_Dron.GetRTW(1));
	pDC->LineTo(pDoc->m_Dron.GetRTW(-1));
	//���ϳ���
	pDC->MoveTo(pDoc->m_Dron.GetLBW(1));
	pDC->LineTo(pDoc->m_Dron.GetLBW(-1));
	//���ϳ���
	pDC->MoveTo(pDoc->m_Dron.GetRBW(1));
	pDC->LineTo(pDoc->m_Dron.GetRBW(-1));

	pDC->Ellipse(pDoc->m_Dron.GetRect());

	

	//��Ʈ��
	 //Gdi +
	
	Point Bit_Point[3] =
	{ Point(pDoc->m_Dron.GetSlt().x, pDoc->m_Dron.GetSlt().y), Point(pDoc->m_Dron.GetSrt().x, pDoc->m_Dron.GetSrt().y), Point(pDoc->m_Dron.GetSl().x, pDoc->m_Dron.GetSl().y) };
	PointF pf[3];
	
	for (int cntn = 0; cntn < 3; cntn++)
	{
		pf[cntn].X = Bit_Point[cntn].X;
		pf[cntn].Y = Bit_Point[cntn].Y;
	}

	ColorMatrix ClrMatrix = {
		1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.1f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.5f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 1.0f
	};

	ImageAttributes ImgAttr;
	ImgAttr.SetColorMatrix(&ClrMatrix, ColorMatrixFlagsDefault, ColorAdjustTypeBitmap);

	if (pDoc->m_iDronAttackMode == DRON_ATTACK)
	{
		switch (pDoc->m_iTog)
		{
		case 0:
			pDoc->m_iTog = 1;
			G.DrawImage(pDoc->m_BitmapR, pf, 3, 0, 0, pDoc->m_BitmapR->GetWidth(), pDoc->m_BitmapR->GetHeight(), Gdiplus::Unit::UnitPixel, &ImgAttr, NULL);
			break;
		case 1:
			pDoc->m_iTog = 0;
			break;
		}
	}
	else
	{
		G.DrawImage(pDoc->m_BitmapG, pf, 3, 0, 0, pDoc->m_BitmapG->GetWidth(), pDoc->m_BitmapG->GetHeight(), Gdiplus::Unit::UnitPixel, &ImgAttr, NULL);
	}
	
	//Ŀ�� ������ �ؼ��ʿ�
	

	//HCURSOR hCursor = AfxGetApp()->LoadCursorW(IDC_CURSOR_ANIM);
	//SetCursor(hCursor);
	if (CRect(0, 0, 1000, 700).PtInRect(pDoc->m_ptMP))
	{
		Point Cs_Point[3] = { Point(pDoc->m_ptMP.x - 20, pDoc->m_ptMP.y - 20), Point(pDoc->m_ptMP.x + 20, pDoc->m_ptMP.y - 20), Point(pDoc->m_ptMP.x - 20, pDoc->m_ptMP.y + 20) };
		PointF Cs_PF[3];
		for (int Cs_Cnt = 0; Cs_Cnt < 3; Cs_Cnt++)
		{
			Cs_PF[Cs_Cnt].X = Cs_Point[Cs_Cnt].X;
			Cs_PF[Cs_Cnt].Y = Cs_Point[Cs_Cnt].Y;
		}

		G.DrawImage(pDoc->m_BitmapAnim, Cs_PF, 3, 0, 0, pDoc->m_BitmapAnim->GetWidth(), pDoc->m_BitmapAnim->GetHeight(), Gdiplus::Unit::UnitPixel, &ImgAttr1, NULL);
	}

	if (pDoc->m_iCtrlMode == AUTO)
	{
		str1.Format(_T("������� : �ڵ�"));
	}
	else
	{
		str1.Format(_T("������� : ����"));
	}

	if (pDoc->m_iCreateMode == AUTO)
	{
		str2.Format(_T("�������� : �ڵ�"));
	}
	else
	{
		str2.Format(_T("�������� : ����"));
	}

	pDC->TextOutW(10, 10, str1);
	pDC->TextOutW(150, 10, str2);

	//str.Format(_T("(%d, %d)"), pDoc->m_ptMP.x, pDoc->m_ptMP.y);
	//pDC->TextOutW(10, 10, str);

	//str.Format(_T("dgr %lf"), pDoc->m_Dron.GetAng() * 180 / PI);
	//pDC->TextOutW(10, 30, str);

	//str.Format(_T("���� %d"), pDoc->m_dlgSet.m_iInx);
	//pDC->TextOutW(10, 50, str);

	//str.Format(pDoc->m_strPesTime);
	//pDC->TextOutW(10, 70, str);
}


// CStaff_DronSimulatorView ����

#ifdef _DEBUG
void CStaff_DronSimulatorView::AssertValid() const
{
	CView::AssertValid();
}

void CStaff_DronSimulatorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CStaff_DronSimulatorDoc* CStaff_DronSimulatorView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CStaff_DronSimulatorDoc)));
	return (CStaff_DronSimulatorDoc*)m_pDocument;
}
#endif //_DEBUG


// CStaff_DronSimulatorView �޽��� ó����---------------------------------------------------------------------------------------- �޼��� ó����


int CStaff_DronSimulatorView::OnCreate(LPCREATESTRUCT lpCreateStruct)	//---------------------------------------------------- �⺻ Ÿ�̸� �ʱ�ȭ(OnCreate)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  ���⿡ Ư��ȭ�� �ۼ� �ڵ带 �߰��մϴ�.

	srand((unsigned)time(0));
	SetTimer(BASIC_TIMER, 30, NULL);

	return 0;
}


void CStaff_DronSimulatorView::OnCreatmode()							//---------------------------------------------------- �޴�(�����������)
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	CStaff_DronSimulatorDoc* pDoc = GetDocument();

	switch (pDoc->m_iCreateMode)
	{
	case AUTO:
		pDoc->m_iCreateMode = NON_AUTO;
		KillTimer(ANIMAL_TIMER);
		pDoc->m_iCursorMode = 1;
		AfxMessageBox(_T("���� �������� ����Ǿ����ϴ�."));
		break;
	case NON_AUTO:
		pDoc->m_iCreateMode = AUTO;
		SetTimer(ANIMAL_TIMER, pDoc->m_iCreateSpeed, NULL);
		pDoc->m_iCursorMode = 0;
		AfxMessageBox(_T("�ڵ� �������� ����Ǿ����ϴ�."));
		break;
	}
}


void CStaff_DronSimulatorView::OnUpdateCreatmode(CCmdUI *pCmdUI)		//---------------------------------------------------- �޴�������Ʈ(�����������)
{
	// TODO: ���⿡ ��� ������Ʈ UI ó���� �ڵ带 �߰��մϴ�.
	CStaff_DronSimulatorDoc* pDoc = GetDocument();

	switch (pDoc->m_iCreateMode)
	{
	case AUTO:
		pCmdUI->SetText(_T("����"));
		break;
	case NON_AUTO:
		pCmdUI->SetText(_T("�ڵ�"));
		break;
	}
}


void CStaff_DronSimulatorView::OnMouseMove(UINT nFlags, CPoint point)	//---------------------------------------------------- ���콺 �̵�
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	CStaff_DronSimulatorDoc* pDoc = GetDocument();
	pDoc->m_ptMP = point;

	CView::OnMouseMove(nFlags, point);
}

void CStaff_DronSimulatorView::OnLButtonDown(UINT nFlags, CPoint point)	//---------------------------------------------------- ���콺 Ŭ��
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CStaff_DronSimulatorDoc* pDoc = GetDocument();

	switch (pDoc->m_iCreateMode)
	{
	case NON_AUTO:			//������尡 �����϶� ���콺 ���� ��ư�� �����ٸ�

		CAnim *tp;
		int id = rand() % 3 + 1;
		tp = new CAnim(point, pDoc->m_iAnimalSpeed, id);

		pDoc->m_Animal.Ins(tp);

		break;
	}
	pDoc->m_ptMP = point;
	CView::OnLButtonDown(nFlags, point);
}

BOOL CStaff_DronSimulatorView::OnEraseBkgnd(CDC* pDC)	//------------------------------------------------------------------- ȭ�� ����� �޼���
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	
	return TRUE;

	//return CView::OnEraseBkgnd(pDC);
}

void CStaff_DronSimulatorView::OnCtrlmode()	//------------------------------------------------------------------------------ �޴��� ��� �������
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	CStaff_DronSimulatorDoc* pDoc = GetDocument();

	switch (pDoc->m_iCtrlMode)
	{
	case AUTO:
		pDoc->m_iCtrlMode = NON_AUTO;
		AfxMessageBox(_T("���� �������� ����Ǿ����ϴ�."));
		break;
	case NON_AUTO:
		pDoc->m_iCtrlMode = AUTO;
		AfxMessageBox(_T("�ڵ� �������� ����Ǿ����ϴ�."));
		break;
	}

}


void CStaff_DronSimulatorView::OnUpdateCtrlmode(CCmdUI *pCmdUI)
{
	// TODO: ���⿡ ��� ������Ʈ UI ó���� �ڵ带 �߰��մϴ�.
	CStaff_DronSimulatorDoc* pDoc = GetDocument();

	switch (pDoc->m_iCtrlMode)
	{
	case AUTO:
		pCmdUI->SetText(_T("����"));
		break;
	case NON_AUTO:
		pCmdUI->SetText(_T("�ڵ�"));
		break;
	}

}

void CStaff_DronSimulatorView::OnPesmode()	//----------------------------------------------------------------------------- �޴��� ���������
{
	CStaff_DronSimulatorDoc* pDoc = GetDocument();

	if (pDoc->m_iDronMode == DRON_PESTICIDE)
	{
		pDoc->m_iDronMode = DRON_BACK;
		pDoc->m_iDronModeTemp = DRON_BACK;
		pDoc->m_iDronPecMode = 0;
		pDoc->m_iDronPec = 0;
	}
	else
	{
		pDoc->m_iDronMode = DRON_PESTICIDE;
		pDoc->m_iDronModeTemp = DRON_PESTICIDE;
	}
}


void CStaff_DronSimulatorView::OnUpdatePesmode(CCmdUI *pCmdUI)
{
	// TODO: ���⿡ ��� ������Ʈ UI ó���� �ڵ带 �߰��մϴ�.
	CStaff_DronSimulatorDoc* pDoc = GetDocument();

	if (pDoc->m_iDronMode == DRON_PESTICIDE)
	{
		pCmdUI->SetText(_T("����"));
	}
	else
	{
		pCmdUI->SetText(_T("������"));
	}
}

void CStaff_DronSimulatorView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)	//----------------------------------------- ���������� ���� Ű���� �޼���
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CStaff_DronSimulatorDoc* pDoc = GetDocument();

	switch (pDoc->m_iCtrlMode)
	{
	case NON_AUTO:

		switch (nChar)
		{
		case VK_UP:
			pDoc->m_Dron.Move();
			break;
		case VK_LEFT:
			//m_Dron.LMove();
			pDoc->m_Dron.SetSight2(-10*PI/180);
			break;
		case VK_RIGHT:
			//m_Dron.RMove();
			pDoc->m_Dron.SetSight2(10 * PI / 180);
			break;
		case VK_DOWN:
			pDoc->m_Dron.BMove();
			break;
		}

		break;
	case AUTO:
		break;
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CStaff_DronSimulatorView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.


	//pDoc->m_iDronMode = DRON_PESTICIDE;

	CView::OnRButtonDown(nFlags, point);
}

void CStaff_DronSimulatorView::OnStfset()	//--------------------------------------------------------------------------------- �����޴� ����
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	CStaff_DronSimulatorDoc* pDoc = GetDocument();

	KillTimer(BASIC_TIMER);

	if (pDoc->m_iCreateMode == AUTO)
		KillTimer(ANIMAL_TIMER);


	if (pDoc->m_dlgSet.DoModal() == IDOK)
	{
		pDoc->m_iCreateSpeed = pDoc->m_dlgSet.m_iGen;
		pDoc->m_Dron.SetSpeed(pDoc->m_dlgSet.m_iSpd * 2);
		pDoc->m_iAnimalSpeed = pDoc->m_dlgSet.m_iSpd;

		CAnim *p;
		p = pDoc->m_Animal.GetFp();
		while (p != &(pDoc->m_Animal))
		{
			p->SetSpeed(pDoc->m_dlgSet.m_iSpd);

			p = p->GetFp();
		}
	}

	SetTimer(BASIC_TIMER, 30, NULL);

	if (pDoc->m_iCreateMode == AUTO)
		SetTimer(ANIMAL_TIMER, pDoc->m_iCreateSpeed, NULL);
}

//==============================================================================================================================================================
//==============================================================================================================================================================

CPoint CStaff_DronSimulatorView::RandomAnimal()							//----------------------------------------------------- ���� �ڵ����� ��ġ �����ϰ� �����ִ��Լ�
{
	CPoint pt;

	int i = rand() % 4 + 1;
	switch (i)
	{
	case 1:
		pt.x = rand() % 920;
		pt.y = -20;
		break;
	case 2:
		pt.x = -20;
		pt.y = rand() % 680;
		break;
	case 3:
		pt.x = rand() % 820 + 90;
		pt.y = 720;
		break;
	case 4:
		pt.x = 960;
		pt.y = rand() % 530 + 90;
		break;
	}

	return pt;
}

int CStaff_DronSimulatorView::NumINC()									//---------------------------------------------------- ���� ������ ������ȣ �ο��Ҷ� ���� �Լ�(�Ⱦ�?)
{
	CStaff_DronSimulatorDoc* pDoc = GetDocument();

	if (pDoc->m_iAnimalNum < 300)
		pDoc->m_iAnimalNum++;
	else
		pDoc->m_iAnimalNum = 0;

	return 0;
}

void CStaff_DronSimulatorView::BM_GetSize(CPoint slt, CPoint srt, CPoint sl, double * wd, double * hg)
{
	double w = sqrt((slt.x - srt.x)*(slt.x - srt.x) + (slt.y - srt.y)*(slt.y - srt.y));
	double h = sqrt((slt.x - sl.x)*(slt.x - sl.x) + (slt.y - sl.y)*(slt.y - sl.y));

	*wd = w;
	*hg = h;
}


//==============================================================================================================================================================
//==============================================================================================================================================================

void CStaff_DronSimulatorView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CStaff_DronSimulatorDoc* pDoc = GetDocument();


	DronAct(nIDEvent);		//��п� ���Ѻκ�(View�� ����Լ�)

	AnimalAct(nIDEvent);	//������ ���Ѻκ�


								/* ����� �κи� �̰��� �ۼ� */

	//==============================================================================================================
	// ������ ����� ����ġ�� ������ ����, ����� ����
	//==============================================================================================================
	CAnim *p;
	p = pDoc->m_Animal.GetFp();
	int cck = 0;
	while (p != &(pDoc->m_Animal))
	{
		if (sqrt(pow((pDoc->m_Dron.GetPos().x - p->GetPos().x), 2) + pow((pDoc->m_Dron.GetPos().y - p->GetPos().y), 2)) <= 120)
		{
			p->SetMode(ANIMAL_RUN);
			p->SetMode2(ANIMAL_MOVE);
		}

		if (sqrt(pow((pDoc->m_Dron.GetPos().x - p->GetPos().x), 2) + pow((pDoc->m_Dron.GetPos().y - p->GetPos().y), 2)) <= 150)
		{
			cck = 1;
		}

		p = p->GetFp();
	}

	if (cck == 1)
		pDoc->m_iDronAttackMode = DRON_ATTACK;
	else
		pDoc->m_iDronAttackMode = DRON_MOVE;

	//===============================================================================================================

	//===============================================================================================================

	if (pDoc->m_strPesTime!=pDoc->m_dlgSet.m_strFirst)
	{
		if (pDoc->m_dlgSet.m_iInx != 0)
		{
			pDoc->m_strPesTime = pDoc->m_dlgSet.m_strFirst;
			pDoc->m_iPesTimeMode = 1;
		}
		else
			pDoc->m_iPesTimeMode = 0;
	}

	if (pDoc->m_iPesTimeMode == 1)
	{
		CString str2;
		str2.Format(_T("%d-%d-%d %d:%d"), COleDateTime::GetCurrentTime().GetYear(), COleDateTime::GetCurrentTime().GetMonth(),
			COleDateTime::GetCurrentTime().GetDay(), COleDateTime::GetCurrentTime().GetHour(), COleDateTime::GetCurrentTime().GetMinute());

		if (pDoc->m_strPesTime == str2)
		{
			pDoc->m_iDronMode = DRON_PESTICIDE;
			pDoc->m_iDronModeTemp = DRON_PESTICIDE;
			pDoc->m_dlgSet.m_ctTime.del(str2);
			pDoc->m_dlgSet.m_strFirst.Format(_T("����"));
		}
	}

	if (pDoc->m_iCreateMode == NON_AUTO)
		KillTimer(ANIMAL_TIMER);

	//Invalidate(FALSE);
	pDoc->UpdateAllViews(NULL);

	CView::OnTimer(nIDEvent);
}

void CStaff_DronSimulatorView::DronAct(UINT_PTR nIDEvent)		//---------------------------------------------------------------------- ��� ����������
{
	CStaff_DronSimulatorDoc* pDoc = GetDocument();

	switch (pDoc->m_iCtrlMode)
	{
	case AUTO:	//�ڵ����

		//DronMode ����
		//===============================================
		
		{
			CAnim *p, *t;
			p = pDoc->m_Animal.GetFp();
			t = NULL;
			int ck = INT_MAX;		//ck�� �⺻������ �ִ밪���� ����
			int ln;
			while (p != &(pDoc->m_Animal))
			{
				if (pDoc->m_rectBAT.PtInRect(p->GetPos()))	//������ ������ ���������� ����̵�
				{
					ln = (p->GetPos().x)*(p->GetPos().x) + (p->GetPos().y)*(p->GetPos().y);	//�񱳸� ���� ������ ��а��� �Ÿ�
					if (ln < ck)	//���� �Ÿ��� �����ٸ� �� ���� ����
					{
						ck = ln;
						t = p;
					}
				}

				p = p->GetFp();
			}

			if (ck == INT_MAX)		//������ ��Ÿ�� ���� ���ٸ�
			{
				switch (pDoc->m_iDronMode)
				{
				case DRON_ANIMAL:
					if (pDoc->m_iDronModeTemp == DRON_PESTICIDE)
					{
						if (pDoc->m_iDronPecModeTemp == 1)
						{
							pDoc->m_iDronPecModeTemp = 0;
							//pDoc->m_iDronPecMode = 1;
						}
					}
					pDoc->m_iDronMode = pDoc->m_iDronModeTemp;	//������ �ϴ� ��и�� ����
					break;
				case DRON_PESTICIDE:
					break;
				}
			}
			else					//������ ��Ÿ�� ���� �ִٸ�, ���� ����� ���� = t
			{
				pDoc->m_Dron.SetChase(t->GetPos());	//����� ������(���󰡴� ������ ��ǥ) ����
				pDoc->m_iDronMode = DRON_ANIMAL;		//����� �����������
				if (pDoc->m_iDronPecMode == 1)
				{
					pDoc->m_iDronPecModeTemp = 1;
					pDoc->m_iDronPecMode = 0;
				}
			}
		}
		
		//==============================================

		// ��� HOME ��� ���� (�� ���� ������ �������ߵ�)
		//==============================================
		if (pDoc->m_Dron.GetPos() != pDoc->m_ptCenter)
		{
			pDoc->m_iDronHomeMode = DRON_MOVE;
		}

		// ������ DronMode�� ���� ����� ������
		//==============================================
		switch (pDoc->m_iDronMode)
		{
		case DRON_ANIMAL:

			//�⺻������ ���������� �̵�
			{
			CPoint ps;
			ps = pDoc->m_Dron.GetChase();
			pDoc->m_Dron.SetSight(ps, 0);
			}
			pDoc->m_Dron.Move();

			//������ �����ϴ� ���� ����(�̵�����, ���ݵ���-��)
			switch (pDoc->m_iDronAttackMode)
			{
			case DRON_MOVE:
				break;
			case DRON_ATTACK:
				break;
			}

			pDoc->m_Dron.Wing();		//��� ������
			break;
		case DRON_PESTICIDE:
			pDoc->m_iDronModeTemp = DRON_PESTICIDE;

			switch (pDoc->m_iDronPecMode)
			{
			case 0:
				pDoc->m_Dron.SetSight(pDoc->m_ptPMV[pDoc->m_iDronPec], 0);
				pDoc->m_Dron.Move();
				pDoc->m_Dron.Wing();
				if (CRect(pDoc->m_ptPMV[pDoc->m_iDronPec].x - 20, pDoc->m_ptPMV[pDoc->m_iDronPec].y - 20, pDoc->m_ptPMV[pDoc->m_iDronPec].x + 20, pDoc->m_ptPMV[pDoc->m_iDronPec].y + 20).PtInRect(pDoc->m_Dron.GetPos()))
				{
					pDoc->m_Dron.SetPosition(pDoc->m_ptPMV[pDoc->m_iDronPec].x, pDoc->m_ptPMV[pDoc->m_iDronPec].y);
					pDoc->m_iDronPecMode = 1;
				}
				break;
			case 1:
				pDoc->m_Dron.SetSight(pDoc->m_ptPSP[pDoc->m_iDronPec], 0);
				pDoc->m_Dron.Move();
				pDoc->m_Dron.Wing();

				if (CRect(pDoc->m_ptPSP[pDoc->m_iDronPec].x - 20, pDoc->m_ptPSP[pDoc->m_iDronPec].y - 20, pDoc->m_ptPSP[pDoc->m_iDronPec].x + 20, pDoc->m_ptPSP[pDoc->m_iDronPec].y + 20).PtInRect(pDoc->m_Dron.GetPos()))
				{
					pDoc->m_Dron.SetPosition(pDoc->m_ptPSP[pDoc->m_iDronPec].x, pDoc->m_ptPSP[pDoc->m_iDronPec].y);
					if (pDoc->m_iDronPec == 19)
					{
						pDoc->m_iDronMode = DRON_BACK;
						pDoc->m_iDronModeTemp = DRON_BACK;
						pDoc->m_iDronPecMode = 0;
						pDoc->m_iDronPec = 0;
					}
					else
					{
						pDoc->m_iDronPec++;
						pDoc->m_iDronPecMode = 0;
					}
				}
				break;
			}

			break;
		case DRON_BACK:

			switch (pDoc->m_iDronHomeMode)
			{
			case DRON_MOVE:		//���� �����Ҷ����� ������
				pDoc->m_Dron.SetSight(pDoc->m_ptCenter, 0);
				pDoc->m_Dron.Move();

				if (CRect(475, 330, 515, 370).PtInRect(pDoc->m_Dron.GetPos()))	//�߾� ��ó�� �����ϸ� �߾����� �� ����, ��� �������
				{
					pDoc->m_Dron.SetPosition(495, 350);
					pDoc->m_iDronHomeMode = DRON_CHARGING;
				}

				pDoc->m_Dron.Wing();
				break;
			case DRON_CHARGING:	//���� �����ϸ� �ȿ�����
				break;
			}

			break;
		}

		break;
	case NON_AUTO:	//�������۸��

		pDoc->m_Dron.Wing();
		break;
	}
}

void CStaff_DronSimulatorView::AnimalAct(UINT_PTR nIDEvent)		//-------------------------------------------------------------- ���� ������ ����
{
	CStaff_DronSimulatorDoc* pDoc = GetDocument();

	if (pDoc->m_iCtrlMode == AUTO)//==================================================================twefwe
	{
		switch (nIDEvent)	//���� �ڵ�����(Timer�� ������)
		{
		case ANIMAL_TIMER:
			CAnim *tp;
			int id = rand() % 3 + 1;
			tp = new CAnim(RandomAnimal(), pDoc->m_iAnimalSpeed, id);

			pDoc->m_Animal.Ins(tp);
			break;
		}
	}
	

	CAnim *p, *d;	//d:������ ���� �����ͺ���
	d = NULL;
	int del=0;		//������ ���� ���ǰ˻� ����
	p = pDoc->m_Animal.GetFp();
	while (p != &(pDoc->m_Animal))
	{
		switch (p->GetMode())
		{
		case ANIMAL_COME:	//������ Come ��忡�� ����(�⺻ ��)

			switch (p->GetMode2())
			{
			case ANIMAL_MOVE:
				p->SetSight(p->GetDstn(), 0);
				p->Move();

				//������ ��ó�� �����ϸ� �������� ��ġ���� --> ��ġ���� �����ʰ� �¿�� �����ϰ� �ϴ°��� �����Ͱ���
				//if (CRect(p->GetDstn().x - 10, p->GetDstn().y - 10, p->GetDstn().x + 10, p->GetDstn().y + 10).PtInRect(p->GetPos()))
				//{
				//	pDoc->m_Animal.SetPosition(p->GetDstn().x, p->GetDstn().y);
				//	pDoc->m_Animal.SetMode2(ANIMAL_ARRIVE);
				//}

				break;
			case ANIMAL_ARRIVE:
				break;
			}

			break;
		case ANIMAL_RUN:

			//Run �϶� ���2�� ����(ȭ�� ������ ������ ������)
			{
			CRect rt;
			GetClientRect(&rt);
			if (!rt.PtInRect(p->GetPos()))
			{
				p->SetMode2(ANIMAL_DISAPPEAR);
			}
			}

			switch (p->GetMode2())
			{
			case ANIMAL_MOVE:
				p->Run(pDoc->m_Dron.GetPos());

				break;
			case ANIMAL_DISAPPEAR:
				d = p;
				del = 1;
				break;
			}


			break;
		}

		p = p->GetFp();

		if (del == 1)
		{
			del = 0;
			d->del();
		}
	}

}

/*	��� ���� �˻��ϴ� ���

CAnim *p;
p = m_Animal.GetFp();
while (p != &m_Animal)
{


p = p->GetFp();
}

*/

