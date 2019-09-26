

#include "stdafx.h"

#include "CustObj.h"

#define PI 3.14159265359

/* Obj 최상위클래스 */

void CCustObj::SetPosition(int x, int y)
{
	p0.x = x;
	p0.y = y;
}

void CCustObj::SetSpeed(int spd)
{
	speed = spd;
}

void CCustObj::SetSight(CPoint p2, int mode)	//mode=1 -> 반대방향 향하기
{
	ang = atan2(p2.y - p0.y, p2.x - p0.x);

	if (mode == 1)
	{
		ang = ang - PI;
	}

	p1.x = (int)(speed*cos(ang)) + p0.x;
	p1.y = (int)(speed*sin(ang)) + p0.y;

}

void CCustObj::Move()
{
	p0.x = p1.x;
	p0.y = p1.y;

	p1.x = (int)(speed*cos(ang)) + p0.x;
	p1.y = (int)(speed*sin(ang)) + p0.y;
}

/* 드론 */
/* [L|R|B]Move는 View에서 구현하는게 좋을듯함 */
CDron::CDron()
	:degree(0)
{}

void CDron::SetSize(int sz)
{
	size = sz;
}

void CDron::SetChase(CPoint cs)
{
	chase = cs;
}

void CDron::SetLngs(int ln)
{
	lngs = ln;
}

void CDron::SetSP()
{
	double kang = 30 * PI / 180;

	double tang = ang + kang;

	sr.x = (int)(lngs*cos(tang)) + p0.x;
	sr.y = (int)(lngs*sin(tang)) + p0.y;

	tang = ang - kang;

	sl.x = (int)(lngs*cos(tang)) + p0.x;
	sl.y = (int)(lngs*sin(tang)) + p0.y;

	tang = atan2(sr.y - sl.y, sr.x - sl.x);

	tang = tang - 90 * PI / 180;

	slt.x = (int)((lngs - 45)*cos(tang)) + sl.x;
	slt.y = (int)((lngs - 45)*sin(tang)) + sl.y;

	tang = atan2(sl.y - sr.y, sl.x - sr.x);

	tang = tang + 90 * PI / 180;

	srt.x = (int)((lngs - 45)*cos(tang)) + sr.x;
	srt.y = (int)((lngs - 45)*sin(tang)) + sr.y;
}

void CDron::SetSight2(double ang1)
{
	ang = ang + ang1;

	p1.x = (int)(speed*cos(ang)) + p0.x;
	p1.y = (int)(speed*sin(ang)) + p0.y;
}

int CDron::GetSize() const
{
	return size;
}

CPoint CDron::GetPos() const
{
	return p0;
}

CPoint CDron::GetSight() const
{
	return p1;
}

double CDron::GetAng() const
{
	return ang;
}

CPoint CDron::GetChase() const
{
	return chase;
}

CPoint CDron::GetSl() const
{
	return sl;
}

CPoint CDron::GetSr() const
{
	return sr;
}

CPoint CDron::GetSlt() const
{
	return slt;
}

CPoint CDron::GetSrt() const
{
	return srt;
}

CPoint CDron::GetLT() const
{
	int x = p0.x - size;
	int y = p0.y - size;

	CPoint pt(x, y);

	return pt;
}

CPoint CDron::GetRB() const
{
	int x = p0.x + size;
	int y = p0.y + size;

	CPoint pt(x, y);

	return pt;
}

CPoint CDron::GetRT() const
{
	int x = p0.x + size;
	int y = p0.y - size;

	CPoint pt(x, y);

	return pt;
}

CPoint CDron::GetLB() const
{
	int x = p0.x - size;
	int y = p0.y + size;

	CPoint pt(x, y);

	return pt;
}

CRect CDron::GetRect() const
{
	int x1 = p0.x - size*0.5;
	int y1 = p0.y - size*0.5;

	int x2 = p0.x + size*0.5;
	int y2 = p0.y + size*0.5;

	CRect rt;
	rt.SetRect(x1, y1, x2, y2);

	return rt;
}

CPoint CDron::GetLTW(int n) const
{
	double radian = degree *3.141592 / 180;

	int x = p0.x - size + n*int(sin(radian) * size * 0.5);
	int y = p0.y - size + n*int(cos(radian) * size * 0.5);
	
	CPoint pt(x, y);

	return pt;
}

CPoint CDron::GetRTW(int n) const
{
	double radian = degree *3.141592 / 180;

	int x = p0.x + size + n*int(sin(radian) * size * 0.5);
	int y = p0.y - size + n*int(cos(radian) * size * 0.5);

	CPoint pt(x, y);

	return pt;
}

CPoint CDron::GetLBW(int n) const
{
	double radian = degree *3.141592 / 180;

	int x = p0.x - size + n*int(sin(radian) * size * 0.5);
	int y = p0.y + size + n*int(cos(radian) * size * 0.5);

	CPoint pt(x, y);

	return pt;
}

