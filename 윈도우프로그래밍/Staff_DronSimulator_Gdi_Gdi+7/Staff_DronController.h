#pragma once
#include "CustObj.h"
#include "atltypes.h"
#include "gdiplusheaders.h"
#include "gdiplusimageattributes.h"
#include "Staff_DronSimulatorView.h"

// CStaff_DronController 뷰입니다.

class CStaff_DronController : public CView
{
	DECLARE_DYNCREATE(CStaff_DronController)

protected:
	CStaff_DronController();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CStaff_DronController();

public:
	virtual void OnDraw(CDC* pDC);      // 이 뷰를 그리기 위해 재정의되었습니다.
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	CPoint m_ptMP2;
	CStaff_DronSimulatorDoc* GetDocument() const;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void ObserveFunc(CPoint *rlt, CPoint *rrt, CPoint *rl,
		CPoint *rslt, CPoint *rsrt, CPoint *rsl);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};


#ifndef _DEBUG  // Staff_DronSimulatorView.cpp의 디버그 버전
inline CStaff_DronSimulatorDoc* CStaff_DronController::GetDocument() const
{
	return reinterpret_cast<CStaff_DronSimulatorDoc*>(m_pDocument);
}

#endif