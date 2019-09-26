#pragma once

#include "stdafx.h"

class CCustTime {
public:
	//변수
	CString m_time;
	CCustTime *fp;
	CCustTime *rp;
public:
	//함수
	CCustTime() {}
	CCustTime(CString);

	void ins(CCustTime *);
	void del(CString);
	void alldel();

};