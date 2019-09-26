// Staff_DronController.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Staff_DronSimulator.h"
#include "Staff_DronSimulatorDoc.h"
#include "Staff_DronController.h"
#include "CustomDC.h"	//더블버퍼링을위한 헤더파일사용


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


// CStaff_DronController 그리기입니다.

void CStaff_DronController::OnDraw(CDC* nDC)
{
	CStaff_DronSimulatorDoc* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.
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
	

	//동물
	ImageAttributes ImgAttr1;
	ColorMap cm;
	cm.oldColor = Gdiplus::Color(255, 255, 0, 0);   // 투명색
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

	//여기까지 도화지(mem)에 따로 그린후
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
	//먼저 구한 배경좌표를 이용해 배경의 4배크기의 도화지 중앙에 배경 회전하여 그리기
	Bitmap bgbm(2000, 1400);
	Graphics bgbmg(&bgbm);
	bgbmg.FillRectangle(&brs, 0, 0, 2000, 1400);
	bgbmg.DrawImage(&membm, BGP, 3);

	//그뒤 출력할만큼 비트맵을 생성하여 거기에 위에서 만든 도화지중 출력할 부분만을 확대 출력
	Bitmap temp(100, 55);
	Graphics vbmg(&temp);
	//Graphics vbmg(&pDoc->m_BitmapView);
	vbmg.FillRectangle(&brs, 0, 0, 100, 55);

	vbmg.DrawImage(&bgbm, 0, 0, V_Point[0].x, V_Point[0].y, 100, 55, Gdiplus::Unit::UnitPixel);


	//vbmg.FillRectangle(&bra, 50, 50, 50, 50);
	//그렇게 만든 비트맵은 다른뷰에서 사용
	//G에 그리기

	Point pptt[3] = { Point(0, 0), Point(500, 0), Point(0, 275) };
	G.DrawImage(&temp, pptt, 3);

	/* 프로그램 설명 */
	pDC->TextOutW(50, 300, _T("< 모든 기능은 좌측 뷰가 선택되어 있을때만 동작합니다. >"));
	pDC->TextOutW(10, 350, _T("* 툴바의 새페이지 버튼은 초기화입니다."));
	pDC->TextOutW(10, 400, _T("* 메뉴 설명"));
	pDC->TextOutW(20, 425, _T("- 동물 생성모드 :  자동/수동"));
	pDC->TextOutW(20, 450, _T("- 드론 조종모드 :  자동/수동"));
	pDC->TextOutW(20, 475, _T("- 농약 살포모드 :  살포/복귀"));
	pDC->TextOutW(20, 500, _T("- 기타 설정         :  이동속도, 동물생성주기, 농약살포예약"));
	pDC->TextOutW(10, 550, _T("* 드론 수동 조종방법"));
	pDC->TextOutW(20, 575, _T("- (방향키) 위         : 전진"));
	pDC->TextOutW(20, 600, _T("- (방향키) 아래     : 후진"));
	pDC->TextOutW(20, 625, _T("- (방향키) 왼쪽     : 반시계방향으로 회전"));
	pDC->TextOutW(20, 650, _T("- (방향키) 오른쪽 : 시계방향으로 회전"));

	//pDC->TextOutW(10, 10, str);
}


// CStaff_DronController 진단입니다.

#ifdef _DEBUG
void CStaff_DronController::AssertValid() const
{
	CView::AssertValid();
}

CStaff_DronSimulatorDoc* CStaff_DronController::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
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


// CStaff_DronController 메시지 처리기입니다.


void CStaff_DronController::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	m_ptMP2 = point;
	//Invalidate();

	CView::OnMouseMove(nFlags, point);
}


int CStaff_DronController::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	SetTimer(30, 30, NULL);

	return 0;
}


void CStaff_DronController::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
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
	CPoint *rslt, CPoint *rsrt, CPoint *rsl)	//확대출력을 위한 함수
{
	CStaff_DronSimulatorDoc* pDoc = GetDocument();
	// 배경의 너비가 1000, 높이가 700
	// 도화지비트맵의 너비는 2000, 높이는 1400
	// 도화지비트맵에 배경을 그리기 위한 왼쪽위(500, 350) 오른쪽위(1500, 350), 왼쪽아래(500, 1050) --1
	// 중심의 좌표는 (1000, 700)
	// 중심에서 배경 꼭지점까지의 거리는 약 610.32778 --2
	// 1과 2를 이용하여 배경 전체 돌리기
	// 그 후 도화지에 배경 돌려서 찍기(돌아간 배경꼭지점 좌표이용)
	// 드론의 slt, srt, sl좌표 도화지에 맞게 재구성 (x좌표 +500, y좌표 +350)
	// 드론의 slt, srt, sl과 중심과의 거리구하기
	// 드론의 slt, srt, sl도 같은각도로 회전(위에서 구한 거리 이용)
	// 500*275 도화지 새로 만들기
	// 

	//중심에서 배경 꼭지점 까지의 거리
	double bglen;
	bglen = 610.32778;

	//배경의 좌측상단, 우측상단, 좌측하단 좌표
	CPoint lt, rt, l;
	//회전후 좌표
	//CPoint rlt, rrt, rl;
	lt.x = 500; lt.y = 350;
	rt.x = 1500; rt.y = 350;
	l.x = 500; l.y = 1050;
	//각 점애대한 현재의 각도
	double lta, rta, la;
	lta = atan2(lt.y - 700, lt.x - 1000);
	rta = atan2(rt.y - 700, rt.x - 1000);
	la = atan2(l.y - 700, l.x - 1000);

	CPoint f;
	f = pDoc->m_Dron.GetPos();
	CPoint t;
	t = pDoc->m_Dron.GetSight();

	//회전할 각도를 정하는것
	double ang;
	ang = pDoc->m_Dron.GetAng();

	//출력할 부분의 좌측상단, 우측상단, 좌측하단 좌표
	CPoint slt, srt, sl;
	//회전후 좌표
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
	//각 점애대한 현재의 각도
	double slta, srta, sla;
	slta = atan2(slt.y - 700, slt.x - 1000);
	srta = atan2(srt.y - 700, srt.x - 1000);
	sla = atan2(sl.y - 700, sl.x - 1000);


	//출력할 부분의 좌표와 중심과의 거리
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
	ang = atan2(p2.y - p0.y, p2.x - p0.x);	//대상-중심 ( 대상과의 현재각도 파악)

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
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	return TRUE;

	//return CView::OnEraseBkgnd(pDC);
}
