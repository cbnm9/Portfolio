// Staff_DronController.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Staff_DronSimulator.h"
#include "Staff_DronSimulatorDoc.h"
#include "Staff_DronController.h"
#include "CustomDC.h"	//������۸������� ������ϻ��


#define PI 3.14159265359


ULONG_PTR g_GdiPlusTokenStaff2;

// CStaff_DronController

IMPLEMENT_DYNCREATE(CStaff_DronController, CView)

CStaff_DronController::CStaff_DronController()
	: m_ptMP2(0)
{
	/* Gdi+ Start */
	GdiplusStartupInput GdiplusStartupInput;
	GdiplusStartup(&g_GdiPlusTokenStaff2, &GdiplusStartupInput, NULL);
}

CStaff_DronController::~CStaff_DronController()
{
	/* Gdi+ Shutdown */
	GdiplusShutdown(g_GdiPlusTokenStaff2);
}

BEGIN_MESSAGE_MAP(CStaff_DronController, CView)
	ON_WM_MOUSEMOVE()
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CStaff_DronController �׸����Դϴ�.

void CStaff_DronController::OnDraw(CDC* nDC)
{
	CStaff_DronSimulatorDoc* pDoc = GetDocument();
	// TODO: ���⿡ �׸��� �ڵ带 �߰��մϴ�.
	CCustomDC pDC(nDC);

	CString str;
	//str.Format(_T("< %d, %d >"), m_ptMP2.x, m_ptMP2.y);

	pDC->MoveTo(0, 275);
	pDC->LineTo(500, 275);

	Graphics G(pDC);

	Bitmap membm(1000, 700);
	Graphics mem(&membm);

	SolidBrush brs(Color(255, 255, 255));
	SolidBrush bra(Color(255, 0, 0));
	mem.FillRectangle(&brs, 0, 0, 1000, 700);

	Pen bpn(Color(0, 0, 0), 3);

	mem.DrawRectangle(&bpn, 40, 28, 920, 644);


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

			if (tp->GetSD()>0)
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

	//������� ��ȭ��(mem)�� ���� �׸���
	CPoint BG_Point[3];
	Point BGP[3];
	CPoint V_Point[3];
	ObserveFunc(&(BG_Point[0]), &(BG_Point[1]), &(BG_Point[2]), &(V_Point[0]), &(V_Point[1]), &(V_Point[2]));
	//wd=100, hg=55

	for (int zzk = 0; zzk < 3; zzk++)
	{
		BGP[zzk].X = BG_Point[zzk].x;
		BGP[zzk].Y = BG_Point[zzk].y;
	}
	//���� ���� �����ǥ�� �̿��� ����� 4��ũ���� ��ȭ�� �߾ӿ� ��� ȸ���Ͽ� �׸���
	Bitmap bgbm(2000, 1400);
	Graphics bgbmg(&bgbm);
	bgbmg.FillRectangle(&brs, 0, 0, 2000, 1400);
	bgbmg.DrawImage(&membm, BGP, 3);

	//�׵� ����Ҹ�ŭ ��Ʈ���� �����Ͽ� �ű⿡ ������ ���� ��ȭ���� ����� �κи��� Ȯ�� ���
	Bitmap temp(100, 55);
	Graphics vbmg(&temp);
	//Graphics vbmg(&pDoc->m_BitmapView);
	vbmg.FillRectangle(&brs, 0, 0, 100, 55);

	vbmg.DrawImage(&bgbm, 0, 0, V_Point[0].x, V_Point[0].y, 100, 55, Gdiplus::Unit::UnitPixel);


	//vbmg.FillRectangle(&bra, 50, 50, 50, 50);
	//�׷��� ���� ��Ʈ���� �ٸ��信�� ���
	//G�� �׸���

	Point pptt[3] = { Point(0, 0), Point(500, 0), Point(0, 275) };
	G.DrawImage(&temp, pptt, 3);

	/* ���α׷� ���� */
	pDC->TextOutW(50, 300, _T("< ��� ����� ���� �䰡 ���õǾ� �������� �����մϴ�. >"));
	pDC->TextOutW(10, 350, _T("* ������ �������� ��ư�� �ʱ�ȭ�Դϴ�."));
	pDC->TextOutW(10, 400, _T("* �޴� ����"));
	pDC->TextOutW(20, 425, _T("- ���� ������� :  �ڵ�/����"));
	pDC->TextOutW(20, 450, _T("- ��� ������� :  �ڵ�/����"));
	pDC->TextOutW(20, 475, _T("- ��� ������� :  ����/����"));
	pDC->TextOutW(20, 500, _T("- ��Ÿ ����         :  �̵��ӵ�, ���������ֱ�, ����������"));
	pDC->TextOutW(10, 550, _T("* ��� ���� �������"));
	pDC->TextOutW(20, 575, _T("- (����Ű) ��         : ����"));
	pDC->TextOutW(20, 600, _T("- (����Ű) �Ʒ�     : ����"));
	pDC->TextOutW(20, 625, _T("- (����Ű) ����     : �ݽð�������� ȸ��"));
	pDC->TextOutW(20, 650, _T("- (����Ű) ������ : �ð�������� ȸ��"));

	//pDC->TextOutW(10, 10, str);
}


