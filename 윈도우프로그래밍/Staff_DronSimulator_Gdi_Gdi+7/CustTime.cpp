
#include "stdafx.h"

#include "CustTime.h"

CCustTime::CCustTime(CString str)
{
	m_time = str;
}

void CCustTime::ins(CCustTime *time)
{
	if (time == NULL)	//매개변수로 NULL이 오면 더미노드 초기화작업
	{
		rp = this;
		fp = this;
		m_time.Format(_T(""));
	}
	else
	{
		time->rp = this;
		time->fp = fp;
		fp->rp = time;
		fp = time;
	}
}

void CCustTime::del(CString str)
{
	CCustTime *p;
	p = fp;
	while (p != this)
	{
		if (p->m_time == str)
			break;

		p = p->fp;
	}

	p->fp->rp = p->rp;
	p->rp->fp = p->fp;
	delete p;
}

void CCustTime::alldel()
{
	CCustTime *p;
	CCustTime *t;
	p = fp;
	while (p != this)
	{
		t = p;
		p = p->fp;
		delete t;
	}

	fp = this;
	rp = this;
}

