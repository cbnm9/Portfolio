/*
컴퓨터공학과 2013150016 서민성
C++ 실기과제
[Q6]반(Class)은 교과목, 교수, 학생으로 구성된다.
반을 만드는 기능과 반별로 출력하는 기능을 추가한 프로그램
Q6.cpp
-반(Class)은 ClassList로 50개까지 관리
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
	Professor(int, char*, char*);     //생성자
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
	Member* ReturnMember(const int&); //멤버 찾아서 리턴
	void DefaultMember();      //디폴트목록 만들기
	void MakeStudent();        //Student객체 만들기
	void MakeProfessor();      //Professor객체 만들기
	void ShowStudent() const;  //학생정보 출력
	void ShowProfessor() const;//교수정보 출력
	void ShowAll() const;      //객체 정보 모두 출력
	~MemberList();             //소멸자
};
Member* MemberList::ReturnMember(const int &sel)
{
	return mList[sel-1];
}
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

/////////////////////
//  Subject Class  //
/////////////////////
class Subject
{
private:
	int ID;
	char *title;
public:
	Subject() {}				   //기본생성자
	Subject(int, char *);		   //생성자
	Subject(const Subject &copy);  //깊은복사
	void ShowTitle() const;        //Title 출력
	void ShowInfo() const;		   //정보출력
	~Subject();					   //소멸자
};
Subject::Subject(int _ID, char *_title)
	:ID(_ID)
{
	title = new char[strlen(_title) + 1];
	strcpy(title, _title);
}
Subject::Subject(const Subject &copy)
	: ID(copy.ID)
{
	title = new char[strlen(copy.title) + 1];
	strcpy(title, copy.title);
}
void Subject::ShowTitle() const
{
	cout<<title<<endl;
}
void Subject::ShowInfo() const
{
	cout.flags(ios::left);
	cout.width(7);
	cout << ID;
	cout << title << endl;
}
Subject::~Subject()
{
	delete []title;
}

/////////////////////////
//  SubjectList Class  //
/////////////////////////
class SubjectList
{
private:
	Subject sList[4];
	int index;
public:
	SubjectList() :index(0) {}				  //기본생성자
	Subject* ReturnSubject(const int&);       //받은 ID에 맞는 subject리턴
	void DefaultSubject();					  //디폴트목록 입력
	void ShowSelectSubject(const int&) const; //선택한과목 출력
	void ShowAll() const;					  //모두 출력
};
Subject* SubjectList::ReturnSubject(const int &sel)
{
	return &(sList[sel-1]);
}
void SubjectList::DefaultSubject() //Subject객체를 만들고 그 객체를 sList 각 요소가 참조
{
	/*
		Subject객체를 heap영역에 만든후 복사를 진행하면
		복사생성자가 호출되 복사를 진행할줄알았는데
		복사생성자가 호출되지않고
		객체 배열이 heap영역에 만든 객체를 직접 참조하는거같음
		그래서 위 두경우는 함수 벗어나면 객체가 없어져서 오류나는거같음
		이유는?

		객체배열은 선언했을때 첨자의 수만큼 참조변수가 만들어진다
		그러므로 객체배열 각 요소는 참조자로 볼수있어
		객체를 생성후 그 객체를 참조하는형식으로 초기화해야한다
		객체를 이 함수에서 초기화후 함수를 나가서 사용해야하므로
		함수가 끝나도 객체가 존재하도록 heap영역에 만든후 참조해야한다
	*/
	sList[index] = *(new Subject(1, "객체지향 언어")); index++;
	sList[index] = *(new Subject(2, "윈도우 응용"));   index++;
	sList[index] = *(new Subject(3, "웹 프로그래밍")); index++;
	sList[index] = *(new Subject(4, "JAVA 응용"));     index++;
}
void SubjectList::ShowSelectSubject(const int &sel) const
{
	cout << "------------------------" << endl;
	cout << "ID     교과목명" << endl;
	cout << "------------------------" << endl;
	sList[sel - 1].ShowInfo();
	cout << "------------------------" << endl;
}
void SubjectList::ShowAll() const
{
	cout << "------------------------" << endl;
	cout << "ID     교과목명" << endl;
	cout << "------------------------" << endl;
	for (int i = 0; i < index; i++)
	{
		sList[i].ShowInfo();
	}
	cout << "------------------------" << endl;
}
SubjectList sl;

