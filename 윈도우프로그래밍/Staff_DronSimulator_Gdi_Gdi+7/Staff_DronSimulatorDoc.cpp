
// Staff_DronSimulatorDoc.cpp : CStaff_DronSimulatorDoc Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
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

// CStaff_DronSimulatorDoc ����/�Ҹ�

/* GDI+ ����� ���� �������� ���� */
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
	// TODO: ���⿡ ��ȸ�� ���� �ڵ带 �߰��մϴ�.
	/* Gdi+ Start */
	GdiplusStartupInput GdiplusStartupInput;
	GdiplusStartup(&g_GdiPlusTokenStaff3, &GdiplusStartupInput, NULL);

	/* ��Ÿ�� �ʱ�ȭ */
	m_rectBAT.SetRect(40, 28, 960, 672);	//��Ÿ��
	m_rectImg.SetRect(50, 38, 950, 662);	//�� �̹���

	/* �� Rect������ �ʱ�ȭ */
	/*
	m_rectSpace[0].SetRect(100, 100, 300, 300);
	m_rectSpace[1].SetRect(400, 100, 450, 300);
	m_rectSpace[2].SetRect(500, 100, 550, 300);
	m_rectSpace[3].SetRect(600, 100, 650, 300);
	m_rectSpace[4].SetRect(700, 100, 750, 300);
	m_rectSpace[5].SetRect(500, 450, 800, 600);
	m_rectSpace[6].SetRect(100, 400, 400, 600);
	*/


	//���� �ʱ�ȭ
	m_Animal.Ins(NULL);

	//��Ʈ�� �ʱ�ȭ
	m_BitmapBat = Bitmap::FromFile(L"BAT.bmp");
	m_BitmapAnim = Bitmap::FromFile(L"AnimCursor.bmp");
	m_BitmapG = Bitmap::FromResource(AfxGetInstanceHandle(), (WCHAR*)MAKEINTRESOURCE(IDB_BITDRON));
	m_BitmapR = Bitmap::FromResource(AfxGetInstanceHandle(), (WCHAR*)MAKEINTRESOURCE(IDB_BITMAPRED));
	m_BitmapPes = Bitmap::FromResource(AfxGetInstanceHandle(), (WCHAR*)MAKEINTRESOURCE(IDB_BITMAPPES));

	//��� �ʱ�ȭ
	m_Dron.SetSpeed(16);
	m_Dron.SetPosition(495, 350);
	CPoint pt(495, 250);
	m_Dron.SetSight(pt, 0);
	m_Dron.SetSize(25);
	m_Dron.SetLngs(100);

	// ���Ѹ��� ��ǥ �ʱ�ȭ
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

	// TODO: ���⿡ ���ʱ�ȭ �ڵ带 �߰��մϴ�.
	// SDI ������ �� ������ �ٽ� ����մϴ�.
	
	//���� �ʱ�ȭ
	m_Animal.alldel();

	//��� �ʱ�ȭ
	m_Dron.SetSpeed(16);
	m_Dron.SetPosition(495, 350);
	CPoint pt(495, 250);
	m_Dron.SetSight(pt, 0);
	m_Dron.SetSize(25);
	m_Dron.SetLngs(100);

	//���� �ʱ�ȭ
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
		// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	}
	else
	{
		// TODO: ���⿡ �ε� �ڵ带 �߰��մϴ�.
	}
}

#ifdef SHARED_HANDLERS

// ����� �׸��� �����մϴ�.
void CStaff_DronSimulatorDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// ������ �����͸� �׸����� �� �ڵ带 �����Ͻʽÿ�.
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

// �˻� ó���⸦ �����մϴ�.
void CStaff_DronSimulatorDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// ������ �����Ϳ��� �˻� �������� �����մϴ�.
	// ������ �κ��� ";"�� ���еǾ�� �մϴ�.

	// ��: strSearchContent = _T("point;rectangle;circle;ole object;");
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

// CStaff_DronSimulatorDoc ����

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


// CStaff_DronSimulatorDoc ���
