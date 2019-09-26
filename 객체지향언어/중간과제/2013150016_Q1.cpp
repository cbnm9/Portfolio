/*
	컴퓨터공학과 2013150016 서민성
	C++ 실기과제
	[Q1]하나의 학생(Student)객체를 만들고,
	이를 복사하여 또 다른 학생객체를 만들어
	두 개의 객체 정보를 화면에 출력하는 프로그램 Q1.cpp
	- 학생의 이름은 100자 이내로 동적관리
	- ID는 정수
	- 깊은 복사를 사용
*/
#include<iostream>
#include<cstring>

using namespace std;


/////////////////////
//  Student Class  //
/////////////////////
class Student
{
private:
	int ID;
	char *name;
	char *major;
public:
	Student() {}                   //기본생성자
	Student(int, char *, char *);  //생성자
	Student(const Student &copy);  //깊은복사
	void ShowInfo() const;         //멤버함수 데이터 출력
	~Student();                    //소멸자
};
Student::Student(int _ID, char *_name, char *_major)
	:ID(_ID)
{
	name = new char[strlen(_name) + 1];
	strcpy(name, _name);
	major = new char[strlen(_major) + 1];
	strcpy(major, _major);
}
Student::Student(const Student &copy)
	:ID(copy.ID)
{
	name = new char[strlen(copy.name) + 1];
	strcpy(name, copy.name);
	major = new char[strlen(copy.major) + 1];
	strcpy(major, copy.major);
}
void Student::ShowInfo() const
{
	cout << ID << " " << name << "  " << major << endl;
}
Student::~Student()
{
	delete []name;
	delete []major;
}


int main()
{
	Student stu1(101, "홍길동", "컴퓨터공학전공"); //Stu1객체 만들기
	
	Student stu2(stu1);                            //stu1객체를 복사하여 stu2에 저장
	
	cout << "---------------------" << endl;
	cout << "ID  Name    Major" << endl;
	cout << "---------------------" << endl;
	stu1.ShowInfo();
	stu2.ShowInfo();
	cout << "---------------------" << endl;

	return 0;
}