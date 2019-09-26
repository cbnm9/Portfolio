
// Staff_DronSimulatorView.cpp : CStaff_DronSimulatorView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "Staff_DronSimulator.h"
#endif

#include "Staff_DronSimulatorDoc.h"
#include "Staff_DronSimulatorView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "CustomDC.h"	//더블버퍼링을위한 헤더파일사용

/*
1. 다이얼로그 생성
4. 오른쪽 아래 칸에 설명서
*/

#define PI 3.14159265359

/* 기본타이머, 동물 자동생성 타이머 ID값 */
#define BASIC_TIMER 10
#define ANIMAL_TIMER 20

/* DRON */

/* 자동,수동 모드값 m_iCtrlMode, m_iCreateMode */
#define AUTO 0
#define NON_AUTO 1

/* 드론 모드값(동물, 농약, 복귀) m_iDronMode */
#define DRON_ANIMAL 0
#define DRON_PESTICIDE 1
#define DRON_BACK 2

/* DRON_ANIMAL 상태일때 (공격중, 공격아닐때) m_iDronAttackMode */
#define DRON_MOVE 0
#define DRON_ATTACK 1

/* DRON_PESTICIDE 상태일때 */


/* DRON_BACK 상태일때(이동중, 충전중) m_iDronHomeMode */
//#define DRON_MOVE 0 - 이미 정의되어 있음
#define DRON_CHARGING 1


/* ANIMAL */

/* 동물 오는중, 도망치는중 모드값 CAnim::Mode */
#define ANIMAL_COME 0
#define ANIMAL_RUN 1

/* ANIMAL_COME 인 동물이 목적지에 도착했을때의 모드값 CAnim::Mode2 */
#define ANIMAL_MOVE 0
#define ANIMAL_ARRIVE 1

/* ANIMAL_RUN 인 동물이 화면에서 사라질때의 모드값 CAnim::Mode2 */
//#define ANIMAL_MOVE 0 - 이미 정의되어있음
#define ANIMAL_DISAPPEAR 1

/* GDI+ 사용을 위한 전역변수 설정 */
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

// CStaff_DronSimulatorView 생성/소멸

CStaff_DronSimulatorView::CStaff_DronSimulatorView()
{
	// TODO: 여기에 생성 코드를 추가합니다.

	/* Gdi+ Start */
	GdiplusStartupInput GdiplusStartupInput;
	GdiplusStartup(&g_GdiPlusTokenStaff, &GdiplusStartupInput, NULL);

	/* 커서 초기화 */
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
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CStaff_DronSimulatorView 그리기

void CStaff_DronSimulatorView::OnDraw(CDC* nDC)
{
	CStaff_DronSimulatorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
	CCustomDC pDC(nDC);
	Graphics G(pDC);

	CString str1, str2;
	

	//배경
	Bitmap membm(1000, 700);
	Graphics mem(&membm);

	SolidBrush brs(Color(255, 255, 255));
	SolidBrush bra(Color(255, 0, 0));
	mem.FillRectangle(&brs, 0, 0, 1000, 700);

	Pen bpn(Color(0, 0, 0), 3);

	mem.DrawRectangle(&bpn, 40, 28, 920, 644);
	//pDC->Rectangle(m_rectBAT);	//울타리

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

	//농약 비트맵

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
	

	//드론
	CPen *pOldPen, NewPen;
	NewPen.CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
	
	pOldPen = pDC->SelectObject(&NewPen);
	pDC->MoveTo(pDoc->m_Dron.GetLB());
	pDC->LineTo(pDoc->m_Dron.GetRT());
	pDC->MoveTo(pDoc->m_Dron.GetLT());
	pDC->LineTo(pDoc->m_Dron.GetRB());
	pDC->SelectObject(pOldPen);
	//시야부분
	pDoc->m_Dron.SetSP();
	pDC->MoveTo(pDoc->m_Dron.GetPos());
	pDC->LineTo(pDoc->m_Dron.GetSl());
	pDC->MoveTo(pDoc->m_Dron.GetPos());
	pDC->LineTo(pDoc->m_Dron.GetSr());
	pDC->MoveTo(pDoc->m_Dron.GetPos());
	pDC->LineTo(pDoc->m_Dron.GetSlt());
	pDC->MoveTo(pDoc->m_Dron.GetPos());
	pDC->LineTo(pDoc->m_Dron.GetSrt());
	//좌상날개
	pDC->MoveTo(pDoc->m_Dron.GetLTW(1));
	pDC->LineTo(pDoc->m_Dron.GetLTW(-1));
	//우상날개
	pDC->MoveTo(pDoc->m_Dron.GetRTW(1));
	pDC->LineTo(pDoc->m_Dron.GetRTW(-1));
	//좌하날개
	pDC->MoveTo(pDoc->m_Dron.GetLBW(1));
	pDC->LineTo(pDoc->m_Dron.GetLBW(-1));
	//우하날개
	pDC->MoveTo(pDoc->m_Dron.GetRBW(1));
	pDC->LineTo(pDoc->m_Dron.GetRBW(-1));

	pDC->Ellipse(pDoc->m_Dron.GetRect());

	

	//비트맵
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
	
	//커서 깜빡임 해소필요
	

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
		str1.Format(_T("조종모드 : 자동"));
	}
	else
	{
		str1.Format(_T("조종모드 : 수동"));
	}

	if (pDoc->m_iCreateMode == AUTO)
	{
		str2.Format(_T("동물생성 : 자동"));
	}
	else
	{
		str2.Format(_T("동물생성 : 수동"));
	}

	pDC->TextOutW(10, 10, str1);
	pDC->TextOutW(150, 10, str2);

	//str.Format(_T("(%d, %d)"), pDoc->m_ptMP.x, pDoc->m_ptMP.y);
	//pDC->TextOutW(10, 10, str);

	//str.Format(_T("dgr %lf"), pDoc->m_Dron.GetAng() * 180 / PI);
	//pDC->TextOutW(10, 30, str);

	//str.Format(_T("갯수 %d"), pDoc->m_dlgSet.m_iInx);
	//pDC->TextOutW(10, 50, str);

	//str.Format(pDoc->m_strPesTime);
	//pDC->TextOutW(10, 70, str);
}


