
// Staff_DronSimulatorDoc.h : CStaff_DronSimulatorDoc Ŭ������ �������̽�
//


#pragma once
#include "gdiplusheaders.h"
#include "CustObj.h"
#include "CustTime.h"
#include "DlgCtl.h"
#include "ATLComTime.h"

class CStaff_DronSimulatorDoc : public CDocument
{
protected: // serialization������ ��������ϴ�.
	CStaff_DronSimulatorDoc();
	DECLARE_DYNCREATE(CStaff_DronSimulatorDoc)

// Ư���Դϴ�.
public:

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// �����Դϴ�.
public:
	virtual ~CStaff_DronSimulatorDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// �˻� ó���⿡ ���� �˻� �������� �����ϴ� ����� �Լ�
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