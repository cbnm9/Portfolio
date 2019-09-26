
// Staff_DronSimulatorView.h : CStaff_DronSimulatorView 클래스의 인터페이스
//

#pragma once
#include "CustObj.h"
#include "atltypes.h"
#include "gdiplusheaders.h"
#include "gdiplusimageattributes.h"
#include "DlgCtl.h"

class CStaff_DronSimulatorView : public CView
{
protected: // serialization에서만 만들어집니다.
	CStaff_DronSimulatorView();
	DECLARE_DYNCREATE(CStaff_DronSimulatorView)

// 특성입니다.
public:
	CStaff_DronSimulatorDoc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// 구현입니다.
public:
	virtual ~CStaff_DronSimulatorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnCreatmode();
	afx_msg void OnUpdateCreatmode(CCmdUI *pCmdUI);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnCtrlmode();
	afx_msg void OnUpdateCtrlmode(CCmdUI *pCmdUI);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPesmode();
	afx_msg void OnUpdatePesmode(CCmdUI *pCmdUI);

	void BM_GetSize(CPoint slt, CPoint srt, CPoint sl, double * wd, double * hg);
	void DronAct(UINT_PTR nIDEvent);
	void AnimalAct(UINT_PTR nIDEvent);
	CPoint RandomAnimal();
	int NumINC();

	/*int m_iDronMode;
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
	Bitmap* m_BitmapPes;*/
	afx_msg void OnStfset();
};

#ifndef _DEBUG  // Staff_DronSimulatorView.cpp의 디버그 버전
inline CStaff_DronSimulatorDoc* CStaff_DronSimulatorView::GetDocument() const
   { return reinterpret_cast<CStaff_DronSimulatorDoc*>(m_pDocument); }
#endif

