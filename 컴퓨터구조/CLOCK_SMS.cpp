/*
	2013150016 서민성 컴퓨터구조 2차팀과제
	가상메모리 연관매핑방식, CLOCK알고리즘 시연 프로그램
	2017.11.29
*/


#define _CRT_SECURE_NO_WARNINGS

#include <iostream>

using namespace std;

/* 페이지 테이블 정보 클래스 */

class PageTableInfo       //PageTable에 기록될 정보
{
private:
	int PageNum;          //Page번호
	int UsedBit;          //사용비트(CLOCK방식)
	int PageFrameNum;	  //PageFrame번호, 이 프로그램에서 사용하지않음
public:
	PageTableInfo() {};
	void SetPageNum(int);	//Page번호 설정
	void SetUsedBit(int);	//UsedBit 설정
	int GetPageNum() const;	//Page번호 반환
	int GetUsedBit() const;	//UsedBit 반환
};


void PageTableInfo::SetPageNum(int pn) //페이지번호 설정
{
	PageNum = pn;
}

void PageTableInfo::SetUsedBit(int ub) //사용비트 설정
{
	UsedBit = ub;
}

int PageTableInfo::GetPageNum() const //페이지번호 반환
{
	return PageNum;
}

int PageTableInfo::GetUsedBit() const //사용비트 반환
{
	return UsedBit;
}

/* 페이지 테이블 객체 클래스 */

class PageTable										//PageTable객체
{
private:
	PageTableInfo pti[5];							//총 5개의 PageFrame이 있으므로 5개
public:
	PageTable();
	PageTableInfo* GetPageTableInfo(int);			//페이지 테이블 정보객체의 포인터 반환
};

PageTable::PageTable()			//페이지테이블의 정보 초기화
{
	for (int i = 0; i < 5; i++)
	{
		pti[i].SetPageNum(100);	//없는 페이지번호로 초기화(100)
		pti[i].SetUsedBit(0);	//사용비트는 0(처음 정보들어올때 차례대로 채워짐)
	}
}

PageTableInfo* PageTable::GetPageTableInfo(int n)	//페이지 테이블 정보객체의 포인터 반환
{
	return &(pti[n]);
}

/* 입력 값 저장 클래스 */

class InputData				//파일에서 입력값 가져와서 저장하는 클래스
{
private:
	int input[1000];		//입력값(최대 1000개)
	int index;				//입력의 갯수
public:
	InputData();			//생성자 -> 파일에서 입력값 가져오는부분
	int GetInput(int);		//특정부분 입력값 반환
	int GetIndex() const;	//입력의 갯수 반환
};

InputData::InputData()		//생성자 : 파일에서 입력값 가져오는부분(객체를 생성하면 자동으로 파일정보를 가져와 저장하게됨)
	:index(0)
{
	FILE *fp;

	char buff[100];			//파일에서 읽기위한 버퍼
	int a;					//정수로 변환후 잠시 가지고있는 버퍼

	fp = fopen("input.txt", "r");

	if (fp == NULL)			//파일이 없으면 프로그램 종료
	{
		cout << "파일이 없습니다." << endl;
		system("PAUSE");
		exit(1);
	}

	while (1)				//파일에서 읽어서 정수로 변환후 저장하는 과정
	{
		fscanf(fp, "%s", buff);

		a = atoi(buff);

		if (a == 99)		//99 입력은 end point(기록하지않는다)
			break;

		input[index] = a;
		index++;

	}

	fclose(fp);
}

int InputData::GetInput(int i)	//특정부분의 입력값 반환
{
	return input[i];
}

int InputData::GetIndex() const	//입력의 갯수 반환
{
	return index;
}

/* 화면에 출력하는 클래스 */
class Display						//정보를 가지고있다가 화면에 출력
{
private:
	int Input[1000];				//입력값의 정보
	int PageTableValue[5][1000];	//PageTable값(각 페이지 프레임이 가지고있는 페이지 넘버값)의 정보
	int UsedBitValue[5][1000];		//사용비트 정보
	char PointerLoc[5][1000];		//포인터 위치
	int index;						//입력의 갯수
	int Success;					//적중한 갯수
	int SuccessLoc[1000];			//적중한 위치
public:
	Display();
	void SetInputValue(int);		//입력값의 정보 입력
	void SetTableValue(int, int);	//PageTable값의 정보 입력
	void SuccessINC();				//적중횟수 증가
	void SetSuccess();				//적중한 위치 기록
	void SetPointer(int);			//포인터 위치 기록
	void SetUsedBit(int, int);		//사용비트 기록
	void IndexINC();				//인덱스 하나증가
	void ShowR();					//화면에 출력
};