// CStaff_DronSimulatorView 진단

#ifdef _DEBUG
void CStaff_DronSimulatorView::AssertValid() const
{
	CView::AssertValid();
}

void CStaff_DronSimulatorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CStaff_DronSimulatorDoc* CStaff_DronSimulatorView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CStaff_DronSimulatorDoc)));
	return (CStaff_DronSimulatorDoc*)m_pDocument;
}
#endif //_DEBUG


// CStaff_DronSimulatorView 메시지 처리기---------------------------------------------------------------------------------------- 메세지 처리기


int CStaff_DronSimulatorView::OnCreate(LPCREATESTRUCT lpCreateStruct)	//---------------------------------------------------- 기본 타이머 초기화(OnCreate)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

	srand((unsigned)time(0));
	SetTimer(BASIC_TIMER, 30, NULL);

	return 0;
}


void CStaff_DronSimulatorView::OnCreatmode()							//---------------------------------------------------- 메뉴(동물생성방식)
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CStaff_DronSimulatorDoc* pDoc = GetDocument();

	switch (pDoc->m_iCreateMode)
	{
	case AUTO:
		pDoc->m_iCreateMode = NON_AUTO;
		KillTimer(ANIMAL_TIMER);
		pDoc->m_iCursorMode = 1;
		AfxMessageBox(_T("수동 생성모드로 변경되었습니다."));
		break;
	case NON_AUTO:
		pDoc->m_iCreateMode = AUTO;
		SetTimer(ANIMAL_TIMER, pDoc->m_iCreateSpeed, NULL);
		pDoc->m_iCursorMode = 0;
		AfxMessageBox(_T("자동 생성모드로 변경되었습니다."));
		break;
	}
}


void CStaff_DronSimulatorView::OnUpdateCreatmode(CCmdUI *pCmdUI)		//---------------------------------------------------- 메뉴업데이트(동물생성방식)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	CStaff_DronSimulatorDoc* pDoc = GetDocument();

	switch (pDoc->m_iCreateMode)
	{
	case AUTO:
		pCmdUI->SetText(_T("수동"));
		break;
	case NON_AUTO:
		pCmdUI->SetText(_T("자동"));
		break;
	}
}


void CStaff_DronSimulatorView::OnMouseMove(UINT nFlags, CPoint point)	//---------------------------------------------------- 마우스 이동
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CStaff_DronSimulatorDoc* pDoc = GetDocument();
	pDoc->m_ptMP = point;

	CView::OnMouseMove(nFlags, point);
}

