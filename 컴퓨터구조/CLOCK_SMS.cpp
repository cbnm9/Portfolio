/*
	2013150016 ���μ� ��ǻ�ͱ��� 2��������
	����޸� �������ι��, CLOCK�˰��� �ÿ� ���α׷�
	2017.11.29
*/


#define _CRT_SECURE_NO_WARNINGS

#include <iostream>

using namespace std;

/* ������ ���̺� ���� Ŭ���� */

class PageTableInfo       //PageTable�� ��ϵ� ����
{
private:
	int PageNum;          //Page��ȣ
	int UsedBit;          //����Ʈ(CLOCK���)
	int PageFrameNum;	  //PageFrame��ȣ, �� ���α׷����� �����������
public:
	PageTableInfo() {};
	void SetPageNum(int);	//Page��ȣ ����
	void SetUsedBit(int);	//UsedBit ����
	int GetPageNum() const;	//Page��ȣ ��ȯ
	int GetUsedBit() const;	//UsedBit ��ȯ
};


void PageTableInfo::SetPageNum(int pn) //��������ȣ ����
{
	PageNum = pn;
}

void PageTableInfo::SetUsedBit(int ub) //����Ʈ ����
{
	UsedBit = ub;
}

int PageTableInfo::GetPageNum() const //��������ȣ ��ȯ
{
	return PageNum;
}

int PageTableInfo::GetUsedBit() const //����Ʈ ��ȯ
{
	return UsedBit;
}

/* ������ ���̺� ��ü Ŭ���� */

class PageTable										//PageTable��ü
{
private:
	PageTableInfo pti[5];							//�� 5���� PageFrame�� �����Ƿ� 5��
public:
	PageTable();
	PageTableInfo* GetPageTableInfo(int);			//������ ���̺� ������ü�� ������ ��ȯ
};

PageTable::PageTable()			//���������̺��� ���� �ʱ�ȭ
{
	for (int i = 0; i < 5; i++)
	{
		pti[i].SetPageNum(100);	//���� ��������ȣ�� �ʱ�ȭ(100)
		pti[i].SetUsedBit(0);	//����Ʈ�� 0(ó�� �������ö� ���ʴ�� ä����)
	}
}

PageTableInfo* PageTable::GetPageTableInfo(int n)	//������ ���̺� ������ü�� ������ ��ȯ
{
	return &(pti[n]);
}

/* �Է� �� ���� Ŭ���� */

class InputData				//���Ͽ��� �Է°� �����ͼ� �����ϴ� Ŭ����
{
private:
	int input[1000];		//�Է°�(�ִ� 1000��)
	int index;				//�Է��� ����
public:
	InputData();			//������ -> ���Ͽ��� �Է°� �������ºκ�
	int GetInput(int);		//Ư���κ� �Է°� ��ȯ
	int GetIndex() const;	//�Է��� ���� ��ȯ
};

InputData::InputData()		//������ : ���Ͽ��� �Է°� �������ºκ�(��ü�� �����ϸ� �ڵ����� ���������� ������ �����ϰԵ�)
	:index(0)
{
	FILE *fp;

	char buff[100];			//���Ͽ��� �б����� ����
	int a;					//������ ��ȯ�� ��� �������ִ� ����

	fp = fopen("input.txt", "r");

	if (fp == NULL)			//������ ������ ���α׷� ����
	{
		cout << "������ �����ϴ�." << endl;
		system("PAUSE");
		exit(1);
	}

	while (1)				//���Ͽ��� �о ������ ��ȯ�� �����ϴ� ����
	{
		fscanf(fp, "%s", buff);

		a = atoi(buff);

		if (a == 99)		//99 �Է��� end point(��������ʴ´�)
			break;

		input[index] = a;
		index++;

	}

	fclose(fp);
}

int InputData::GetInput(int i)	//Ư���κ��� �Է°� ��ȯ
{
	return input[i];
}

int InputData::GetIndex() const	//�Է��� ���� ��ȯ
{
	return index;
}

/* ȭ�鿡 ����ϴ� Ŭ���� */
class Display						//������ �������ִٰ� ȭ�鿡 ���
{
private:
	int Input[1000];				//�Է°��� ����
	int PageTableValue[5][1000];	//PageTable��(�� ������ �������� �������ִ� ������ �ѹ���)�� ����
	int UsedBitValue[5][1000];		//����Ʈ ����
	char PointerLoc[5][1000];		//������ ��ġ
	int index;						//�Է��� ����
	int Success;					//������ ����
	int SuccessLoc[1000];			//������ ��ġ
public:
	Display();
	void SetInputValue(int);		//�Է°��� ���� �Է�
	void SetTableValue(int, int);	//PageTable���� ���� �Է�
	void SuccessINC();				//����Ƚ�� ����
	void SetSuccess();				//������ ��ġ ���
	void SetPointer(int);			//������ ��ġ ���
	void SetUsedBit(int, int);		//����Ʈ ���
	void IndexINC();				//�ε��� �ϳ�����
	void ShowR();					//ȭ�鿡 ���
};

