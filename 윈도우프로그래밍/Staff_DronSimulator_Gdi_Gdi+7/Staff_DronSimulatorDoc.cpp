
// Staff_DronSimulatorDoc.cpp : CStaff_DronSimulatorDoc 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "Staff_DronSimulator.h"
#endif

#include "Staff_DronSimulatorDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CStaff_DronSimulatorDoc

IMPLEMENT_DYNCREATE(CStaff_DronSimulatorDoc, CDocument)

BEGIN_MESSAGE_MAP(CStaff_DronSimulatorDoc, CDocument)
END_MESSAGE_MAP()

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

// CStaff_DronSimulatorDoc 생성/소멸

/* GDI+ 사용을 위한 전역변수 설정 */
ULONG_PTR g_GdiPlusTokenStaff3;

CStaff_DronSimulatorDoc::CStaff_DronSimulatorDoc()
	: m_ick(0)
	, m_BitmapView(100, 55)
	, m_BitmapAnim(NULL)
	, m_iDronMode(DRON_ANIMAL)
	, m_iCreateMode(NON_AUTO)
	, m_iAnimalMode(0)
	, m_iCtrlMode(AUTO)
	, m_iAnimalSpeed(8)
	, m_ptCenter(495, 350)
	, m_ptMP(0)
	, m_iCreateSpeed(6000)
	, m_iAnimalNum(0)
	, m_iDronHomeMode(DRON_CHARGING)
	, m_iDronAttackMode(DRON_MOVE)
	, m_iDronModeTemp(DRON_BACK)
	, m_iCursorMode(0)
	, m_BitmapR(NULL)
	, m_iTog(0)
	, m_iDronPec(0)
	, m_iDronPecMode(0)
	, m_BitmapPes(NULL)
	, m_strPesTime(_T(""))
	, m_iPesTimeMode(0)
	, m_tPesTime(COleDateTime::GetCurrentTime())
	, m_iDronPecModeTemp(0)
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.
	/* Gdi+ Start */
	GdiplusStartupInput GdiplusStartupInput;
	GdiplusStartup(&g_GdiPlusTokenStaff3, &GdiplusStartupInput, NULL);

	/* 울타리 초기화 */
	m_rectBAT.SetRect(40, 28, 960, 672);	//울타리
	m_rectImg.SetRect(50, 38, 950, 662);	//밭 이미지

	/* 밭 Rect변수로 초기화 */
	/*
	m_rectSpace[0].SetRect(100, 100, 300, 300);
	m_rectSpace[1].SetRect(400, 100, 450, 300);
	m_rectSpace[2].SetRect(500, 100, 550, 300);
	m_rectSpace[3].SetRect(600, 100, 650, 300);
	m_rectSpace[4].SetRect(700, 100, 750, 300);
	m_rectSpace[5].SetRect(500, 450, 800, 600);
	m_rectSpace[6].SetRect(100, 400, 400, 600);
	*/


	//동물 초기화
	m_Animal.Ins(NULL);

	//비트맵 초기화
	m_BitmapBat = Bitmap::FromFile(L"BAT.bmp");
	m_BitmapAnim = Bitmap::FromFile(L"AnimCursor.bmp");
	m_BitmapG = Bitmap::FromResource(AfxGetInstanceHandle(), (WCHAR*)MAKEINTRESOURCE(IDB_BITDRON));
	m_BitmapR = Bitmap::FromResource(AfxGetInstanceHandle(), (WCHAR*)MAKEINTRESOURCE(IDB_BITMAPRED));
	m_BitmapPes = Bitmap::FromResource(AfxGetInstanceHandle(), (WCHAR*)MAKEINTRESOURCE(IDB_BITMAPPES));

	//드론 초기화
	m_Dron.SetSpeed(16);
	m_Dron.SetPosition(495, 350);
	CPoint pt(495, 250);
	m_Dron.SetSight(pt, 0);
	m_Dron.SetSize(25);
	m_Dron.SetLngs(100);

	// 농약뿌리는 좌표 초기화
	m_ptPMV[0] = CPoint(540, 600);	m_ptPMV[1] = CPoint(820, 630);	m_ptPMV[2] = CPoint(750, 450);	m_ptPMV[3] = CPoint(940, 410);	m_ptPMV[4] = CPoint(930, 170);
	m_ptPMV[5] = CPoint(740, 190);	m_ptPMV[6] = CPoint(800, 350);	m_ptPMV[7] = CPoint(560, 310);	m_ptPMV[8] = CPoint(580, 480);	m_ptPMV[9] = CPoint(300, 470);
	m_ptPMV[10] = CPoint(310, 640);	m_ptPMV[11] = CPoint(180, 470);	m_ptPMV[12] = CPoint(190, 320);	m_ptPMV[13] = CPoint(410, 330);	m_ptPMV[14] = CPoint(420, 180);
	m_ptPMV[15] = CPoint(650, 170);	m_ptPMV[16] = CPoint(770, 100);	m_ptPMV[17] = CPoint(390, 130);	m_ptPMV[18] = CPoint(170, 100);	m_ptPMV[19] = CPoint(120, 290);

	m_ptPSP[0] = CPoint(630, 650);	m_ptPSP[1] = CPoint(650, 520);	m_ptPSP[2] = CPoint(920, 550);	m_ptPSP[3] = CPoint(880, 360);	m_ptPSP[4] = CPoint(870, 110);
	m_ptPSP[5] = CPoint(880, 280);	m_ptPSP[6] = CPoint(650, 250);	m_ptPSP[7] = CPoint(710, 410);	m_ptPSP[8] = CPoint(430, 390);	m_ptPSP[9] = CPoint(450, 570);
	m_ptPSP[10] = CPoint(180, 560);	m_ptPSP[11] = CPoint(80, 400);	m_ptPSP[12] = CPoint(300, 400);	m_ptPSP[13] = CPoint(300, 250);	m_ptPSP[14] = CPoint(540, 250);
	m_ptPSP[15] = CPoint(540, 100);	m_ptPSP[16] = CPoint(680, 50);	m_ptPSP[17] = CPoint(300, 70);	m_ptPSP[18] = CPoint(290, 200);	m_ptPSP[19] = CPoint(60, 240);
}