void CStaff_DronSimulatorView::OnLButtonDown(UINT nFlags, CPoint point)	//---------------------------------------------------- 마우스 클릭
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CStaff_DronSimulatorDoc* pDoc = GetDocument();

	switch (pDoc->m_iCreateMode)
	{
	case NON_AUTO:			//생성모드가 수동일때 마우스 왼쪽 버튼을 누른다면

		CAnim *tp;
		int id = rand() % 3 + 1;
		tp = new CAnim(point, pDoc->m_iAnimalSpeed, id);

		pDoc->m_Animal.Ins(tp);

		break;
	}
	pDoc->m_ptMP = point;
	CView::OnLButtonDown(nFlags, point);
}

BOOL CStaff_DronSimulatorView::OnEraseBkgnd(CDC* pDC)	//------------------------------------------------------------------- 화면 지우는 메세지
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	
	return TRUE;

	//return CView::OnEraseBkgnd(pDC);
}

void CStaff_DronSimulatorView::OnCtrlmode()	//------------------------------------------------------------------------------ 메뉴의 드론 조종모드
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CStaff_DronSimulatorDoc* pDoc = GetDocument();

	switch (pDoc->m_iCtrlMode)
	{
	case AUTO:
		pDoc->m_iCtrlMode = NON_AUTO;
		AfxMessageBox(_T("수동 조종모드로 변경되었습니다."));
		break;
	case NON_AUTO:
		pDoc->m_iCtrlMode = AUTO;
		AfxMessageBox(_T("자동 조종모드로 변경되었습니다."));
		break;
	}

}


void CStaff_DronSimulatorView::OnUpdateCtrlmode(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	CStaff_DronSimulatorDoc* pDoc = GetDocument();

	switch (pDoc->m_iCtrlMode)
	{
	case AUTO:
		pCmdUI->SetText(_T("수동"));
		break;
	case NON_AUTO:
		pCmdUI->SetText(_T("자동"));
		break;
	}

}

void CStaff_DronSimulatorView::OnPesmode()	//----------------------------------------------------------------------------- 메뉴의 농약살포모드
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
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	CStaff_DronSimulatorDoc* pDoc = GetDocument();

	if (pDoc->m_iDronMode == DRON_PESTICIDE)
	{
		pCmdUI->SetText(_T("복귀"));
	}
	else
	{
		pCmdUI->SetText(_T("농약살포"));
	}
}

void CStaff_DronSimulatorView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)	//----------------------------------------- 수동조종을 위한 키보드 메세지
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

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
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.


	//pDoc->m_iDronMode = DRON_PESTICIDE;

	CView::OnRButtonDown(nFlags, point);
}

void CStaff_DronSimulatorView::OnStfset()	//--------------------------------------------------------------------------------- 설정메뉴 열기
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
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

CPoint CStaff_DronSimulatorView::RandomAnimal()							//----------------------------------------------------- 동물 자동생성 위치 랜덤하게 정해주는함수
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

int CStaff_DronSimulatorView::NumINC()									//---------------------------------------------------- 동물 생성시 고유번호 부여할때 쓰는 함수(안씀?)
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
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CStaff_DronSimulatorDoc* pDoc = GetDocument();


	DronAct(nIDEvent);		//드론에 관한부분(View의 멤버함수)

	AnimalAct(nIDEvent);	//동물에 관한부분


								/* 공통된 부분만 이곳에 작성 */

	//==============================================================================================================
	// 동물과 드론이 마주치면 동물은 도망, 드론은 공격
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
			pDoc->m_dlgSet.m_strFirst.Format(_T("변경"));
		}
	}

	if (pDoc->m_iCreateMode == NON_AUTO)
		KillTimer(ANIMAL_TIMER);

	//Invalidate(FALSE);
	pDoc->UpdateAllViews(NULL);

	CView::OnTimer(nIDEvent);
}