Display::Display()
	:index(0), Success(0)
{
	for (int i = 0; i < 1000; i++)	//������ ��ġ �ʱ�ȭ
		SuccessLoc[i] = 0;
}

void Display::SetInputValue(int pn)			//�Է°��� ���� �Է�
{
	Input[index] = pn;
}

void Display::SetTableValue(int pn, int n)	//PageTable��(�� ������ �������� �������ִ� ������ �ѹ���)�� ���� �Է�
{
	PageTableValue[n][index] = pn;
}

void Display::SuccessINC()	//����Ƚ�� ����
{
	Success++;
}

void Display::SetSuccess()	//���߽� �� ��ġ�� 1
{
	SuccessLoc[index] = 1;
}

void Display::SetPointer(int ptr)	//�����Ͱ� ��ġ�Ѱ� ǥ��
{
	for (int i = 0; i < 5; i++)		//���� ��������ġ�迭 �ʱ�ȭ
	{
		PointerLoc[i][index] = ' ';
	}

	int z;

	for (z = 0; z < 5; z++)		//ó�� ���� ä������������ �˻��ϱ����� �ݺ���
	{
		if (PageTableValue[z][index] == 100)
			break;
	}

	if (z != 5)					//ó�� ������� ä�����µ��ȿ��� ������ �̵�����(�� ���� ��������ġ�� 0���� ����)
	{
		PointerLoc[0][index] = '-';
	}
	else
		PointerLoc[ptr][index] = '-';
}

void Display::SetUsedBit(int ub, int n)	//����Ʈ ���� �Է�
{
	UsedBitValue[n][index] = ub;
}

void Display::IndexINC()	//�ε��� �ϳ�����
{
	index++;
}

void Display::ShowR()					//ȭ�鿡 ���, ����� ���� ������ ��������
{
	cout << "  <<  2013150016  ���μ� >> |                                           " << endl;
	cout << "                            |  PageNumber ���� '-' ǥ�� : �������� ��ġ " << endl;
	cout << "       Clock �˰���       |  PageNumber �Ʒ��� 0 �Ǵ� 1 : UsedBit�� ��" << endl;
	cout << "----------------------------" << endl<<endl;

	/*  ������� (215�� ~ 295��)  */

	for (int k = 0; k < index+1; k++)
	{
		cout << "=====";
	}

	cout << endl;

	printf("%6c", ' ');

	for (int z = 0; z < index; z++)		//������ ��ġ�� S���
	{
		if (SuccessLoc[z] == 0)
			printf("%3c  ", ' ');
		else
			printf("%3c  ", 'S');
	}

	cout << endl;
	
	for (int k = 0; k < index+1; k++)
	{
		cout << "=====";
	}
	cout << endl;

	printf("%4s", " IN ��");

	for (int i = 0; i < index; i++)		//�Է°� ȭ�鿡 ���
	{
		printf("%3d  ", Input[i]);
	}

	cout << endl;

	for (int k = 0; k < index+1; k++)
	{
		cout << "=====";
	}
	cout << endl;

	for (int j = 0; j < 5; j++)			//�� ������ �����Ӻ� ��������ȣ�� ����Ʈ ȭ�鿡 ���
	{
		printf("%6s", "P.N��");

		for (int i = 0; i < index; i++)	//��������ȣ(100�̸� ����ִٴ� ��)
		{
			if (PageTableValue[j][i] == 100)
				printf("%3c  ", ' ');
			else
				printf("%1c%2d  ", PointerLoc[j][i], PageTableValue[j][i]);

		}
		cout << endl;

		printf("%6s", "U.B��");

		for (int i = 0; i < index; i++)	//����Ʈ
		{
			if (PageTableValue[j][i] == 100 || PageTableValue[j][i] == -100)
				printf("%3c  ", ' ');
			else
				printf("%3d  ", UsedBitValue[j][i]);

		}
		cout << endl;

		for (int k = 0; k < index+1; k++)
		{
			cout << "-----";
		}
		cout << endl;
	}

	for (int k = 0; k < index+1; k++)
	{
		cout << "=====";
	}
	cout << endl;

	/*  ��������� �Է°� ���ö������� ������ �����ִ� �κ� */

	cout << endl;
	cout << endl;

	/* �Ʒ� 8���� ������ �������� �� ���߷��� �����ִ� �κ� */

	cout << "< ���� PageTable�� ���� >" << endl << endl;
	printf("%s", "PageFrame 0 : "); printf("%d\n", PageTableValue[0][index - 1]);
	printf("%s", "PageFrame 1 : "); printf("%d\n", PageTableValue[1][index - 1]);
	printf("%s", "PageFrame 2 : "); printf("%d\n", PageTableValue[2][index - 1]);
	printf("%s", "PageFrame 3 : "); printf("%d\n", PageTableValue[3][index - 1]);
	printf("%s", "PageFrame 4 : "); printf("%d\n", PageTableValue[4][index - 1]);

	double result = (double)Success / index;
	cout << endl << endl << "< ���߷� > : " << Success << '/' << index << " = " << result << endl << endl;
}

