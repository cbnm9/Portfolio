#pragma once

/* 임의로 생성한 클래스 같이관리 */

class CCustObj
{
protected:
	CPoint p0;	//중심좌표
	CPoint p1;	//시야좌표
	double ang;	//시야좌표를 결정해주는 각도
	int speed;	//이동속도
public:
	CCustObj() {}
	void SetPosition(int, int);		//중심좌표 설정
	void SetSpeed(int);				//움직이는 속도설정
	void SetSight(CPoint, int);	//시야좌표 설정(대상좌표쪽을 바라보게)

	void Move();					//시야방향으로 움직임
};

/* 드론 */

class CDron : public CCustObj
{
private:
	//변수
	int size;
	int degree;	//드론 날개 회전을 위한 각도 변수
	int lngs;	//시야 길이
	CPoint sl;	//왼쪽 시야좌표
	CPoint sr;	//오른쪽 시야좌표
	CPoint slt;	//왼쪽 위(먼부분)
	CPoint srt;	//오른쪽 위
	CPoint chase;	//쫒아가는 동물의 위치
public:
	CDron();
	void SetSize(int);
	void SetChase(CPoint);
	void SetLngs(int);
	void SetSP();
	void SetSight2(double ang1);

	int GetSize() const;
	CPoint GetPos() const;
	CPoint GetSight() const;
	double GetAng() const;
	CPoint GetChase() const;
	CPoint GetSl() const;
	CPoint GetSr() const;
	CPoint GetSlt() const;
	CPoint GetSrt() const;

	/* 드론 그리기위한 각 좌표 구하기 */
	CPoint GetLT() const;	//좌상, 대각선 그리기
	CPoint GetRB() const;	//우하
	CPoint GetRT() const;	//우상
	CPoint GetLB() const;	//좌하

	CRect GetRect() const;	//원그리기

	CPoint GetLTW(int) const;	//좌상날개
	CPoint GetRTW(int) const;	//우상
	CPoint GetLBW(int) const;	//좌하
	CPoint GetRBW(int) const;	//우하

	void Wing();	//날개회전(각도변경)

	void LMove();	//왼쪽으로 이동
	void RMove();	//오른쪽으로 이동
	void BMove();	//뒤로 이동
};


/* 동물(연결리스트 구조) */

class CAnim : public CCustObj
{
private:
	int ID;
	int Mode;
	int Mode2;
	CPoint dstn;
	int dstnID;
	PointF pf[3];
	CAnim *fp;
	CAnim *rp;
public:
	CAnim() : Mode2(0)
	{}
	CAnim(CPoint, int spd, int id);
	void Ins(CAnim *);	//삽입
	void del();	//삭제
	void alldel();

	void SetSightByD(int);
	void SetMode(int);
	void SetMode2(int);

	CPoint GetPos() const;
	CAnim* GetFp() const;
	int GetMode() const;
	int GetMode2() const;
	int GetDstnID() const;
	PointF* GetPF();
	CPoint GetDstn() const;
	int GetSD() const;
	void Run(CPoint);
	void RandomDstn();
};