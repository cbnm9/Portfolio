#pragma once

#include "stdafx.h"

class CCustTime {
public:
	//����
	CString m_time;
	CCustTime *fp;
	CCustTime *rp;
public:
	//�Լ�
	CCustTime() {}
	CCustTime(CString);

	void ins(CCustTime *);
	void del(CString);
	void alldel();

};