Display::Display()
	:index(0), Success(0)
{
	for (int i = 0; i < 1000; i++)	//적중한 위치 초기화
		SuccessLoc[i] = 0;
}

void Display::SetInputValue(int pn)			//입력값의 정보 입력
{
	Input[index] = pn;
}

void Display::SetTableValue(int pn, int n)	//PageTable값(각 페이지 프레임이 가지고있는 페이지 넘버값)의 정보 입력
{
	PageTableValue[n][index] = pn;
}

void Display::SuccessINC()	//적중횟수 증가
{
	Success++;
}

void Display::SetSuccess()	//적중시 그 위치에 1
{
	SuccessLoc[index] = 1;
}

void Display::SetPointer(int ptr)	//포인터가 위치한곳 표시
{
	for (int i = 0; i < 5; i++)		//먼저 포인터위치배열 초기화
	{
		PointerLoc[i][index] = ' ';
	}

	int z;

	for (z = 0; z < 5; z++)		//처음 값이 채워지는중인지 검사하기위한 반복문
	{
		if (PageTableValue[z][index] == 100)
			break;
	}

	if (z != 5)					//처음 비워진곳 채워지는동안에는 포인터 이동없음(그 동안 포인터위치는 0으로 고정)
	{
		PointerLoc[0][index] = '-';
	}
	else
		PointerLoc[ptr][index] = '-';
}

void Display::SetUsedBit(int ub, int n)	//사용비트 정보 입력
{
	UsedBitValue[n][index] = ub;
}

void Display::IndexINC()	//인덱스 하나증가
{
	index++;
}

