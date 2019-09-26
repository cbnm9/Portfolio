/*
��ǻ�Ͱ��а� 2013150016 ���μ�
C++ �Ǳ����
[Q6]��(Class)�� ������, ����, �л����� �����ȴ�.
���� ����� ��ɰ� �ݺ��� ����ϴ� ����� �߰��� ���α׷�
Q6.cpp
-��(Class)�� ClassList�� 50������ ����
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
	virtual void ShowInfo() const;  //������� 
	virtual ~Member();              //�Ҹ���
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
	cout.flags(ios::left);   //��������
	cout.width(7);           //7ĭ Ȯ��
	cout << ID;
	cout.width(8);           //8ĭȮ��
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
	Student() {}                   //�⺻������
	Student(int, char *, char *);  //������
	Student(const Student &copy);  //��������
	void ShowInfo() const;         //����Լ� ������ ���
	~Student();                    //�Ҹ���
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
	char *department;                 //����
public:
	Professor() {}                    //�⺻������
	Professor(int, char*, char*);     //������
	Professor(const Professor &copy); //��������
	void ShowInfo() const;            //�������
	~Professor();                     //�Ҹ���
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
	MemberList() :index(0) {}  //�⺻������
	Member* ReturnMember(const int&); //��� ã�Ƽ� ����
	void DefaultMember();      //����Ʈ��� �����
	void MakeStudent();        //Student��ü �����
	void MakeProfessor();      //Professor��ü �����
	void ShowStudent() const;  //�л����� ���
	void ShowProfessor() const;//�������� ���
	void ShowAll() const;      //��ü ���� ��� ���
	~MemberList();             //�Ҹ���
};
Member* MemberList::ReturnMember(const int &sel)
{
	return mList[sel-1];
}
void MemberList::MakeStudent()
{
	char temp1[100]; //�̸��� 100���̳�
	char temp2[100];

	cout << "====�л������Է�====" << endl;
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

	cout << "====���������Է�====" << endl;
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
			cout << "�л�";
			mList[i]->ShowInfo();
		}
		else if (strcmp(typeid(*(mList[i])).name(), "class Professor") == 0)
		{
			cout.flags(ios::left);
			cout.width(8);
			cout << "����";
			mList[i]->ShowInfo();
		}
	}
	cout << "-----------------------------------------" << endl;
}
void MemberList::DefaultMember()
{
	mList[index] = new Student(index + 1, "��ö��", "��ǻ�Ͱ�������"); index++;
	mList[index] = new Student(index + 1, "ȫ�浿", "����Ʈ��������"); index++;
	mList[index] = new Student(index + 1, "�赿��", "����Ʈ��������"); index++;
	mList[index] = new Student(index + 1, "��ö��", "��ǻ�Ͱ�������"); index++;

	mList[index] = new Professor(index + 1, "�̿���", "��ǻ�Ͱ�������"); index++;
	mList[index] = new Professor(index + 1, "������", "��ǻ�Ͱ�������"); index++;
	mList[index] = new Professor(index + 1, "���ϵ�", "��ǻ�Ͱ�������"); index++;
	mList[index] = new Professor(index + 1, "������", "����Ʈ��������"); index++;
	mList[index] = new Professor(index + 1, "���ο�", "����Ʈ��������"); index++;
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
	Subject() {}				   //�⺻������
	Subject(int, char *);		   //������
	Subject(const Subject &copy);  //��������
	void ShowTitle() const;        //Title ���
	void ShowInfo() const;		   //�������
	~Subject();					   //�Ҹ���
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
	SubjectList() :index(0) {}				  //�⺻������
	Subject* ReturnSubject(const int&);       //���� ID�� �´� subject����
	void DefaultSubject();					  //����Ʈ��� �Է�
	void ShowSelectSubject(const int&) const; //�����Ѱ��� ���
	void ShowAll() const;					  //��� ���
};
Subject* SubjectList::ReturnSubject(const int &sel)
{
	return &(sList[sel-1]);
}
void SubjectList::DefaultSubject() //Subject��ü�� ����� �� ��ü�� sList �� ��Ұ� ����
{
	/*
		Subject��ü�� heap������ ������ ���縦 �����ϸ�
		��������ڰ� ȣ��� ���縦 �������پ˾Ҵµ�
		��������ڰ� ȣ������ʰ�
		��ü �迭�� heap������ ���� ��ü�� ���� �����ϴ°Ű���
		�׷��� �� �ΰ��� �Լ� ����� ��ü�� �������� �������°Ű���
		������?

		��ü�迭�� ���������� ÷���� ����ŭ ���������� ���������
		�׷��Ƿ� ��ü�迭 �� ��Ҵ� �����ڷ� �����־�
		��ü�� ������ �� ��ü�� �����ϴ��������� �ʱ�ȭ�ؾ��Ѵ�
		��ü�� �� �Լ����� �ʱ�ȭ�� �Լ��� ������ ����ؾ��ϹǷ�
		�Լ��� ������ ��ü�� �����ϵ��� heap������ ������ �����ؾ��Ѵ�
	*/
	sList[index] = *(new Subject(1, "��ü���� ���")); index++;
	sList[index] = *(new Subject(2, "������ ����"));   index++;
	sList[index] = *(new Subject(3, "�� ���α׷���")); index++;
	sList[index] = *(new Subject(4, "JAVA ����"));     index++;
}
void SubjectList::ShowSelectSubject(const int &sel) const
{
	cout << "------------------------" << endl;
	cout << "ID     �������" << endl;
	cout << "------------------------" << endl;
	sList[sel - 1].ShowInfo();
	cout << "------------------------" << endl;
}
void SubjectList::ShowAll() const
{
	cout << "------------------------" << endl;
	cout << "ID     �������" << endl;
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
	int ID;              //Ŭ���� ID
	Subject *subject;    //Ŭ������ ����
	Member *mList[50];   //Ŭ������ �л� �� ����
	int index;
public:
	Class() :index(0) {}                   //�⺻������
	void InputIDandSubject(int, Subject*); //�������ID, subject�� �� ����
	void InputMember(Member *);            //�������mList�� �� ����
	void ShowTitle() const;                //Ŭ����ID�� �� Ŭ������ ���� ���
	void ShowInfo() const;                 //Ŭ������ ������� ���
	/*
	Class�� ��� subject�� mList�� �ٸ�class�� ����� �����ϹǷ� 
	�Ҹ��ڸ����� delete�Ϸ����ϸ� ������ ���Եȴ�(�Ҹ��Ѱ��� �ǼҸ��Ϸ��ؼ� ����)
	MemberList Class���� Member�� �����ͷ� �޾Ƽ� �����ؾ�
	�л�/���� ������ �����ؼ� mList����� MemberListŬ������ mList�� �����Ѵ�
	���� �����ϰԵǸ� ���������� ��ü�� �߰��� �������� �ʾƵ��Ǽ�
	�޸� ��뿡 ȿ�����̰Եȴ�
	�׷��� subject����� �ٸ� Ŭ������ ������ �����ϰ��ߴ�.
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
			cout << "�л�";
			mList[i]->ShowInfo();
		}
		else if (strcmp(typeid(*(mList[i])).name(), "class Professor") == 0)
		{
			cout.flags(ios::left);
			cout.width(8);
			cout << "����";
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
	ClassList() :index(0) {} //�⺻������
	void MakeClass();        //Class�����
	void ShowClass() const;  //Class���� ����
	~ClassList();            //�Ҹ���
};
void ClassList::MakeClass()
{
	int sel;

	cList[index]=new Class(); //Class�� mList�� �������� �־���ϹǷ� Ŭ���������� �� ������ ������
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
	cout << "--------< �� �� >--------" << endl;
	cout << "  1. �л� �߰�" << endl;
	cout << "  2. ���� �߰�" << endl;
	cout << endl;
	cout << "  3. �л���� ���" << endl;
	cout << "  4. ������� ���" << endl;
	cout << "  5. ��ü��� ���" << endl;
	cout << endl;
	cout << "  6. ��(Class) ����"<< endl;
	cout << "  7. �� ������ ���"<< endl;
	cout << endl;
	cout << "  9. ȭ������" << endl;
	cout << "  0. ����" << endl;
	cout << "-------------------------" << endl;
	cout << "  ����>>"; cin >> sel;
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
	MainCtrl() { Main(); } //�����ڿ��� Main()ȣ�������μ� MainCtrl��ü ������ ���α׷� ����
};
int MainCtrl::Main()
{
	int sel;

	ml.DefaultMember();  //����Ʈ��� �ֱ�(������ü ml)
	sl.DefaultSubject(); //����Ʈ��� �ֱ�(������ü sl)

	while (1)
	{
		sel = mm.IssueMenu(); //���θ޴�(������ü mm)

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