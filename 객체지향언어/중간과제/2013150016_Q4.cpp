/*
컴퓨터공학과 2013150016 서민성
C++ 실기과제
[Q4]등록된 모든 Member(학생 및 교수)를 요청시
화면에 출력하는 프로그램 Q4.cpp
*/
#include<iostream>
#include<cstring>
#include<typeinfo>

using namespace std;

/////////////////////
//  Member Class   //
/////////////////////
class Member
{
private:
	int ID;
	char *name;
public:
	Member() {}                     //기본생성자
	Member(int, char*);             //생성자
	Member(const Member &);         //깊은복사
	virtual void ShowInfo() const;  //정보출력 
	virtual ~Member();              //소멸자
};
Member::Member(int _ID, char *_name)
	:ID(_ID)
{
	name = new char[strlen(_name) + 1];
	strcpy(name, _name);
}
Member::Member(const Member &copy)
	: ID(copy.ID)
{
	name = new char[strlen(copy.name) + 1];
	strcpy(name, copy.name);
}
void Member::ShowInfo() const
{
	cout.flags(ios::left);   //왼쪽정렬
	cout.width(7);           //7칸 확보
	cout << ID;
	cout.width(8);           //8칸확보
	cout << name;
}
Member::~Member()
{
	delete []name;
}

/////////////////////
//  Student Class  //
/////////////////////
class Student : public Member
{
private:
	char *major;
public:
	Student() {}                   //기본생성자
	Student(int, char *, char *);  //생성자
	Student(const Student &copy);  //깊은복사
	void ShowInfo() const;         //멤버함수 데이터 출력
	~Student();                    //소멸자
};
Student::Student(int _ID, char *_name, char *_major)
	:Member(_ID, _name)
{
	major = new char[strlen(_major) + 1];
	strcpy(major, _major);
}
Student::Student(const Student &copy)
	: Member(copy)
{
	major = new char[strlen(copy.major) + 1];
	strcpy(major, copy.major);
}
void Student::ShowInfo() const
{
	Member::ShowInfo();
	cout << ' ' << major << endl;
}
Student::~Student()
{
	delete []major;
}

///////////////////////
//  Professor Class  //
///////////////////////
class Professor : public Member
{
private:
	char *department;                 //전공
public:
	Professor() {}                    //기본생성자
	Professor(int, char*, char*);     //싱성자
	Professor(const Professor &copy); //깊은복사
	void ShowInfo() const;            //정보출력
	~Professor();                     //소멸자
};
Professor::Professor(int _ID, char *_name, char *_department)
	:Member(_ID, _name)
{
	department = new char[strlen(_department) + 1];
	strcpy(department, _department);
}
Professor::Professor(const Professor &copy)
	: Member(copy)
{
	department = new char[strlen(copy.department) + 1];
	strcpy(department, copy.department);
}
void Professor::ShowInfo() const
{
	Member::ShowInfo();
	cout << ' ' << department << endl;
}
Professor::~Professor()
{
	delete []department;
}