CPoint CDron::GetRBW(int n) const
{
	double radian = degree *3.141592 / 180;

	int x = p0.x + size + n*int(sin(radian) * size * 0.5);
	int y = p0.y + size + n*int(cos(radian) * size * 0.5);

	CPoint pt(x, y);

	return pt;
}

void CDron::Wing()
{
	degree = (degree + 72) % 360;
}

void CDron::LMove()
{
	double angt = ang;

	double ang1 = ang - (90 * PI / 180);

	p1.x = (int)(speed*cos(ang1)) + p0.x;
	p1.y = (int)(speed*sin(ang1)) + p0.y;

	Move();

	p1.x = (int)(speed*cos(angt)) + p0.x;
	p1.y = (int)(speed*sin(angt)) + p0.y;
}

void CDron::RMove()
{
	double angt = ang;

	double ang1 = ang + (90 * PI / 180);

	p1.x = (int)(speed*cos(ang1)) + p0.x;
	p1.y = (int)(speed*sin(ang1)) + p0.y;

	Move();

	p1.x = (int)(speed*cos(angt)) + p0.x;
	p1.y = (int)(speed*sin(angt)) + p0.y;
}

void CDron::BMove()
{
	double angt = ang;

	double ang1 = ang - PI;

	p1.x = (int)(speed*cos(ang1)) + p0.x;
	p1.y = (int)(speed*sin(ang1)) + p0.y;

	Move();

	p1.x = (int)(speed*cos(angt)) + p0.x;
	p1.y = (int)(speed*sin(angt)) + p0.y;
}

/* 동물 */

CAnim::CAnim(CPoint pt, int spd, int id)
	:ID(id)
{
	SetPosition(pt.x, pt.y);
	SetSpeed(spd);
	RandomDstn();
	SetSight(dstn, 0);
	Mode = 0;
	Mode2 = 0;
}

void CAnim::Ins(CAnim *na)	//rp에 삽입
{
	if (na == NULL)	//매개변수로 NULL이 오면 더미노드 초기화작업
	{
		rp = this;
		fp = this;
		dstnID = 0;
	}
	else
	{
		na->rp = rp;
		na->fp = rp->fp;
		rp->fp = na;
		rp = na;
	}
}

void CAnim::del()			//fp 삭제(선입선출)
{
	if (fp != this)
	{
		rp->fp = fp;
		fp->rp = rp;

		delete this;
	}
}

void CAnim::alldel()
{
	CAnim *p;
	CAnim *t;
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

void CAnim::SetSightByD(int degree)
{
	double ang1;
	
	ang1 = PI*degree / 180;

	p1.x = (int)(speed*cos(ang1)) + p0.x;
	p1.y = (int)(speed*sin(ang1)) + p0.y;
}

void CAnim::SetMode(int mode)
{
	Mode = mode;
}

void CAnim::SetMode2(int mode)
{
	Mode2 = mode;
}

CPoint CAnim::GetPos() const
{
	return p0;
}

CAnim* CAnim::GetFp() const
{
	return fp;
}

int CAnim::GetMode() const
{
	return Mode;
}

int CAnim::GetMode2() const
{
	return Mode2;
}

int CAnim::GetDstnID() const
{
	return dstnID;
}

CPoint CAnim::GetDstn() const
{
	return dstn;
}

PointF* CAnim::GetPF()
{
	Point pt[3];
	if (p0.x < p1.x)
	{
		pt[0].X = p0.x - 20; pt[0].Y = p0.y - 20;
		pt[1].X = p0.x + 20; pt[1].Y = p0.y - 20;
		pt[2].X = p0.x + 20; pt[2].Y = p0.y + 20;
	}
	else
	{
		pt[0].X = p0.x - 20; pt[0].Y = p0.y - 20;
		pt[1].X = p0.x + 20; pt[1].Y = p0.y - 20;
		pt[2].X = p0.x - 20; pt[2].Y = p0.y + 20;
	}

	for (int i = 0; i < 3; i++)
	{
		pt[i].X = pf[i].X;
		pt[i].Y = pf[i].Y;
	}

	return pf;
}

int CAnim::GetSD() const
{
	return p1.x - p0.x;
}

void CAnim::Run(CPoint pt)	//빠른 속도로 이동
{
	SetSight(pt, 1);
	Move();
	Move();
	Move();
	Move();
}

void CAnim::RandomDstn()
{
	CPoint pt;

	int i = rand() % 7 + 1;
	dstnID = i;
	switch (i)
	{
	case 1:
		pt.x = 190;
		pt.y = 190;
		break;
	case 2:
		pt.x = 420;
		pt.y = 190;
		break;
	case 3:
		pt.x = 540;
		pt.y = 190;
		break;
	case 4:
		pt.x = 620;
		pt.y = 190;
		break;
	case 5:
		pt.x = 730;
		pt.y = 190;
		break;
	case 6:
		pt.x = 240;
		pt.y = 500;
		break;
	case 7:
		pt.x = 650;
		pt.y = 500;
		break;
	}

	dstn = pt;
}
