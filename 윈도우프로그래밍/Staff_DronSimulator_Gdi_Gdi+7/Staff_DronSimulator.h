
// Staff_DronSimulator.h : Staff_DronSimulator ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.


// CStaff_DronSimulatorApp:
// �� Ŭ������ ������ ���ؼ��� Staff_DronSimulator.cpp�� �����Ͻʽÿ�.
//

class CStaff_DronSimulatorApp : public CWinApp
{
public:
	CStaff_DronSimulatorApp();


// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CStaff_DronSimulatorApp theApp;
