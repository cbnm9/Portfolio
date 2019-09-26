
// Staff_DronSimulatorDoc.h : CStaff_DronSimulatorDoc 클래스의 인터페이스
//


#pragma once
#include "gdiplusheaders.h"
#include "CustObj.h"
#include "CustTime.h"
#include "DlgCtl.h"
#include "ATLComTime.h"

class CStaff_DronSimulatorDoc : public CDocument
{
protected: // serialization에서만 만들어집니다.
	CStaff_DronSimulatorDoc();
	DECLARE_DYNCREATE(CStaff_DronSimulatorDoc)

// 특성입니다.
public:

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 구현입니다.
public:
	virtual ~CStaff_DronSimulatorDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 검색 처리기에 대한 검색 콘텐츠를 설정하는 도우미 함수
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	Bitmap m_BitmapView;
	int m_ick;

	Bitmap* m_BitmapBat;
	CAnim m_Animal;
	CDron m_Dron;
	Bitmap* m_BitmapAnim;

	int m_iDronMode;
	int m_iCreateMode;
	int m_iAnimalMode;
	int m_iCtrlMode;
	int m_iAnimalSpeed;
	CPoint m_ptCenter;
	CRect m_rectBAT;
	CPoint m_ptMP;
	int m_iCreateSpeed;
	CRect m_rectSpace[7];
	int m_iAnimalNum;
	int m_iDronHomeMode;
	int m_iDronAttackMode;
	int m_iDronModeTemp;
	int m_iCursorMode;
	ImageAttributes ImgAttr;
	int m_iTog;
	CRect m_rectImg;

	Bitmap *m_BitmapG;
	Bitmap* m_BitmapR;
	Bitmap* m_BitmapV;

	CPoint m_ptPMV[20];
	CPoint m_ptPSP[20];
	int m_iDronPec;

	int m_iDronPecMode;
	Bitmap* m_BitmapPes;
	CString m_strPesTime;
	int m_iPesTimeMode;
	CDlgCtl m_dlgSet;
	COleDateTime m_tPesTime;
	int m_iDronPecModeTemp;
};