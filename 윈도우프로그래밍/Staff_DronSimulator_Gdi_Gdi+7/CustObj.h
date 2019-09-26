#pragma once

/* ���Ƿ� ������ Ŭ���� ���̰��� */

class CCustObj
{
protected:
	CPoint p0;	//�߽���ǥ
	CPoint p1;	//�þ���ǥ
	double ang;	//�þ���ǥ�� �������ִ� ����
	int speed;	//�̵��ӵ�
public:
	CCustObj() {}
	void SetPosition(int, int);		//�߽���ǥ ����
	void SetSpeed(int);				//�����̴� �ӵ�����
	void SetSight(CPoint, int);	//�þ���ǥ ����(�����ǥ���� �ٶ󺸰�)

	void Move();					//�þ߹������� ������
};

/* ��� */

class CDron : public CCustObj
{
private:
	//����
	int size;
	int degree;	//��� ���� ȸ���� ���� ���� ����
	int lngs;	//�þ� ����
	CPoint sl;	//���� �þ���ǥ
	CPoint sr;	//������ �þ���ǥ
	CPoint slt;	//���� ��(�պκ�)
	CPoint srt;	//������ ��
	CPoint chase;	//�i�ư��� ������ ��ġ
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

	/* ��� �׸������� �� ��ǥ ���ϱ� */
	CPoint GetLT() const;	//�»�, �밢�� �׸���
	CPoint GetRB() const;	//����
	CPoint GetRT() const;	//���
	CPoint GetLB() const;	//����

	CRect GetRect() const;	//���׸���

	CPoint GetLTW(int) const;	//�»󳯰�
	CPoint GetRTW(int) const;	//���
	CPoint GetLBW(int) const;	//����
	CPoint GetRBW(int) const;	//����

	void Wing();	//����ȸ��(��������)

	void LMove();	//�������� �̵�
	void RMove();	//���������� �̵�
	void BMove();	//�ڷ� �̵�
};


/* ����(���Ḯ��Ʈ ����) */

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
	void Ins(CAnim *);	//����
	void del();	//����
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