/* MMU Ŭ���� */

class MMU					//�Է¹��� ��ȣ�� ���� �����ϴ� ��ü(�ּ�ã��)
{
private:
	PageTable pt;			//���������̺� ��ü
	int pointer;			//��ü�� ������ ã������ ������

	/* �Ʒ� 4���Լ��� MMU�� ����Լ������� ����ϴ� �Լ� */

	int CheckMiss(int);		//����, ���� Ȯ��
	void SearchPoint();		//��ü�� ���� ��ġã��
	void ChangePage(int);	//������ ��ü�ϱ�
	void PointINC();		//������ �ϳ�������
public:
	MMU();
	void Read(Display*);	//�Է°�(��������ȣ) �б�
};

MMU::MMU()
	:pointer(0)
{}

void MMU::PointINC()  //������ �ϳ��� �����ϸ鼭 ȸ����Ű�� �Լ�
{
	if (pointer < 4)
		pointer++;
	else
		pointer = 0;
}

int MMU::CheckMiss(int pn)	//���߽� ����Ʈ1�μ����� 1��ȯ, ���н� 0��ȯ
{
	for (int i = 0; i < 5; i++)
	{
		if (pt.GetPageTableInfo(i)->GetPageNum() == pn)		//���߽� ����Ʈ�� 1�ιٲٱ�
		{
			pt.GetPageTableInfo(i)->SetUsedBit(1);
			return 1;
		}
	}

	return 0;
}

void MMU::SearchPoint() //��ü�� ��ġã��
{
	while (1)
	{
		if (pt.GetPageTableInfo(pointer)->GetUsedBit() == 0)	//����Ʈ�� 0�ΰ� ã���� ��
		{
			break;
		}
		else
		{
			pt.GetPageTableInfo(pointer)->SetUsedBit(0); //����Ʈ�� 0���� �ٲٸ鼭 ������ �̵���Ű��
			PointINC();
		}
	}
}

void MMU::ChangePage(int pn)	//������ ��ü�ϱ�, ��ü�� ������ �̵�
{
	pt.GetPageTableInfo(pointer)->SetPageNum(pn);	//��������ü
	PointINC();										//������ �̵�
}

void MMU::Read(Display *dy)		//�� �����Լ�
{
	InputData ip;				//�Էµ����͸� �������ִ� ��ü

	for (int i = 0; i < ip.GetIndex(); i++)	//�Էµ� ������ ����(�б� Ƚ��)��ŭ �ݺ�
	{

		if (!CheckMiss(ip.GetInput(i))) //Miss�� ������, ���߽� ������ ������ CheckMiss �Լ��� ����
		{
			SearchPoint();				//��ü�� ��ġ ã��
			ChangePage(ip.GetInput(i)); //��ü�ϱ�
		}
		else							//�����ߴٸ�
		{
			dy->SetSuccess();			//��� Ŭ������ ������ ��ġ ���
			dy->SuccessINC();			//��� Ŭ������ ����Ƚ�� ����
		}

		/* ���� �� PageTable�� ���¸� ��� Ŭ������ ���� */

		dy->SetInputValue(ip.GetInput(i));	//�Է°�

		for (int t = 0; t < 5; t++)			//�� �����������Ӻ� ��������ȣ, ����Ʈ
		{
			dy->SetTableValue(pt.GetPageTableInfo(t)->GetPageNum(), t);
			dy->SetUsedBit(pt.GetPageTableInfo(t)->GetUsedBit(), t);
		}

		dy->SetPointer(pointer);			//���� �������� ��ġ

		dy->IndexINC();						//���Ŭ������ �ε��� ����
	}

}

/* ���� �Լ� */

int main()
{
	MMU mmu;			//MMU��ü����
	Display dp;			//Display��ü����

	mmu.Read(&dp);		//input.txt���� �Է� �о ���α׷� ����

	dp.ShowR();			//ȭ�鿡 ������ �����ֱ�

	system("PAUSE");	//��� ����(�������Ϸ� ������������ ����)

	return 0;
}