void CStaff_DronSimulatorView::DronAct(UINT_PTR nIDEvent)		//---------------------------------------------------------------------- 드론 움직임제어
{
	CStaff_DronSimulatorDoc* pDoc = GetDocument();

	switch (pDoc->m_iCtrlMode)
	{
	case AUTO:	//자동모드

		//DronMode 결정
		//===============================================
		
		{
			CAnim *p, *t;
			p = pDoc->m_Animal.GetFp();
			t = NULL;
			int ck = INT_MAX;		//ck는 기본적으로 최대값으로 설정
			int ln;
			while (p != &(pDoc->m_Animal))
			{
				if (pDoc->m_rectBAT.PtInRect(p->GetPos()))	//동물이 있으면 동물쪽으로 드론이동
				{
					ln = (p->GetPos().x)*(p->GetPos().x) + (p->GetPos().y)*(p->GetPos().y);	//비교를 위한 동물과 드론간의 거리
					if (ln < ck)	//만약 거리가 가깝다면 그 동물 선택
					{
						ck = ln;
						t = p;
					}
				}

				p = p->GetFp();
			}

			if (ck == INT_MAX)		//동물이 울타리 내에 없다면
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
					pDoc->m_iDronMode = pDoc->m_iDronModeTemp;	//이전에 하던 드론모드 수행
					break;
				case DRON_PESTICIDE:
					break;
				}
			}
			else					//동물이 울타리 내에 있다면, 가장 가까운 동물 = t
			{
				pDoc->m_Dron.SetChase(t->GetPos());	//드론의 목적지(따라가는 동물의 좌표) 설정
				pDoc->m_iDronMode = DRON_ANIMAL;		//드론의 동물추적모드
				if (pDoc->m_iDronPecMode == 1)
				{
					pDoc->m_iDronPecModeTemp = 1;
					pDoc->m_iDronPecMode = 0;
				}
			}
		}
		
		//==============================================

		// 드론 HOME 모드 설정 (집 떠나 있으면 움직여야됨)
		//==============================================
		if (pDoc->m_Dron.GetPos() != pDoc->m_ptCenter)
		{
			pDoc->m_iDronHomeMode = DRON_MOVE;
		}

		// 결정된 DronMode에 대한 드론의 움직임
		//==============================================
		switch (pDoc->m_iDronMode)
		{
		case DRON_ANIMAL:

			//기본적으로 동물쪽으로 이동
			{
			CPoint ps;
			ps = pDoc->m_Dron.GetChase();
			pDoc->m_Dron.SetSight(ps, 0);
			}
			pDoc->m_Dron.Move();

			//동물을 공격하는 상태 조절(이동만함, 공격도함-빛)
			switch (pDoc->m_iDronAttackMode)
			{
			case DRON_MOVE:
				break;
			case DRON_ATTACK:
				break;
			}

			pDoc->m_Dron.Wing();		//드론 날개짓
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
			case DRON_MOVE:		//집에 도착할때까지 움직임
				pDoc->m_Dron.SetSight(pDoc->m_ptCenter, 0);
				pDoc->m_Dron.Move();

				if (CRect(475, 330, 515, 370).PtInRect(pDoc->m_Dron.GetPos()))	//중앙 근처에 도착하면 중앙으로 값 변경, 드론 충전모드
				{
					pDoc->m_Dron.SetPosition(495, 350);
					pDoc->m_iDronHomeMode = DRON_CHARGING;
				}

				pDoc->m_Dron.Wing();
				break;
			case DRON_CHARGING:	//집에 도착하면 안움직임
				break;
			}

			break;
		}

		break;
	case NON_AUTO:	//수동조작모드

		pDoc->m_Dron.Wing();
		break;
	}
}

void CStaff_DronSimulatorView::AnimalAct(UINT_PTR nIDEvent)		//-------------------------------------------------------------- 동물 움직임 제어
{
	CStaff_DronSimulatorDoc* pDoc = GetDocument();

	if (pDoc->m_iCtrlMode == AUTO)//==================================================================twefwe
	{
		switch (nIDEvent)	//동물 자동생성(Timer가 있을때)
		{
		case ANIMAL_TIMER:
			CAnim *tp;
			int id = rand() % 3 + 1;
			tp = new CAnim(RandomAnimal(), pDoc->m_iAnimalSpeed, id);

			pDoc->m_Animal.Ins(tp);
			break;
		}
	}
	

	CAnim *p, *d;	//d:삭제를 위한 포인터변수
	d = NULL;
	int del=0;		//삭제를 위한 조건검사 변수
	p = pDoc->m_Animal.GetFp();
	while (p != &(pDoc->m_Animal))
	{
		switch (p->GetMode())
		{
		case ANIMAL_COME:	//동물은 Come 모드에서 시작(기본 값)

			switch (p->GetMode2())
			{
			case ANIMAL_MOVE:
				p->SetSight(p->GetDstn(), 0);
				p->Move();

				//목적지 근처에 도착하면 목적지로 위치고정 --> 위치고정 하지않고 좌우로 진동하게 하는것이 좋은것같음
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

			//Run 일때 모드2값 결정(화면 밖으로 나가면 없어짐)
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

/*	모든 동물 검색하는 방법

CAnim *p;
p = m_Animal.GetFp();
while (p != &m_Animal)
{


p = p->GetFp();
}

*/