// CStaff_DronController �����Դϴ�.

#ifdef _DEBUG
void CStaff_DronController::AssertValid() const
{
	CView::AssertValid();
}

CStaff_DronSimulatorDoc* CStaff_DronController::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CStaff_DronSimulatorDoc)));
	return (CStaff_DronSimulatorDoc*)m_pDocument;
}

#ifndef _WIN32_WCE
void CStaff_DronController::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CStaff_DronController �޽��� ó�����Դϴ�.


void CStaff_DronController::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	m_ptMP2 = point;
	//Invalidate();

	CView::OnMouseMove(nFlags, point);
}


int CStaff_DronController::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  ���⿡ Ư��ȭ�� �ۼ� �ڵ带 �߰��մϴ�.
	SetTimer(30, 30, NULL);

	return 0;
}


void CStaff_DronController::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	CStaff_DronSimulatorDoc* pDoc = GetDocument();
	
	//if (pDoc->m_ick == 1)
	//{
	//	pDoc->m_ick = 0;
	//	Invalidate(FALSE);
	//}

	//Invalidate(FALSE);

	CView::OnTimer(nIDEvent);
}

void CStaff_DronController::ObserveFunc(CPoint *rlt, CPoint *rrt, CPoint *rl,
	CPoint *rslt, CPoint *rsrt, CPoint *rsl)	//Ȯ������� ���� �Լ�
{
	CStaff_DronSimulatorDoc* pDoc = GetDocument();
	// ����� �ʺ� 1000, ���̰� 700
	// ��ȭ����Ʈ���� �ʺ�� 2000, ���̴� 1400
	// ��ȭ����Ʈ�ʿ� ����� �׸��� ���� ������(500, 350) ��������(1500, 350), ���ʾƷ�(500, 1050) --1
	// �߽��� ��ǥ�� (1000, 700)
	// �߽ɿ��� ��� ������������ �Ÿ��� �� 610.32778 --2
	// 1�� 2�� �̿��Ͽ� ��� ��ü ������
	// �� �� ��ȭ���� ��� ������ ���(���ư� �������� ��ǥ�̿�)
	// ����� slt, srt, sl��ǥ ��ȭ���� �°� �籸�� (x��ǥ +500, y��ǥ +350)
	// ����� slt, srt, sl�� �߽ɰ��� �Ÿ����ϱ�
	// ����� slt, srt, sl�� ���������� ȸ��(������ ���� �Ÿ� �̿�)
	// 500*275 ��ȭ�� ���� �����
	// 

	//�߽ɿ��� ��� ������ ������ �Ÿ�
	double bglen;
	bglen = 610.32778;

	//����� �������, �������, �����ϴ� ��ǥ
	CPoint lt, rt, l;
	//ȸ���� ��ǥ
	//CPoint rlt, rrt, rl;
	lt.x = 500; lt.y = 350;
	rt.x = 1500; rt.y = 350;
	l.x = 500; l.y = 1050;
	//�� ���ִ��� ������ ����
	double lta, rta, la;
	lta = atan2(lt.y - 700, lt.x - 1000);
	rta = atan2(rt.y - 700, rt.x - 1000);
	la = atan2(l.y - 700, l.x - 1000);

	CPoint f;
	f = pDoc->m_Dron.GetPos();
	CPoint t;
	t = pDoc->m_Dron.GetSight();

	//ȸ���� ������ ���ϴ°�
	double ang;
	ang = pDoc->m_Dron.GetAng();

	//����� �κ��� �������, �������, �����ϴ� ��ǥ
	CPoint slt, srt, sl;
	//ȸ���� ��ǥ
	//CPoint rslt, rsrt, rsl;
	slt = pDoc->m_Dron.GetSlt();
	slt.x = slt.x + 500;
	slt.y = slt.y + 350;

	srt = pDoc->m_Dron.GetSrt();
	srt.x = srt.x + 500;
	srt.y = srt.y + 350;

	sl = pDoc->m_Dron.GetSl();
	sl.x = sl.x + 500;
	sl.y = sl.y + 350;
	//�� ���ִ��� ������ ����
	double slta, srta, sla;
	slta = atan2(slt.y - 700, slt.x - 1000);
	srta = atan2(srt.y - 700, srt.x - 1000);
	sla = atan2(sl.y - 700, sl.x - 1000);


	//����� �κ��� ��ǥ�� �߽ɰ��� �Ÿ�
	double sltlen, srtlen, sllen;
	sltlen = sqrt((slt.x - 1000)*(slt.x - 1000) + (slt.y - 700)*(slt.y - 700));
	srtlen = sqrt((srt.x - 1000)*(srt.x - 1000) + (srt.y - 700)*(srt.y - 700));
	sllen = sqrt((sl.x - 1000)*(sl.x - 1000) + (sl.y - 700)*(sl.y - 700));

	double ag90;
	ag90 = 90 * PI / 180;

	int ckp;
	ckp = ang * 180 / PI;

	if (ckp < 0)
	{
		// langl-90
		ag90 = (-1)*ang - ag90;

	}
	else if(ckp > 0)
	{
		// 270-langl
		ag90 = ag90 * 3 - ang;
	}
	else if (ckp == 0)
	{
		//-90
		ag90 = (-1)*ag90;
	}

	/*
	ang = atan2(p2.y - p0.y, p2.x - p0.x);	//���-�߽� ( ������ ���簢�� �ľ�)

	p1.x = (int)(speed*cos(ang)) + p0.x;
	p1.y = (int)(speed*sin(ang)) + p0.y;
	*/

	lta = lta + ag90; rta = rta + ag90; la = la + ag90;
	slta = slta + ag90; srta = srta + ag90; sla = sla + ag90;

	rlt->x = (int)(bglen*cos(lta)) + 1000;	rlt->y = (int)(bglen*sin(lta)) + 700;
	rrt->x = (int)(bglen*cos(rta)) + 1000;	rrt->y = (int)(bglen*sin(rta)) + 700;
	rl->x = (int)(bglen*cos(la)) + 1000;	rl->y = (int)(bglen*sin(la)) + 700;

	rslt->x = (int)(sltlen*cos(slta)) + 1000;	rslt->y = (int)(sltlen*sin(slta)) + 700;
	rsrt->x = (int)(srtlen*cos(srta)) + 1000;	rsrt->y = (int)(srtlen*sin(srta)) + 700;
	rsl->x = (int)(sllen*cos(sla)) + 1000;	rsl->y = (int)(sllen*sin(sla)) + 700;


}

BOOL CStaff_DronController::OnEraseBkgnd(CDC* pDC)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	return TRUE;

	//return CView::OnEraseBkgnd(pDC);
}