void Display::ShowR()					//화면에 출력, 출력을 위해 구성이 복잡해짐
{
	cout << "  <<  2013150016  서민성 >> |                                           " << endl;
	cout << "                            |  PageNumber 옆에 '-' 표시 : 포인터의 위치 " << endl;
	cout << "       Clock 알고리즘       |  PageNumber 아래의 0 또는 1 : UsedBit의 값" << endl;
	cout << "----------------------------" << endl<<endl;

	/*  여기부터 (215줄 ~ 295줄)  */

	for (int k = 0; k < index+1; k++)
	{
		cout << "=====";
	}

	cout << endl;

	printf("%6c", ' ');

	for (int z = 0; z < index; z++)		//성공한 위치에 S찍기
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

	printf("%4s", " IN ㅣ");

	for (int i = 0; i < index; i++)		//입력값 화면에 출력
	{
		printf("%3d  ", Input[i]);
	}

	cout << endl;

	for (int k = 0; k < index+1; k++)
	{
		cout << "=====";
	}
	cout << endl;

	for (int j = 0; j < 5; j++)			//각 페이지 프레임별 페이지번호와 사용비트 화면에 출력
	{
		printf("%6s", "P.Nㅣ");

		for (int i = 0; i < index; i++)	//페이지번호(100이면 비어있다는 뜻)
		{
			if (PageTableValue[j][i] == 100)
				printf("%3c  ", ' ');
			else
				printf("%1c%2d  ", PointerLoc[j][i], PageTableValue[j][i]);

		}
		cout << endl;

		printf("%6s", "U.Bㅣ");

		for (int i = 0; i < index; i++)	//사용비트
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

	/*  여기까지는 입력값 들어올때마다의 내역을 보여주는 부분 */

	cout << endl;
	cout << endl;

	/* 아래 8개의 문장이 최종상태 및 적중률을 보여주는 부분 */

	cout << "< 최종 PageTable의 상태 >" << endl << endl;
	printf("%s", "PageFrame 0 : "); printf("%d\n", PageTableValue[0][index - 1]);
	printf("%s", "PageFrame 1 : "); printf("%d\n", PageTableValue[1][index - 1]);
	printf("%s", "PageFrame 2 : "); printf("%d\n", PageTableValue[2][index - 1]);
	printf("%s", "PageFrame 3 : "); printf("%d\n", PageTableValue[3][index - 1]);
	printf("%s", "PageFrame 4 : "); printf("%d\n", PageTableValue[4][index - 1]);

	double result = (double)Success / index;
	cout << endl << endl << "< 적중률 > : " << Success << '/' << index << " = " << result << endl << endl;
}

/* MMU 클래스 */

class MMU					//입력받은 번호로 연산 수행하는 객체(주소찾기)
{
private:
	PageTable pt;			//페이지테이블 객체
	int pointer;			//교체할 정보를 찾기위한 포인터

	/* 아래 4개함수는 MMU의 멤버함수에서만 사용하는 함수 */

	int CheckMiss(int);		//적중, 실패 확인
	void SearchPoint();		//교체할 곳의 위치찾기
	void ChangePage(int);	//페이지 교체하기
	void PointINC();		//포인터 하나씩증가
public:
	MMU();
	void Read(Display*);	//입력값(페이지번호) 읽기
};

MMU::MMU()
	:pointer(0)
{}

void MMU::PointINC()  //포인터 하나씩 증가하면서 회전시키는 함수
{
	if (pointer < 4)
		pointer++;
	else
		pointer = 0;
}

int MMU::CheckMiss(int pn)	//적중시 사용비트1로설정후 1반환, 실패시 0반환
{
	for (int i = 0; i < 5; i++)
	{
		if (pt.GetPageTableInfo(i)->GetPageNum() == pn)		//적중시 사용비트를 1로바꾸기
		{
			pt.GetPageTableInfo(i)->SetUsedBit(1);
			return 1;
		}
	}

	return 0;
}

void MMU::SearchPoint() //교체할 위치찾기
{
	while (1)
	{
		if (pt.GetPageTableInfo(pointer)->GetUsedBit() == 0)	//사용비트가 0인곳 찾으면 끝
		{
			break;
		}
		else
		{
			pt.GetPageTableInfo(pointer)->SetUsedBit(0); //사용비트를 0으로 바꾸면서 포인터 이동시키기
			PointINC();
		}
	}
}

void MMU::ChangePage(int pn)	//페이지 교체하기, 교체후 포인터 이동
{
	pt.GetPageTableInfo(pointer)->SetPageNum(pn);	//페이지교체
	PointINC();										//포인터 이동
}

void MMU::Read(Display *dy)		//주 동작함수
{
	InputData ip;				//입력데이터를 가지고있는 객체

	for (int i = 0; i < ip.GetIndex(); i++)	//입력된 데이터 갯수(읽기 횟수)만큼 반복
	{

		if (!CheckMiss(ip.GetInput(i))) //Miss시 수행함, 적중시 수행할 내용은 CheckMiss 함수에 있음
		{
			SearchPoint();				//교체할 위치 찾기
			ChangePage(ip.GetInput(i)); //교체하기
		}
		else							//적중했다면
		{
			dy->SetSuccess();			//출력 클래스에 적중한 위치 기록
			dy->SuccessINC();			//출력 클래스에 적중횟수 증가
		}

		/* 수행 후 PageTable의 상태를 출력 클래스에 저장 */

		dy->SetInputValue(ip.GetInput(i));	//입력값

		for (int t = 0; t < 5; t++)			//각 페이지프레임별 페이지번호, 사용비트
		{
			dy->SetTableValue(pt.GetPageTableInfo(t)->GetPageNum(), t);
			dy->SetUsedBit(pt.GetPageTableInfo(t)->GetUsedBit(), t);
		}

		dy->SetPointer(pointer);			//현재 포인터의 위치

		dy->IndexINC();						//출력클래스의 인덱스 증가
	}

}

/* 메인 함수 */

int main()
{
	MMU mmu;			//MMU객체생성
	Display dp;			//Display객체생성

	mmu.Read(&dp);		//input.txt에서 입력 읽어서 프로그램 수행

	dp.ShowR();			//화면에 수행결과 보여주기

	system("PAUSE");	//잠깐 멈춤(실행파일로 실행했을때를 위함)

	return 0;
}