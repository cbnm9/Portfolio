/*
	��ǻ�Ͱ��а� 2013150016 ���μ�
	C++ �Ǳ����
	[Q1]�ϳ��� �л�(Student)��ü�� �����,
	�̸� �����Ͽ� �� �ٸ� �л���ü�� �����
	�� ���� ��ü ������ ȭ�鿡 ����ϴ� ���α׷� Q1.cpp
	- �л��� �̸��� 100�� �̳��� ��������
	- ID�� ����
	- ���� ���縦 ���
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
	Student() {}                   //�⺻������
	Student(int, char *, char *);  //������
	Student(const Student &copy);  //��������
	void ShowInfo() const;         //����Լ� ������ ���
	~Student();                    //�Ҹ���
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
	Student stu1(101, "ȫ�浿", "��ǻ�Ͱ�������"); //Stu1��ü �����
	
	Student stu2(stu1);                            //stu1��ü�� �����Ͽ� stu2�� ����
	
	cout << "---------------------" << endl;
	cout << "ID  Name    Major" << endl;
	cout << "---------------------" << endl;
	stu1.ShowInfo();
	stu2.ShowInfo();
	cout << "---------------------" << endl;

	return 0;
}