CStaff_DronSimulatorDoc::~CStaff_DronSimulatorDoc()
{
	/* Gdi+ Shutdown */
	GdiplusShutdown(g_GdiPlusTokenStaff3);
}

BOOL CStaff_DronSimulatorDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.
	
	//동물 초기화
	m_Animal.alldel();

	//드론 초기화
	m_Dron.SetSpeed(16);
	m_Dron.SetPosition(495, 350);
	CPoint pt(495, 250);
	m_Dron.SetSight(pt, 0);
	m_Dron.SetSize(25);
	m_Dron.SetLngs(100);

	//변수 초기화
	m_ick = 0;
	m_iDronMode = DRON_ANIMAL;
	m_iCreateMode = NON_AUTO;
	m_iAnimalMode = 0;
	m_iCtrlMode = AUTO;
	m_iAnimalSpeed = 8;
	m_iCreateSpeed = 6000;
	m_iAnimalNum = 0;
	m_iDronHomeMode = DRON_CHARGING;
	m_iDronAttackMode = DRON_MOVE;
	m_iDronModeTemp = DRON_BACK;
	m_iCursorMode = 0;
	m_iTog = 0;
	m_iDronPec = 0;
	m_iDronPecMode = 0;
	m_strPesTime.Format(_T(""));
	m_iPesTimeMode = 0;
	m_dlgSet.m_ctTime.alldel();
	m_dlgSet.m_iGen = 6000;
	m_dlgSet.m_iSpd = 8;

	return TRUE;
}




// CStaff_DronSimulatorDoc serialization

void CStaff_DronSimulatorDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
	}
}

#ifdef SHARED_HANDLERS

// 축소판 그림을 지원합니다.
void CStaff_DronSimulatorDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 문서의 데이터를 그리려면 이 코드를 수정하십시오.
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 검색 처리기를 지원합니다.
void CStaff_DronSimulatorDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 문서의 데이터에서 검색 콘텐츠를 설정합니다.
	// 콘텐츠 부분은 ";"로 구분되어야 합니다.

	// 예: strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CStaff_DronSimulatorDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CStaff_DronSimulatorDoc 진단

#ifdef _DEBUG
void CStaff_DronSimulatorDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CStaff_DronSimulatorDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CStaff_DronSimulatorDoc 명령