////////////////////////
//  MemberList Class  //
////////////////////////
class MemberList
{
private:
	Member *mList[50];
	int index;
public:
	MemberList() :index(0) {}  //기본생성자
	void DefaultMember();      //디폴트목록 만들기
	void MakeStudent();        //Student객체 만들기
	void MakeProfessor();      //Professor객체 만들기
	void ShowStudent() const;  //학생정보 출력
	void ShowProfessor() const;//교수정보 출력
	void ShowAll() const;      //객체 정보 모두 출력
	~MemberList();             //소멸자
};
void MemberList::MakeStudent()
{
	char temp1[100]; //이름은 100자이내
	char temp2[100];

	cout << "====학생정보입력====" << endl;
	cout << "- ID: " << index + 1 << endl;
	cout << "- Name: ";  cin >> temp1;
	cout << "- Major: "; cin >> temp2;
	cout << "====================" << endl;

	mList[index] = new Student(index + 1, temp1, temp2);
	index++;

	cout << "==>> Added succesfully..." << endl;
}
void MemberList::MakeProfessor()
{
	char temp1[100];
	char temp2[100];

	cout << "====교수정보입력====" << endl;
	cout << "- ID: " << index + 1 << endl;
	cout << "- Name: ";       cin >> temp1;
	cout << "- Department: "; cin >> temp2;
	cout << "====================" << endl;

	mList[index] = new Professor(index + 1, temp1, temp2);
	index++;

	cout << "==>> Added succesfully..." << endl;
}
void MemberList::ShowStudent() const
{
	cout << "--------<All Students>---------" << endl;
	cout << "ID     Name     Major" << endl;
	cout << "-------------------------------" << endl;

	for (int i = 0; i<index; i++)
	{
		if (strcmp(typeid(*(mList[i])).name(), "class Student") == 0)
			mList[i]->ShowInfo();
	}
	cout << "-------------------------------" << endl;
}
void MemberList::ShowProfessor() const
{
	cout << "--------<All Professors>-------" << endl;
	cout << "ID     Name     Department" << endl;
	cout << "-------------------------------" << endl;

	for (int i = 0; i < index; i++)
	{
		if (strcmp(typeid(*(mList[i])).name(), "class Professor") == 0)
			mList[i]->ShowInfo();
	}
	cout << "-------------------------------" << endl;
}
void MemberList::ShowAll() const
{
	cout << "-------------<All Members>---------------" << endl;
	cout << "Status  ID     Name     Department/Major" << endl;
	cout << "-----------------------------------------" << endl;

	for (int i = 0; i<index; i++)
	{
		if (strcmp(typeid(*(mList[i])).name(), "class Student") == 0)
		{
			cout.flags(ios::left);
			cout.width(8);
			cout << "학생";
			mList[i]->ShowInfo();
		}
		else if (strcmp(typeid(*(mList[i])).name(), "class Professor") == 0)
		{
			cout.flags(ios::left);
			cout.width(8);
			cout << "교수";
			mList[i]->ShowInfo();
		}
	}
	cout << "-----------------------------------------" << endl;
}
void MemberList::DefaultMember()
{
	mList[index] = new Student(index + 1, "김철수", "컴퓨터공학전공"); index++;
	mList[index] = new Student(index + 1, "홍길동", "소프트웨어전공"); index++;
	mList[index] = new Student(index + 1, "김동수", "소프트웨어전공"); index++;
	mList[index] = new Student(index + 1, "이철희", "컴퓨터공학전공"); index++;

	mList[index] = new Professor(index + 1, "이영희", "컴퓨터공학전공"); index++;
	mList[index] = new Professor(index + 1, "박진희", "컴퓨터공학전공"); index++;
	mList[index] = new Professor(index + 1, "김일동", "컴퓨터공학전공"); index++;
	mList[index] = new Professor(index + 1, "정진선", "소프트웨어전공"); index++;
	mList[index] = new Professor(index + 1, "전두원", "소프트웨어전공"); index++;
}
MemberList::~MemberList()
{
	for (int i = 0; i<index; i++)
		delete mList[i];
}
MemberList ml;

//////////////////////
//  MainMenu Class  //
//////////////////////
class MainMenu
{
public:
	int IssueMenu();
};
int MainMenu::IssueMenu()
{
	int sel;

	cout << endl;
	cout << "--------< 메 뉴 >--------" << endl;
	cout << "  1. 학생 추가" << endl;
	cout << "  2. 교수 추가" << endl;
	cout << endl;
	cout << "  3. 학생목록 출력" << endl;
	cout << "  4. 교수목록 출력" << endl;
	cout << "  5. 전체목록 출력" << endl;
	cout << endl;
	cout << "  9. 화면정리" << endl;
	cout << "  0. 종료" << endl;
	cout << "-------------------------" << endl;
	cout << "  선택>>"; cin >> sel;
	cout << endl;

	return sel;
}
MainMenu mm;

//////////////////////
//  MainCtrl Class  //
//////////////////////
class MainCtrl
{
private:
	int Main();
public:
	MainCtrl() { Main(); } //생성자에서 Main()호출함으로서 MainCtrl객체 생성시 프로그램 시작
};
int MainCtrl::Main()
{
	int sel;

	ml.DefaultMember();  //디폴트목록 넣기(전역객체 ml)

	while (1)
	{
		sel = mm.IssueMenu(); //메인메뉴(전역객체 mm)

		switch (sel)
		{
		case 1:
			ml.MakeStudent();
			break;
		case 2:
			ml.MakeProfessor();
			break;
		case 3:
			ml.ShowStudent();
			break;
		case 4:
			ml.ShowProfessor();
			break;
		case 5:
			ml.ShowAll();
			break;
		case 9:
			system("cls");
			break;
		case 0:
			return 0;
		default:
			cout << "==> " << sel << ": Invalid selection." << endl;
		}
	}
}

MainCtrl mc;

int main() {}