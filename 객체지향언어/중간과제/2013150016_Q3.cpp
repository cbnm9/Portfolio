/*
	��ǻ�Ͱ��а� 2013150016 ���μ�
	C++ �Ǳ����
	[Q3]�л������� ���������� �Է� �޾� ����� �����
	��û�� ȭ�鿡 ����ϴ� ���α׷� Q3.cpp
	-�л�/������ü�� ���������Ͽ� class MemberList��
	�Բ� 50������ ����
	-�������� Ȱ���Ͽ� ���
	-��ü�� ���ϴ� Ŭ���� �̸��� ã�� ��ȯ
	typeid(object).name() // class Ŭ�����̸�
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
	Member() {}                     //�⺻������
	Member(int, char*);             //������
	Member(const Member &);         //��������
	virtual void ShowInfo() const;  //�������, ������Ȱ��(virtual) 
	virtual ~Member();              //�Ҹ���
};
Member::Member(int _ID, char *_name)
	:ID(_ID)
{
	name = new char[strlen(_name) + 1];
	strcpy(name, _name);
}
Member::Member(const Member &copy)
	:ID(copy.ID)
{
	name = new char[strlen(copy.name)+1];
	strcpy(name, copy.name);
}
void Member::ShowInfo() const
{
	cout.flags(ios::left);   //��������
	cout.width(7);           //7ĭ Ȯ��
	cout << ID ;
	cout.width(8);           //8ĭȮ��
	cout<< name ;
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
	Student() {}                   //�⺻������
	Student(int, char *, char *);  //������
	Student(const Student &copy);  //��������
	void ShowInfo() const;         //����Լ� ������ ���, ����Ŭ�������� virtual���������Ƿ� �ڵ����� �����Լ�����
	~Student();                    //�Ҹ���, ���� ������������ �����Լ�
};
Student::Student(int _ID, char *_name, char *_major)
	:Member(_ID, _name)
{
	major = new char[strlen(_major) + 1];
	strcpy(major, _major);
}
Student::Student(const Student &copy)
	:Member(copy)
{
	major = new char[strlen(copy.major) + 1];
	strcpy(major, copy.major);
}
void Student::ShowInfo() const
{
	Member::ShowInfo();
	cout <<' '<< major << endl;
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
	char *department;                 //����
public:
	Professor() {}                    //�⺻������
	Professor(int, char*, char*);     //�̼���
	Professor(const Professor &copy); //��������
	void ShowInfo() const;            //�������, �� StudentŬ������ ������������ �����Լ�
	~Professor();                     //�Ҹ���
};
Professor::Professor(int _ID, char *_name, char *_department)
	:Member(_ID, _name)
{
	department = new char[strlen(_department)+1];
	strcpy(department, _department);
}
Professor::Professor(const Professor &copy)
	:Member(copy)
{
	department = new char[strlen(copy.department)+1];
	strcpy(department, copy.department);
}
void Professor::ShowInfo() const
{
	Member::ShowInfo();
	cout<<' '<<department<<endl;
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
	MemberList() :index(0) {}  //�⺻������
	void DefaultMember();      //����Ʈ��� �����
	void MakeStudent();        //Student��ü �����
	void MakeProfessor();      //Professor��ü �����
	void ShowStudent() const;  //�л����� ���
	void ShowProfessor() const;//�������� ���
	void ShowAll() const;      //��ü ���� ��� ���
	~MemberList();             //�Ҹ���
};
void MemberList::MakeStudent()
{
	char temp1[100]; //�̸��� 100���̳�
	char temp2[100];

	cout<<"====�л������Է�===="<<endl;
	cout<<"- ID: "<<index+1<<endl;
	cout<<"- Name: ";  cin>>temp1;
	cout<<"- Major: "; cin>>temp2;
	cout<<"===================="<<endl;

	mList[index]=new Student(index+1, temp1, temp2);
	index++;

	cout<<"==>> Added succesfully..."<<endl;
}
void MemberList::MakeProfessor()
{
	char temp1[100];
	char temp2[100];

	cout<<"====���������Է�===="<<endl;
	cout<<"- ID: "<<index+1<<endl;
	cout<<"- Name: ";       cin>>temp1;
	cout<<"- Department: "; cin>>temp2;
	cout<<"===================="<<endl;

	mList[index] = new Professor(index+1, temp1, temp2);
	index++;

	cout<<"==>> Added succesfully..."<<endl;
}
void MemberList::ShowStudent() const
{
	cout<<"-------------------------------"<<endl;
	cout<<"ID     Name     Major"<<endl;
	cout<<"-------------------------------"<<endl;

	for(int i=0; i<index; i++)
	{
		if(strcmp(typeid(*(mList[i])).name(), "class Student") == 0)
			mList[i]->ShowInfo();
	}
	cout<<"-------------------------------"<<endl;
}
void MemberList::ShowProfessor() const
{
	cout << "-------------------------------" << endl;
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
	cout<<"-------------------------------"<<endl;
	cout<<"ID     Name     Major/Department"<<endl;
	cout<<"-------------------------------"<<endl;

	for(int i=0; i<index; i++)
	{
		mList[i]->ShowInfo();
	}
	cout<<"-------------------------------"<<endl;
}
void MemberList::DefaultMember()
{
	mList[index] = new Student(index+1, "��ö��", "��ǻ�Ͱ�������"); index++;
	mList[index] = new Student(index+1, "ȫ�浿", "����Ʈ��������"); index++;
	mList[index] = new Student(index+1, "�赿��", "����Ʈ��������"); index++;
	mList[index] = new Student(index+1, "��ö��", "��ǻ�Ͱ�������"); index++;

	mList[index] = new Professor(index+1, "�̿���", "��ǻ�Ͱ�������"); index++;
	mList[index] = new Professor(index+1, "������", "��ǻ�Ͱ�������"); index++;
	mList[index] = new Professor(index+1, "���ϵ�", "��ǻ�Ͱ�������"); index++;
	mList[index] = new Professor(index+1, "������", "����Ʈ��������"); index++;
	mList[index] = new Professor(index+1, "���ο�", "����Ʈ��������"); index++;
}
MemberList::~MemberList()
{
	for(int i=0; i<index; i++)
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

	cout<<endl;
	cout<<"--------< �� �� >--------"<<endl;
	cout<<"  1. �л� �߰�"<<endl;
	cout<<"  2. ���� �߰�"<<endl;
	cout<<endl;
	cout<<"  3. �л���� ���"<<endl;
	cout<<"  4. ������� ���"<<endl;
	cout<<endl;
	cout<<"  9. ȭ������"<<endl;
	cout<<"  0. ����"<<endl;
	cout<<"-------------------------"<<endl;
	cout<<"  ����>>"; cin>>sel;
	cout<<endl;

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
	MainCtrl() { Main(); } //�����ڿ��� Main()ȣ�������μ� MainCtrl��ü ������ ���α׷� ����
};
int MainCtrl::Main()
{
	int sel;

	ml.DefaultMember();  //����Ʈ��� �ֱ�(������ü ml)

	while(1)
	{
		sel= mm.IssueMenu(); //���θ޴�(������ü mm)

		switch(sel)
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
		case 9:
			system("cls");
			break;
		case 0:
			return 0;
		default:
			cout<<"==> "<<sel<<": Invalid selection."<<endl;
		}
	}
}

MainCtrl mc;

int main() {}