///////////////////
//  Class Class  //
///////////////////
class Class
{
private:
	int ID;              //클래스 ID
	Subject *subject;    //클래스의 과목
	Member *mList[50];   //클래스의 학생 및 교수
	int index;
public:
	Class() :index(0) {}                   //기본생성자
	void InputIDandSubject(int, Subject*); //멤버변수ID, subject에 값 대입
	void InputMember(Member *);            //멤버변수mList에 값 대입
	void ShowTitle() const;                //클래스ID와 그 클래스의 과목 출력
	void ShowInfo() const;                 //클래스의 모든정보 출력
	/*
	Class의 멤버 subject와 mList는 다른class의 멤버를 공유하므로 
	소멸자를통해 delete하려고하면 오류가 나게된다(소멸한곳을 또소멸하려해서 오류)
	MemberList Class에서 Member를 포인터로 받아서 공유해야
	학생/교수 구분이 가능해서 mList멤버는 MemberList클래스의 mList를 공유한다
	또한 공유하게되면 같은정보의 객체를 추가로 생성하지 않아도되서
	메모리 사용에 효율적이게된다
	그래서 subject멤버도 다른 클래스의 정보를 공유하게했다.
	*/
};
void Class::InputIDandSubject(int _ID, Subject *sub)
{
	ID = _ID;
	subject = sub;
}
void Class::InputMember(Member *mem)
{
	mList[index] = mem;
	index++;
}
void Class::ShowTitle() const
{
	cout.flags(ios::left);
	cout.width(11);
	cout<<ID;
	subject->ShowTitle();
}
void Class::ShowInfo() const
{
	cout<<"----------<Class Details>---------------"<<endl;
	cout<<"-Class ID: "<<ID<<endl;
	cout<<"-Subject: "; subject->ShowInfo();
	cout<<"-Class Members:"<<endl;
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
	cout<<"----------------------------------------"<<endl;
}

///////////////////////
//  ClassList Class  //
///////////////////////
class ClassList
{
private:
	Class *cList[50];
	int index;
public:
	ClassList() :index(0) {} //기본생성자
	void MakeClass();        //Class만들기
	void ShowClass() const;  //Class정보 보기
	~ClassList();            //소멸자
};
void ClassList::MakeClass()
{
	int sel;

	cList[index]=new Class(); //Class의 mList에 여러값을 넣어야하므로 클래스생성과 값 대입을 따로함
	while(1)
	{
		sl.ShowAll();
		cout<<"Enter subject ID to add >> "; cin>>sel;
		if(sel<1 || sel>4)
			cout << "==> " << sel << ": Invalid selection." << endl;
		else
			break;
	}
	cList[index]->InputIDandSubject(index+1, sl.ReturnSubject(sel));

	ml.ShowAll();
	while(1)
	{
		cout<<"Enter member ID (0 for no more) >> "; cin>>sel;
		if(sel==0)
			break;
		else
			cList[index]->InputMember(ml.ReturnMember(sel));
	}
	index++;

}
void ClassList::ShowClass() const
{
	int sel;

	if(index==0)
	{
		cout<<"==> No class registered..."<<endl;
		return;
	}

	cout<<"-----------------------"<<endl;
	cout<<"Class ID   Title"<<endl;
	cout<<"-----------------------"<<endl;
	for(int i=0; i<index; i++)
	{
		cList[i]->ShowTitle();
	}
	cout<<"-----------------------"<<endl;
	cout<<"Enter class ID: "; cin>>sel;
	cout<<endl;
	cList[sel-1]->ShowInfo();
}
ClassList::~ClassList()
{
	for(int i=0; i<index; i++)
		delete cList[i];
}
ClassList cl;

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
	cout << "  6. 반(Class) 구성"<< endl;
	cout << "  7. 반 구성원 출력"<< endl;
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
	sl.DefaultSubject(); //디폴트목록 넣기(전역객체 sl)

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
		case 6:
			cl.MakeClass();
			break;
		case 7:
			cl.ShowClass();
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