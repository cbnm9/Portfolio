/*
��ǻ�Ͱ��а� 2013150016 ���μ�

1�� : �л�����(�̸�, ID, �ּ�, �ڵ�����ȣ)�� �Է¹޾� student.db���Ͽ� �����ϰ�,
student.db���Ͽ� ����� ������ ����Ϳ� ������ִ� ���α׷�.

2�� : ������ ����Ʈ�� ����ü�迭�� �Է��ϰ�, ����Ʈ�� �迭�� ����� ���� ���

3�� : 1. �޴� �����, �ǵ��ư��� ��� �߰�
	  2. �л����� �����ϴ� ��� �߰�
	  3. �л����� ����, Ž���� ����ϴ� ��� �߰�

*/
#include<stdio.h>    //printf, scanf�� �⺻���� ����� ���� ����
#include<string.h>   //���ڿ� ó���� ���� ����
#include<stdlib.h>   //�޸� �����Ҵ��� ���� ����
#include<time.h>     //�����߻� seed���� �ֱ����� ����

#define BUFF_LEN 150       //�����Է��Ҷ� �ӽ÷ξ� �迭�ִ����
#define ARY_LEN 200		   //�Է��� �л������� �ִ밪(�迭�� �������ִ� �ִ밹��)
#define HASH_ID_LEN 1000   //Hash Table ũ��(ID Ž����)
#define HASH_NAME_LEN 100  //Hash Table ũ��(Name Ž����)
#define NO_PHONE "No_Phone"//�ڵ����� ������ �ԷµǴµ�����
#define MAXNUM 2017999999  //�Է¹��� ID�� �ִ밪
#define MINNUM 2010000000  //�Է¹��� ID�� �ּҰ�
#define RAND_ID (double)rand()/RAND_MAX*(MAXNUM-MINNUM)+MINNUM
/*
  rand()%max �� �Ѵٸ� max�� ū���϶� �޺κ��� �պκк��� ����Ȯ���� �����Ե�
  rand�� ���������ҋ� �������� 0~1������ �Ǽ��� �����
  �ű⿡ �����ϰ���� �ִ��� ���ϸ�
  0~max ������ ���� ������Ȯ���� ����
  (double)rand()/RAND_MAX*(maxnum-minnum) = 0~(maxnum-minnum) �� �������
  ���� +minnum�� �ϸ� minnum~maxnum �� �������
*/

enum{ IN=10, TOUCH, OUT, EXIT, PREV, MENU }; //��ɼ���->{���θ����, �����ϱ�, ����ϱ�, ���α׷�����, �������� ���ư���, �޴��� ���ư��� }

//////////////////////////////////    ����ü ����    ///////////////////////////////////////////
                                                                                            ////
typedef struct {     //�ּ������� ���� ����ü												////
	char *state;     //��																	////
	char *city;      //��,��,��																////
	char *village;   //��,��,��('��'�Ʒ� '��'�� �ü����ִ�)									////
	char *street;    //���θ�																////
	char *home;      //���ּ�																////
}address;																					////
																							////
typedef struct {     //�л����� ������ ���� ����ü											////
	char *fname;     //firstname															////
	char *lname;     //lastname																////
	int id;          //student-ID															////
	address adr;     //address																////
	char *pnum;      //Phone-Number															////
}info;																						////
																							////
typedef struct list{       //�л��������������� ����Ʈ										////
	info student;          //�л�����														////
	struct list *flink;	   //�ڽź��� ���� ���� ��带 ����Ŵ								////
	struct list *llink;	   //�ڽź��� ���߿� ���� ��带 ����Ŵ							////
}list;																						////
																							////
////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////    �Լ�����    ////////////////////////////////////////////
                                                                                            ////
void cleanstdin();						   //ǥ���Է¹��� �����ִ� �Լ�                     ////
int ckalloc(void *);					   //�����Ҵ� Ȯ���ϴ� �Լ�                         ////
void ckno(char **);						   //���ڿ��Է½� NO�� �Է��ߴ��� Ȯ���ϴ� �Լ�     ////
void cleaninfo(info *);					   //Info�� �� ��� NULL�� �ʱ�ȭ					////
void prtline(info);						   //�Է������� Ȯ���� ���							////
int excname(char **);					   //����, ����, Ư������ ����ó��					////
int excaddr(char **);					   //����, Ư������ ����ó��						////
int excphone(char **);					   //����, -�� ������ ���� ����ó��					////
void sortbyname(info *, int, int);		   //�̸��� ����									////
void sortbyid(info *, int, int);		   //ID�� ����										////
int inpsearchname(char **, char **);	   //Ž���� ���� �̸��Է¹ޱ�						////
void inpsearchid(int *);				   //Ž���� ���� id�Է¹ޱ�							////
                                                                                            ////
void issuemenu();						   //�޴� �����ֱ�									////
int intro();							   //���α׷� ������ �ȳ����ڿ� ����ϴ� �Լ�       ////
int checksure(int);						   //������ ����� Ȯ������ Ȯ���ϱ�				////
void checkmenu(int);					   //�� �Լ����� ���̴� print�� ���				////
																							////
int inputinfo(int);						   //������ �Է¹����� ���Ͽ� ���� �Լ�             ////
int inputrepeat(list *, list **, int *, int *);//Ű����� �����Է��ϴ°� �ݺ����ִ� �Լ�	////
int inputkeyboard(list *, list **);		   //Ű����� �Է¹޴� �Լ�							////
void ckmoreinput(char *);				   //���� �� �Է��Ұ��� �����Լ�					////
																							////
int strinp(char **);					   //Ű����� �Է¹޴��Լ�                          ////
int inpfirstname(list *);				   //FirstName �Է��ϴ� �Լ�                        ////
int inplastname(list *);				   //LastName �Է��ϴ� �Լ�							////
void randomid(list *, list *);			   //ID ���������ϴ� �Լ�                           ////
																							////
int inpaddress(list *);				       //�ּ� �Է��ϴ� �Լ�                             ////
int inpaddressone(list *);				   //ù��° �ּ��Է�								////
int inpaddresstwo(list *);				   //�ι�° �ּ��Է�								////
int inpaddressthree(list *);			   //����° �ּ��Է�								////
int inpaddressfour(list *);				   //�׹�° �ּ��Է�								////
int inpaddressfive(list *);				   //������ �ּ��Է�								////
																							////
int inppnum(list *, list *);               //��ȭ��ȣ �Է��ϴ� �Լ�                         ////
																							////
void cmpinfolen(info, int *, int *);	   //�̸�, �ּ� �ִ���� ��ŭ ���鹮�� �־��ִ��Լ� ////
int lenname(info);		                   //�̸����� ���                                  ////
int lenadrs(address);	                   //�ּұ��� ���                                  ////
																							////
int issueretouchmenu();					   //������ ��� �����ϴ� �޴�						////
int retouchfile();						   //���� ����										////
char checkmoreretouch();				   //�� �����Ұ��� ����								////
int deleteinfo(list *);					   //����											////
int addinfo(list *);					   //�߰�											////
int retouchinfo_search(list *);			   //������ �׸� ����								////
int retouchitemselect();				   //� �׸� ������������ �޴�					//// 
int retouchinfo_retouch(list *, list *);   //������ �׸� ����								////
int retouch_name(list *);				   //�̸� ���Է�									////
int retouch_address_menu();				   //������ �ּ��׸� ����							////
																							////
int retouch_address(list *);			   //�ּ� ����										////
int retouch_address_one(list *, list **);  //ù��° �ּ� ����								////
int retouch_address_two(list *, list **);  //�ι�° �ּ� ����								////
int retouch_address_three(list *, list **);//����° �ּ� ����								////
int retouch_address_four(list *, list **); //�׹�° �ּ� ����								////
int retouch_address_five(list *, list **); //������ �ּ� ����								////
																							////
int retouch_phonenumber(list *, list *);   //��ȭ��ȣ ���Է�								////
																							////
int issuedisplaymenu();					   //����ϴ� ��� ���� �޴�						////
void displayselect(int);				   //����ϴ� ��� �����ϴ� �Լ�					////
void displaytosort(int);				   //������ ����ϴ� �Լ�							////
																							////
int displaysearch();					   //Ž���ؼ� ����ϴ� �Լ�							////
int searchselect();						   //Ž���� ��� ����								////
void displaysearchedinfo(list *);		   //������ ���� ���								////
char checkmoresearch();					   //�� Ž������ Ȯ��								////
																							////
void displayinfo(FILE *);				   //������ �����͸� ����Ϳ� ����ϴ� �Լ�         ////
                                                                                            ////
void listtofile(list *, int, int);		   //list���� ���Ϸ� ������ ����				    ////
void headtofile(FILE **, int, int);		   //���� �������κ� ���Ͽ� ����					////
void nametofile(FILE **, list **, int);	   //Name ���Ͽ� ����								////
void idtofile(FILE **, list **);		   //ID ���Ͽ� ����									////
void addresstofile(FILE **, list **, int); //Address ���Ͽ� ����							////
void phonenumbertofile(FILE **, list **);  //PhoneNumber ���Ͽ� ����						////
																							////
void arraytofile(info *, int, int, int);   //�迭���� ���Ϸ� ������ ����					////
void arynametofile(FILE **, info *, int);  //Name ���Ͽ� ����								////
void aryidtofile(FILE **, info *);		   //ID ���Ͽ� ����									////
void aryaddresstofile(FILE **, info *, int);//Address ���Ͽ� ����							////
void aryphonenumbertofile(FILE **, info *);//PhoneNumber ���Ͽ� ����						////
																							////
int filetolist(list *);					   //���Ͽ��� list�� ������ �б�                    ////
int nametolist(FILE **, list **);		   //Name ���Ͽ��� �б�								////
void idtolist(FILE **, list **);		   //ID ���Ͽ��� �б�								////
int addresstolist(FILE **, list **);	   //Address ���Ͽ��� �б�							////
int phonenumbertolist(FILE **, list **);   //PhoneNumber ���Ͽ��� �б�						////
void checklentolist(list **, int *, int *);//�̸�, �ּ� ���̱��ϱ�							////
																							////
int filetoarray(info *, int *);			   //���Ͽ��� �迭�� ������ �б�					////
int nametoarray(FILE **, info **, int);	   //Name ���Ͽ��� �б�								////
void idtoarray(FILE **, info **, int);	   //ID ���Ͽ��� �б�								////
int addresstoarray(FILE **, info **, int); //Address ���Ͽ��� �б�							////
int pnumbertoarray(FILE **, info **, int); //PhoneNunber ���Ͽ��� �б�						////
void checklentoarray(info *, int, int *, int *);//�̸�, �ּ� ���̱��ϱ�						////
																							////
void freelist(list *);                     //����Ʈ �����Ҵ� �����ϱ�                       ////
void freearray(info *, int);			   //�迭 �����Ҵ� �����ϱ�							////
void freename(info *);					   //name�� �����Ҵ�����κ� ����					////
void freeaddress(info *);				   //address�� �����Ҵ�����κ� ����				////
void freephonenumber(info *);			   //��ȭ��ȣ�� �����Ҵ�����κ� ����				////
																							////
void inputlist(list *, list *);			   //list�� ����߰��ϱ�							////
list* findlistbyname(list *, char *, char *);//�̸����� Ž��								////
int chooseone(list **, int);			   //���� ���������� �Ѹ� ����						////
list* findlistbyid(list *, int);		   //ID�� Ž��										////
void deletelist(list *);				   //����ϳ� ����									////
																							////
int filetohash(list **, int);				//���Ͽ��� hash�� ������ �б�					////
void clearhashtable(list **, int);				//hash table �ʱ�ȭ							////
int hashfunctionbyname(char *);				//Name�� ���� key�� ã��						////
int hashfunctionbyid(int);					//ID�� ���� key�� ã��							////
void inputhash(list **, list *, int);		//hash table�� ������ �ֱ�						////
list* hashingbyname(list **, char *, char *);//Name���� Ž��								////
void minisort(list *, list **);				//Ž���� �������ε� ID�� ����					////
list* hashingbyid(list **, int);			//ID�� Ž��										////
void freehash(list **, int);				//�ؽ����̺� �����Ҵ� ����						////
																							////
////////////////////////////////////////////////////////////////////////////////////////////////

int main(void)
{
	int cdt;                    //��ɺ� ���α׷������� ��Ʈ�����ִ� ����

	while (1)                   //���α׷� �����Ҷ����� �ݺ�
	{
		cdt = intro();          //���α׷����� �� �ȳ����ִ� ���ڿ� ���
		if (cdt == EXIT) break; //�����Ѵٰ� �����ϸ� ���α׷�����
		cdt = inputinfo(cdt)  ; //�л����� student.db���Ͽ� �Է�
		displayselect(cdt);     //student.db������ ������ ����Ϳ� ���
	}

	return 0;
}

/////////////////////////////////////     �Լ� ����     ////////////////////////////////////////
                                                                                            ////
void cleanstdin()              //Ű���� �Է¹��� ����
{
	while (getchar() != '\n'); //�Է¹��ۿ� �ִ� ������'\n'���� ������
}

int ckalloc(void *vp) //void������ ��������μ� �پ��� �ڷ����� �� �Լ� �ϳ��� �˻簡��
{
	if (vp == NULL)   //�����Ҵ翡 �����ߴٸ� 0��ȯ
	{
		printf("�����Ҵ��� �����߽��ϴ�.\n");
		return 0;
	}

	return 1;		  //�����Ҵ翡 �����ߴٸ� 1��ȯ
}

void ckno(char **ckn)            //�����Է½� ���ϴºκ�(n�Է��Ѻκ�)Ȯ�����ִ� �Լ�
{
	if (strcmp(*ckn, "n") == 0)  //���� "n"�� �Է��ߴٸ�
	{
		free(*ckn);              //�ش� �޸𸮸� ��ȯ�Ѵ�
		*ckn = NULL;             //�׸��� NULL�����͸� �־��ش�
	}
}

void cleaninfo(info *p)     //�� ��� �⺻�� NULL�� �����ϱ�(���� ������ �� ���free�Լ��� ����)
{
	p->fname = NULL;
	p->lname = NULL;
	p->adr.state = NULL;
	p->adr.city = NULL;
	p->adr.village = NULL;
	p->adr.street = NULL;
	p->adr.home = NULL;
	p->pnum = NULL;
}

void prtline(info pl) //�л����� ���ٿ����
{
	char *h;
	int i = 0;
	h=pl.adr.home;

	printf("%s %s   %d   ", pl.fname, pl.lname, pl.id);

	if (pl.adr.state)
		printf("%s ", pl.adr.state);
	if (pl.adr.city)
		printf("%s ", pl.adr.city);
	if (pl.adr.village)
		printf("%s ", pl.adr.village);
	if (pl.adr.street)
		printf("%s ", pl.adr.street);

	while(1)
	{
		if(h[i]=='\0')
			break;
		else
		{
			if(h[i]=='_')
				printf("%c", ' ');
			else
				printf("%c", h[i]);
		}
		i++;
	}

	printf("   %s\n", pl.pnum);
}

int excname(char **name)		//�̸� �� 1~3��° �ּ� �Է¿� ���� ����Ž�� �Լ�
{
	int len = strlen(*name);	//Ž���� ���ڼ�
	int i;						//�ݺ��� ����
	char t;						//���Ҷ� ���ϰ� �ϱ����� ����

	for (i = 0; i < len; i++)	//���鹮��(�����̽� or Tap) �� ������ 2��ȯ
	{
		t = (*name)[i];
		if (t==' '||t=='\t')	//t�� ���鹮�� �Ǵ� �ǹ����ϋ� if�� �����
			return 2;
	}

	for (i = 0; i < len; i++)	//�ѱ��� �ƴѵ� ��� �ƴҶ� 3 ��ȯ
	{
		t = (*name)[i];
		if (((t & 0x80) == 0) && !(t>='a'&&t<='z' || t>='A'&&t<='Z'))
			return 3;
	}

	return 1;	//������ return �������� ���� ���°�
}

int excaddr(char **addr)		//���θ�/���� �ּҿ� ���� ����Ž�� �Լ�
{
	int len = strlen(*addr);	//Ž���� ���ڼ�
	int i;						//�ݺ��� ����
	char t;						//���Ҷ� ���ϰ� �ϱ����� ����

	for (i = 0; i < len; i++)	//���鹮��(�����̽� or Tap) �� ������ 2��ȯ
	{
		t = (*addr)[i];
		if (t == ' ' || t == '\t')	//t�� ���鹮�� �Ǵ� �ǹ����ϋ� if�� �����
			return 2;
	}

	for (i = 0; i < len; i++)		//�ѱ� ���� ���� - ��� �ƴҶ� 3��ȯ
	{
		t = (*addr)[i];
		if (((t & 0x80) == 0) && !(t >= 'a'&&t <= 'z' || t >= 'A'&&t <= 'Z'))
			if (!(t >= '0'&&t <= '9') && (t != '-'))
				return 3;
	}

	return 1;	//������ return �������� ���� ���°�
}

int excphone(char **pn)			//��ȭ��ȣ�� ���� ����Ž�� �Լ�
{
	int len = strlen(*pn);
	int i;
	char t;

	if ((len == 1) && ((*pn)[0] == 'n')) //��ȭ�Ⱑ ���ٴ� �Է��� ��������
		return 1;

	for (i = 0; i < len; i++)			 //���鹮��(�����̽� or Tap) �� ������ 2��ȯ
	{
		t = (*pn)[i];
		if (t == ' ' || t == '\t')
			return 2;
	}

	for (i = 0; i < len; i++)		//���ڶ� - �� �ٸ��� ������ 3��ȯ
	{
		t = (*pn)[i];
		if (!(t >= '0'&&t <= '9') && (t != '-'))
			return 3;
	}

	return 1;	//�����Է½� 1��ȯ
}

void sortbyname(info *ary, int left, int right)
{
	info p = ary[left];		//pivot�� ���� ���ʿ��, �̶� ary[left]�� ������̳� �ٸ�����
	int l = left;			//left��� l���(�Ʒ����� left����ϹǷ� ���� ���ϸ�ȵ�)
	int r = right;			//right��� r���

	while (l < r)			//l<r�϶��� �ݺ�������(l=r�Ǹ� ����)
	{
		while ((l < r))
		{
			if(strcmp(ary[r].fname, p.fname)==0)		//fname�� ������ lname���� ��
			{
				if(strcmp(ary[r].lname, p.lname)<0)		//lname���� �������� �׳� �Ѿ(������ ������)
					break;
			}
			else if(strcmp(ary[r].fname, p.fname)<0)
				break;

			r--;
		}

		if (l != r)							//�̰� ������ٴ°��� ary[r]<pivot�̶�°�
		{
			ary[l] = ary[r];				//������� ary[l]�� ary[r]�ǰ�������, ary[r]�� ������̵�
		}

		while ((l < r))
		{
			if(strcmp(ary[l].fname, p.fname)==0)
			{
				if(strcmp(ary[l].lname, p.lname)>0)
					break;
			}
			else if(strcmp(ary[l].fname, p.fname)>0)
				break;

			l++;
		}

		if (l != r)							//�̰� ������ٴ°��� ary[l]>pivot�̶�°�
		{
			ary[r] = ary[l];				//������� ary[r]�� ary[l]���� ����, ary[l]�� ������̵�
			r--;							//���⼭ �̸�r���� ���̸� ���� while�� ���ǰ˻� ���ص���
		}
											//���� �� �� if���� ��������ʾҴٴ°��� l==r�̶�°��̰�, ary[l]=ary[r]=�����
	}

	ary[l] = p;								//���������� ������� pivot�� ������ pivot ������ ������, �������� ū���� �ְԵ�
	//p = l;								//�̶� index l�� pivot�� ��ġ�� ����Ŵ

	if (left < l)							//pivot�� ��ġ�� left���� �������̶�� (pivot ���ʿ� ���� �����Ҷ���)
		sortbyname(ary, left, l - 1);		//���ʿ����� �����Ľ���
	if (right > l)							//pivot�� ��ġ�� right���� �����̶�� (pivot �����ʿ� ���� �����Ҷ���)
		sortbyname(ary, l + 1, right);		//�����ʿ����� �����Ľ���
}

void sortbyid(info *ary, int left, int right)
{
	info p = ary[left];		//pivot�� ���� ���ʿ��, �̶� ary[left]�� ������̳� �ٸ�����
	int l = left;			//left��� l���(�Ʒ����� left����ϹǷ� ���� ���ϸ�ȵ�)
	int r = right;			//right��� r���

	while (l < r)			//l<r�϶��� �ݺ�������(l=r�Ǹ� ����)
	{
		while ((ary[r].id >= p.id) && (l < r))	//l==r�� �ǰų�, ary[r]�� pivot���� �������϶� �׸�
			r--;

		if (l != r)							//�̰� ������ٴ°��� ary[r]<pivot�̶�°�
		{
			ary[l] = ary[r];				//������� ary[l]�� ary[r]�ǰ�������, ary[r]�� ������̵�
		}

		while ((ary[l].id <= p.id) && (l < r))	//l==r�� �ǰų�, ary[l]�� pivot���� ū���϶� �׸�
			l++;

		if (l != r)							//�̰� ������ٴ°��� ary[l]>pivot�̶�°�
		{
			ary[r] = ary[l];				//������� ary[r]�� ary[l]���� ����, ary[l]�� ������̵�
			r--;							//���⼭ �̸�r���� ���̸� ���� while�� ���ǰ˻� ���ص���
		}
											//���� �� �� if���� ��������ʾҴٴ°��� l==r�̶�°��̰�, ary[l]=ary[r]=�����
	}

	ary[l] = p;								//���������� ������� pivot�� ������ pivot ������ ������, �������� ū���� �ְԵ�
	//p = l;								//�̶� index l�� pivot�� ��ġ�� ����Ŵ

	if (left < l)							//pivot�� ��ġ�� left���� �������̶�� (pivot ���ʿ� ���� �����Ҷ���)
		sortbyid(ary, left, l - 1);			//���ʿ����� �����Ľ���
	if (right > l)							//pivot�� ��ġ�� right���� �����̶�� (pivot �����ʿ� ���� �����Ҷ���)
		sortbyid(ary, l + 1, right);		//�����ʿ����� �����Ľ���
}

int inpsearchname(char **fname, char **lname) //Ž���� ���� �̸� �Է�
{
	system("cls");
	printf("==< Ž���� ���� �̸� �Է� >==\n");
	printf("@@ ���� �� �ѱ۸� �Է°����մϴ�. @@\n\n");
	while (1)
	{
		printf("%-15s", "First Name(��)"); printf("%c", ':');
		if (!strinp(fname))  //�����Ҵ� �����ߴٸ� 0��ȯ
		{
			return 0;
		}
		if (excname(fname) != 1)			//�� �Է¿� ���� ����ó��, ���ܰ� �߻��ߴٸ�
		{
			printf("�Է� : %s\n", *fname);
			printf("���� ���� �� �ѱ� �� ����, ���� �Ǵ� ������ ���ԵǾ����ϴ�.\n");
			printf("�ٽ� �Է��� �ֽʽÿ�.\n\n");
			free(*fname);
		}
		else												//���������� �ԷµǾ��ٸ�
			break;
	}
	while (1)
	{
		printf("%-15s", "Last Name(�̸�)"); printf("%c", ':');
		if (!strinp(lname))  //�����Ҵ� �����ߴٸ� 0��ȯ
		{
			free(*fname);
			return 0;
		}
		if (excname(lname) != 1)			//�̸� �Է¿� ���� ����ó��, ���ܰ� �߻��ߴٸ�
		{
			printf("�Է� : %s\n", *lname);
			printf("���� ���� �� �ѱ� �� ����, ���� �Ǵ� ������ ���ԵǾ����ϴ�.\n");
			printf("�ٽ� �Է��� �ֽʽÿ�.\n\n");
			free(*lname);
		}
		else												//���������� �ԷµǾ��ٸ�
			break;
	}
	return 1; //���������� �ԷµǾ��ٸ�
}

void inpsearchid(int *id) //Ž���� ���� �й� �Է�
{
	system("cls");
	printf("==< Ž���� ���� �й� �Է� >==\n");
	printf("@@ ���ڸ� �Է°����մϴ�. @@\n");
	printf("@@ ����� �� ���Ĵ� �Էµ��� �ʽ��ϴ�. @@\n\n");
	while (1)
	{
		printf("  �й� : ");
		scanf("%d", id);
		cleanstdin();
		if (*id >= MINNUM && *id <= MAXNUM)
			break;
		else
		{
			printf("%d : �߸��� �Է��Դϴ�.\n", *id);
			printf("�ٽ� �Է��� �ֽʽÿ�.\n\n");
		}
	}
}

void issuemenu() //Main-Menu
{
	printf("====< �л� ���� ���� ���α׷� >====\n\n");
	printf("  1. �л����� ���θ����\n");
	printf("\n");
	printf("  2. �л����� �����ϱ�\n");
	printf("  3. �л����� ����ϱ�\n");
	printf("\n");
	printf("  0. ���α׷� �����ϱ�\n\n");
	printf("===================================\n");
}

int intro()   //���α׷��� ���۽� �޴������ϴ°�
{
	int cdt;  //��� �����ϱ�

	while (1)  //��ɼ��� �ùٸ����Ҷ����� �ݺ�
	{
		issuemenu(); //�޴� �����ֱ�

		printf(" �۾� ����>> "); 
		scanf("%d", &cdt);
		cleanstdin();           //scanf�� �Է��� ���� ��������� ���鹮�ڿ��� ����

		switch (cdt)            //switch-case ����Ͽ� ������ ���� �� �Էµ��°�� �߸��� ����ó��
		{
		case 1:                 //�Է��ϰڴٰ� ���ý�
			if(checksure(cdt))	//������ ��� �������� Ȯ��
				return IN;
			else
				break;
		case 2:                 //��������
			if(checksure(cdt))
				return TOUCH;
			else
				break;
		case 3:                 //��¼���
			if(checksure(cdt))
				return OUT;
			else
				break;
		case 0:					//���ἱ��
			return EXIT;
		default:                //������ ��� �� �̻��� �Է� �������� �������� ���ư�
			printf("�߸��� �����Դϴ�.\n");
			break;
		}
		system("cls");
	}
}

int checksure(int cdt)	//Ȯ���� �������� ����
{
	char sel;

	checkmenu(cdt);	//�� ���ÿ� ���� ���� ���

	while (1)
	{
		printf("��� �Ͻðڽ��ϱ�? (y/n) : ");
		scanf("%c", &sel);
		cleanstdin();        //scanf�� �Է��� ���� ��������� ���鹮�ڿ��� ����

		if (sel == 'y')
		{
			return 1;
		}
		else if (sel == 'n') //n���ý� �ٽ� �޴��� ���ư���
			return 0;
		else
		{
			printf("�߸��� �����Դϴ�. y�Ǵ� n�� �������ּ���\n");
		}
	}
}

void checkmenu(int cdt)	//�� ���ÿ� ���� ����
{
	system("cls");
	switch(cdt)
	{
	case 1:
		printf("==========< ���� ����� ���� >==========\n\n");
		printf(" ���� student.db������ �����˴ϴ�.\n\n");
		break;
	case 2:
		printf("==========< ���� �����ϱ� ���� >=========\n\n");
		printf(" �����߰�, ����, �����۾��� �����մϴ�.\n\n");
		break;
	case 3:
		printf("==========< ���� ����ϱ� ���� >=========\n\n");
		printf(" student.db������ ������ ȭ�鿡 ����մϴ�.\n\n");
		break;
	default:
		printf("�׷����� �����ϴ�.\n");
		break;
	}
}

int inputinfo(int a)      //���α׷��� �����Է�, ���Ͽ� ���� ����ϴºκ�
{
	list head;            //head���(head.llink=ù���, head.flink=���������)
	list *temp;           //�ӽó��
	int namelen=0;		  //�̸��� ������ ĭ��
	int addresslen=0;     //�ּҿ� ������ ĭ��

	head.llink = &head;   //list�� ���� �� ���
	head.flink = &head;   //list�� ���� �� ���

	srand(time(NULL));    //�����߻� seed�� �ٲ��ֱ�

	if(a==TOUCH)		  //���� �����ϱ� �����ߴٸ�
	{
		a=retouchfile();  //�����ϱ� �Լ� ȣ��
		if (a == PREV || a == MENU) //�����ϱ��Լ����� ���� �Ǵ� �޴��� ���ư��� ���Ѵٸ�
			return MENU;			//�޴��� ���ư���(�����ϱ� ����=�޴�)
		else
			return IN;	  //�����ߴٸ� IN�� ������ �������� ������ֱ�
	}
	else if(a==OUT)		  //����ϱ� �����ߴٸ� �ٷ� ������ΰ���
		return OUT;

	temp = (list *)malloc(sizeof(list)); //����Ʈ ù��� ����
	if (!ckalloc(temp))					 //�����Ҵ� Ȯ��, ���н� -1��ȯ(�޴��� ���ư���)
	{
		printf("��ġ : temp�Ҵ�\n\n");
		return -1;
	}

	if (inputrepeat(&head, &temp, &namelen, &addresslen) == -1) //�л����� �ݺ��Է�, ���н�
	{
		listtofile(&head, namelen, addresslen); //���������� �Է������� ���Ͽ��ְ�
		return -1;								//-1��ȯ
	}

	listtofile(&head, namelen, addresslen);   //����Ʈ���� ���Ϸ� �����;���

	return IN; //�Է������� 1��ȯ
}

int inputrepeat(list *head, list **temp, int *namelen, int *adrlen)
{
	char cdt;		   //�л����� �� �Է��Ұ��� ����� ���ǹ��� ���� ����

	while (1)          //�л����� �ݺ��Է��Ͽ� list�� ����
	{
		system("cls"); //ȭ�� ����ֱ�

		printf("***�л������Է�***\n");

		cleaninfo(&((*temp)->student)); //�Է��ϱ��� ����� NULL�� �ʱ�ȭ���ֱ�

		if (inputkeyboard(head, temp) == -1) //Ű����� �Է�, �����ϸ� -1��ȯ
			return -1;

		inputlist(head, *temp); //List�� ���ο� ������

		cmpinfolen((*temp)->student, namelen, adrlen); //�̸�, �ּұ��� ��

		system("cls");

		printf("���Ͽ� �̷��� �Էµ˴ϴ�.\n\n");
		prtline((*temp)->student); //��� �߰��� ���� ���ٷ� ������
		*temp = NULL;              //temp �ٽ� NULL�� ����ֱ�

		ckmoreinput(&cdt); //�л����� �� �Է��Ұ��� ����
		if (cdt == 'n')
			break;         //�л����� �� �Է����� �����Ÿ� �ݺ��� Ż���ϱ�

		*temp = (list *)malloc(sizeof(list)); //���ο� ��� �����
		if (!ckalloc(*temp))                  //�����Ҵ� Ȯ��, ���н� -1��ȯ
		{
			printf("��ġ : temp�Ҵ�\n\n");
			return -1;
		}
	}
	return 1; //������ 1��ȯ
}

int inputkeyboard(list *head, list **temp)	 //Ű����� ���� �Է¹޴� �Լ�
{
	if (!inpfirstname(*temp))				 //FirstName�Է�, ���н� -1��ȯ
	{
		printf("��ġ : FirstName�Ҵ�\n\n");
		free(*temp);						 //���н� temp��ȯ
		return -1;
	}
	if (!inplastname(*temp))				 //LastName�Է�, ���н� -1��ȯ
	{
		printf("��ġ : LastName�Ҵ�\n\n");
		freename(&((*temp)->student));		 //���н� temp�� fname��ȯ �� temp��ȯ
		free(*temp);
		return -1;
	}
	randomid(head, *temp);					 //ID �Է�		
	if (!inpaddress(*temp))                  //Address�Է�, ���н� -1��ȯ
	{
		printf("��ġ : Address�Ҵ�\n\n");
		freename(&((*temp)->student));       //���н� �̸�, �ּ� �Է��Ѱű��� ��ȯ �� temp��ȯ
		freeaddress(&((*temp)->student));
		free(*temp);
		return -1;
	}
	if (!inppnum(head, *temp))               //PhoneNumber�Է�, ���н� -1��ȯ
	{
		printf("��ġ : PhoneNumber�Ҵ�\n\n");
		freename(&((*temp)->student));		 //���н� �̸�, �ּҹ�ȯ�� temp��ȯ
		freeaddress(&((*temp)->student));
		free(*temp);
		return -1;
	}
	return 1; //������ 1��ȯ
}

void ckmoreinput(char *cdt)
{
	while (1) //�л����� �� �Է����� ����
	{
		printf("�л������� �� �Է� �Ͻðڽ��ϱ�?(y/n) : ");
		scanf("%c", cdt);
		cleanstdin();			  //scanf�� �Է¹ް��� ���������ֱ�

		if (*cdt == 'y' || *cdt == 'n') //y�Ǵ�n����� �ԷµǾ����� Ȯ��
			break;                      //����� �Է����� �ݺ��� Ż��
		else
			printf("y �Ǵ� n �� �Է��� �ֽʽÿ�.\n");
	}
}

int strinp(char **inp)                        //Ű����κ��� �Է¹޾Ƽ� ����ü����� �������ִ��Լ�
{
	char buff[BUFF_LEN];                      //�̸�, �ּ�, ��ȭ��ȣ �̰ɷι޾Ƽ� �˳��ѹ迭 ����
	int wp = 1;

	while (wp)
	{
		wp = 0;                               //�⺻������ �ݺ�����
		fgets(buff, sizeof(buff), stdin);     //���鹮�� �ޱ����� fgets �Լ� ���
		if (buff[strlen(buff) - 1] == '\n')   //���ڿ� ���� �� ���๮�ڸ� �ι��ڷ� �ٲ���(fgets�� ���๮�ڱ��� ������)
			buff[strlen(buff) - 1] = '\0';
		else if (strlen(buff) >= BUFF_LEN)    //���� ���ڿ��� buff�� ������(��ĥ��) �ٽ��Է¹޾ƾ���->�ݺ�
		{
			printf("�ʹ� �� ���ڿ��� �ԷµǾ����ϴ�.\n");
			printf("�ٽ� �Է��� �ֽʽÿ�.\n\n");
			printf("���Է� : ");
			wp = 1;
		}

		if (strlen(buff) <= 0)				   //���� �Է¾��� ���͸� �������
		{
			printf("���ڿ��� �Է��� �ֽʽÿ�.\n\n");
			printf("���Է� : ");
			wp = 1;
		}
	}
	*inp = (char *)malloc(strlen(buff) + 1);  //���ڿ����� +1��ŭ �����Ҵ�
	if (!ckalloc(*inp))                       //�����Ҵ��� ���������� 0��ȯ�� !�����ڿ����� if�� �����
	{
		return 0;                             //0��ȯ : �����Ҵ� ����
	}

	strcpy(*inp, buff);                       //���ڿ��� ����ü�� �����س���

	return 1;                                 //1��ȯ : �����Ҵ� ����
}

int inpfirstname(list *temp)              //firstname�Է�
{
	printf("<1> �̸��Է�\n");
	printf("@@ ���� �̸��� ���� �� �ѱ۸� �Է°����մϴ�. @@\n");
	printf("@@ ���� �̸��� ������ ���Խ�ų�� �����ϴ�. @@\n\n");

	while (1)
	{
		printf("%-15s", "First Name(��)"); printf("%c", ':');
		if (!strinp(&(temp->student.fname)))  //�����Ҵ� �����ߴٸ� 0��ȯ
		{
			return 0;
		}
		if (excname(&(temp->student.fname)) != 1)			//�� �Է¿� ���� ����ó��, ���ܰ� �߻��ߴٸ�
		{
			printf("�Է� : %s\n", temp->student.fname);
			printf("���� ���� �� �ѱ� �� ����, ���� �Ǵ� ������ ���ԵǾ����ϴ�.\n");
			printf("�ٽ� �Է��� �ֽʽÿ�.\n\n");
			free(temp->student.fname);
		}
		else												//���������� �ԷµǾ��ٸ�
			break;
	}

	return 1;                             //�Է� ������ 1��ȯ
}

int inplastname(list *temp)               //lastname �Է�
{
	while (1)
	{
		printf("%-15s", "Last Name(�̸�)"); printf("%c", ':');
		if (!strinp(&(temp->student.lname)))  //�����Ҵ� �����ߴٸ� 0��ȯ
		{
			return 0;
		}
		if (excname(&(temp->student.lname)) != 1)		//�̸� �Է¿� ���ܰ� �߻��ߴٸ�
		{
			printf("�Է� : %s\n", temp->student.lname);
			printf("�̸��� ���� �� �ѱ� �� ����, ���� �Ǵ� ������ ���ԵǾ����ϴ�.\n");
			printf("�ٽ� �Է��� �ֽʽÿ�.\n\n");
			free(temp->student.lname);
		}
		else											//���������� �ԷµǾ��ٸ�
			break;
	}

	return 1;                             //�Է� ������ 1��ȯ
}

void randomid(list *head, list *temp)  //ID�����߻�, �Է��ϴ� �Լ�
{
	list *p;  //��� Ž�������� ����Ʈ������
	int ck;

	while (1)
	{
		temp->student.id = RAND_ID; //minnum���� maxnum���� �����Ѽ� �̾��ִ°�
		ck = 0;   //Ȯ�κ��� �ʱ�ȭ
		p = head->llink; //ù ��Һ��� Ž��
		while (p != head) //����� Ž��, ���� ù��Ҷ�� �ݺ����� ����ȵ�
		{
			if (p->student.id == temp->student.id) //ID�� �ߺ��Ǿ��ٸ�
			{
				ck = 1;                                         //Ȯ�κ���=1
				break;
			}
			p = p->llink;
		}
		if (ck != 1)    //ID�� �ߺ������ʾҴٸ�
			break;   //������
	}

	printf("<2> ID �������� : %d\n\n", temp->student.id);
}

int inpaddress(list *temp)   //�ּ� �Է��ϴ� �Լ�
{
	if (!inpaddressone(temp))	//�� �ܰ躰 �ּ��Է�, �����Ҵ� ���н� 0��ȯ
		return 0;

	if (!inpaddresstwo(temp))
		return 0;

	if (!inpaddressthree(temp))
		return 0;

	if (!inpaddressfour(temp))
		return 0;

	if (!inpaddressfive(temp))
		return 0;

	return 1; //�Է� ������ 1��ȯ
}

int inpaddressone(list *temp)
{	
	int ck = 0;

	while (1)
	{
		system("cls");
		printf("<3-1> ù��° �ּ��Է�\n");
		printf("@@ �ѱ� �� ��� �Է°����մϴ�. @@\n\n");

		if (ck == 1)
		{
			printf("�Է� : %s\n", temp->student.adr.state);
			printf("�߸��� �Է��Դϴ�. �ٽ� �Է��� �ֽʽÿ�.\n\n");
			free(temp->student.adr.state);
			temp->student.adr.state = NULL;
			ck = 0;
		}

		printf("��, ��(Ư����, ������ ��) : ");

		if (!strinp(&(temp->student.adr.state)))		//�����Ҵ���н� 0��ȯ
			return 0;

		if (excname(&(temp->student.adr.state)) == 1)	//���� ������ 1��ȯ
			break;
		else											//���� ������ ck�� 1����
			ck = 1;
	}
	return 1;	//���� �Է½� 1��ȯ
}

int inpaddresstwo(list *temp)
{
	int ck = 0;

	while (1)
	{
		system("cls");
		printf("<3-2> �ι�° �ּ��Է�\n");
		printf("@@ �ѱ� �� ��� �Է°����մϴ�. @@\n\n");

		if (ck == 1)
		{
			printf("�Է� : %s\n", temp->student.adr.city);
			printf("�߸��� �Է��Դϴ�. �ٽ� �Է��� �ֽʽÿ�.\n\n");
			free(temp->student.adr.city);
			temp->student.adr.city = NULL;
			ck = 0;
		}

		printf("��, ��, �� : ");

		if (!strinp(&(temp->student.adr.city)))			//�����Ҵ���н� 0��ȯ
			return 0;

		if (excname(&(temp->student.adr.city)) == 1)	//���� ������ 1��ȯ
			break;
		else											//���� ������ ck�� 1����
			ck = 1;
	}
	return 1;	//���� �Է½� 1��ȯ
}

int inpaddressthree(list *temp)
{
	int ck = 0;

	while (1)
	{
		system("cls");
		printf("<3-3> ����° �ּ��Է�\n");
		printf("@@ �ѱ� �� ��� �Է°����մϴ�. @@\n");
		printf("@@ �ش������ ���ٸ� n�� �Է��Ͻʽÿ�. @@\n\n");

		if (ck == 1)
		{
			printf("�Է� : %s\n", temp->student.adr.village);
			printf("�߸��� �Է��Դϴ�. �ٽ� �Է��� �ֽʽÿ�.\n\n");
			free(temp->student.adr.village);
			temp->student.adr.village = NULL;
			ck = 0;
		}

		printf("��, ��, �� : ");

		if (!strinp(&(temp->student.adr.village)))		//�����Ҵ���н� 0��ȯ
			return 0;

		if (excname(&(temp->student.adr.village)) == 1)	//���� ������ 1��ȯ
			break;
		else											//���� ������ ck�� 1����
			ck = 1;
	}

	ckno(&(temp->student.adr.village));	//3��° �ּҰ� ������� Ȯ��
	return 1;	//���� �Է½� 1��ȯ
}

int inpaddressfour(list *temp)
{
	int ck = 0;

	while (1)
	{
		system("cls");
		printf("<3-4> �׹�° �ּ��Է�\n");
		printf("@@ �ѱ�, ����, ����, - �� �Է°����մϴ�. @@\n");
		printf("@@ ������� �Է����ֽʽÿ�. @@\n\n");

		if (ck == 1)
		{
			printf("�Է� : %s\n", temp->student.adr.street);
			printf("�߸��� �Է��Դϴ�. �ٽ� �Է��� �ֽʽÿ�.\n\n");
			free(temp->student.adr.street);
			temp->student.adr.street = NULL;
			ck = 0;
		}

		printf("���θ�, ���� : ");

		if (!strinp(&(temp->student.adr.street)))		//�����Ҵ���н� 0��ȯ
			return 0;

		if (excaddr(&(temp->student.adr.street)) == 1)	//���� ������ 1��ȯ
			break;
		else											//���� ������ ck�� 1����
			ck = 1;
	}
	return 1;	//���� �Է½� 1��ȯ
}

int inpaddressfive(list *temp)
{
	int len;
	int i;

	system("cls");
	printf("<3-5> ������ ���ּ� �Է�\n\n");

	printf("���ּ� : ");

	if (!strinp(&(temp->student.adr.home)))		//�����Ҵ���н� 0��ȯ
		return 0;

	len = strlen(temp->student.adr.home);

	for (i = 0; i < len; i++)					//����κп� _�� ��ųֱ�
	{
		if ((temp->student.adr.home)[i] == ' ' || (temp->student.adr.home)[i] == '\t')
			(temp->student.adr.home)[i] = '_';
	}

	return 1;	//���� �Է½� 1��ȯ
}

int inppnum(list *head, list *temp) //��ȭ��ȣ �Է¹޴� �Լ�
{
	list *p;
	int ck = 0;

	while (1)
	{
		system("cls");
		printf("<4> ��ȭ��ȣ�Է�\n");
		printf("@@ ��ȭ��ȣ�� xxx-xxxx-xxxx �������� �Է����ֽʽÿ�. @@\n");
		printf("@@ ��ȭ��ȣ���� ���ڿ� - �� �Է� �����մϴ�. @@\n");
		printf("@@ ��ȭ�Ⱑ ������ n �� �Է��� �ֽʽÿ�. @@\n\n");

		if (ck == 1)
		{
			printf("��ȭ��ȣ�� �ߺ��Ǿ����ϴ�. : %s\n", temp->student.pnum);
			printf("�ٽ� �Է��� �ֽʽÿ�.\n\n");
			free(temp->student.pnum);                      //�����ߴ��� ��ȯ
		}
		else if (ck == 2)
		{
			printf("��ȭ��ȣ�� ���ڿ� - �� �ٸ����� �Ǵ� ������ �����ϴ�.\n");
			printf("�ٽ� �Է��� �ֽʽÿ�\n\n");
		}

		printf("��ȭ��ȣ : ");
		if (!strinp(&(temp->student.pnum))) //��ȭ��ȣ �Է�, �����Ҵ� ���н� 0��ȯ
			return 0;

		ck = 0;           //Ȯ�κ��� �ʱ�ȭ

		if (excphone(&(temp->student.pnum)) != 1)
		{
			printf("�Է� : %s\n", temp->student.pnum);
			free(temp->student.pnum);
			temp->student.pnum = NULL;
			ck = 2;
		}

		p = head->llink;  //ù ��Һ��� Ž��
		while (p != head && (temp->student.pnum != NULL)) //���Ž��, ù����ϰ�� �ݺ��� ����ȵ�
		{
			if (strcmp(temp->student.pnum, p->student.pnum) == 0) //��ȭ��ȣ�� �ߺ���ٸ�
			{
				ck = 1;                                         //Ȯ�κ���=1
				break;
			}
			p = p->llink;
		}
		if (ck != 1 && ck != 2)    //��ȭ��ȣ�� �ߺ������ʾҰ� ���ܰ� ��������
			break;				   //������
	}

	ckno(&(temp->student.pnum));    //n �Է��ߴ��� Ȯ���ϱ�

	if (temp->student.pnum == NULL) //��ȭ�Ⱑ ���ٸ�
	{
		temp->student.pnum = (char *)malloc(strlen(NO_PHONE) + 1); //��ȭ�Ⱑ ���ٴ� ���� ���ڿ��� ����
		if (!ckalloc(temp->student.pnum))                        //�����Ҵ� ���н� 0��ȯ
			return 0;
		strcpy(temp->student.pnum, NO_PHONE);
		(temp->student.pnum)[strlen(NO_PHONE)] = '\0';
	}
	return 1; //������ 1��ȯ
}

void cmpinfolen(info inf, int *nlen, int * adlen) //�̸�, �ּ� �ִ���� ���ϴ��Լ�
{
	int temp;		       //���̱��ϴ� �Լ� �ι�ȣ������ʰ��ϱ�����(���ǹ������� ȣ���ϰ���)

	if ((temp = lenname(inf)) > *nlen)       //���ǰ˻� �ϸ鼭 ������ �� ����
		*nlen = temp;						 //�� ��� �� �� ����

	if ((temp = lenadrs(inf.adr)) > *adlen)  //���� ����
		*adlen = temp;
}

int lenname(info name)   //�̸�, �ּ� �ִ���� ���ϴ� �Լ����� �̸����� ������ ����ϴ� �Լ�
{
	return strlen(name.fname) + strlen(name.lname) + 1;  //fname + lname + 1(��, �̸����̿� ����) ����
}

int lenadrs(address adr)  //�̸�, �ּ� �ִ���� ���ϴ� �Լ����� �ּұ��� ������ ����ϴ� �Լ�
{
	int len = 0;     //�� �ּ� ����
	int eac = 0;	 //�ּ� ����(��, ��, ��, ���θ�, ���ּ�)

	len += strlen(adr.state);   //�� ���ڿ��� ���̸� len�� ������
	eac++;                      //���ڿ�����++

	len += strlen(adr.city);
	eac++;

	if (adr.village != NULL)
	{
		len += strlen(adr.village);
		eac++;
	}
	else
	{
		len += 1;			//NULL�ΰ�� ���ڸ��� #�� �����ϹǷ� ���ڼ� 1���� ��
		eac++;
	}

	len += strlen(adr.street);
	eac++;

	len += strlen(adr.home);

	len += eac;    //�ּ��� ���� + ������ ����

	return len;    //�ּ� �� ���� ��ȯ(�ּһ��� ���� ����)
}

int issueretouchmenu()	//�����ϴ� �Լ� �޴������ϴ� ��
{
	int sel;

	system("cls");
	printf("====<< student.db ���� ���� >>====\n");
	printf("  1. �л����� �߰�\n");
	printf("  2. �л����� ����\n");
	printf("  3. �л����� ����\n");
	printf("\n");
	printf("  4. �������� ���ư���\n");
	printf("===================================\n\n");
	
	while(1)
	{
		printf("  ���� : ");
		scanf("%d", &sel);
		cleanstdin();

		if(sel==1 || sel==2 || sel==3 || sel==4)
			break;
		else
			printf("�߸��� �����Դϴ�.\n\n");
	}
	return sel;	
}

int retouchfile()	//���ϳ��� �����Լ�(����, �߰�, ����)
{
	int sel;				//�������� ���ǰ˻縦 ���� ����
	char ck;				//�� �������� ���� Ȯ���ϴ� ����
	list head;				//������ list�� �����ð�
	int namelen, adrlen;	//�ٽ� list�� ������ �ֱ����� �̸�, �ּ� ���̱��ϱ�
	list *hd=&head;			//�Ʒ��� head�� ���������� ����ϴµ� &(&head)�̷������δ� �ȵǼ� �����ͺ��� ���

	head.flink=&head;		//head��� �ʱ�ȭ
	head.llink=&head;

	sel = filetolist(&head);	//file���� list�� �ޱ�, -1:���Ͽ��½��� 0:�����Ҵ����
	if (sel == -1)
		return MENU;
	else if (sel == 0)
	{
		printf("���� �о������ �����Ҵ翡 �����߽��ϴ�.\n");
		freelist(&head);
		return MENU;
	}

	while(1)
	{
		sel=issueretouchmenu();	//�޴����� �ϳ� �����ϱ�

		switch(sel)
		{
		case 1:
			sel = addinfo(&head);	//�л����� �߰�
			if(sel==0)
				printf("�л����� �߰��� �����Ͽ����ϴ�.\n\n");
			break;
		case 2:
			sel=retouchinfo_search(&head);	//�л����� ����
			break;
		case 3:
			sel=deleteinfo(&head);		//�л����� ����
			break;
		case 4:
			checklentolist(&hd, &namelen, &adrlen);	//��������(���θ޴���)���ư��� - ������� �������� ���Ͽ� �ű�
			listtofile(&head, namelen, adrlen);
			return PREV;
		}
		if (sel == MENU)		//�� �����޴����� ���θ޴��� ���ư��� ���ý� ���ư�
		{
			checklentolist(&hd, &namelen, &adrlen);
			listtofile(&head, namelen, adrlen);
			return MENU;
		}
		else if (sel == PREV)	//�� �����޴����� �����޴��� ���ư��� ���ý� �ƹ���� ����(�ٽ� ������ �ݺ���)
		{
		}
		else
		{
			ck = checkmoreretouch();	//�ٸ� �л������� �����Ұ��� ����
			if (ck == 'n')
				break;
		}
	}
	checklentolist(&hd, &namelen, &adrlen);
	listtofile(&head, namelen, adrlen);
	system("cls");
	return 1;	//������ ������ 1��ȯ
}

char checkmoreretouch()	//�ٸ� �л������� �����Ұ��ΰ�?
{
	char sel;

	while(1)
	{
		printf("�ٸ� �л� ������ �����Ͻðڽ��ϱ�? (y/n) : ");
		scanf("%c", &sel);
		cleanstdin();
		if (sel == 'y' || sel == 'n')
			break;
		else
			printf("�����Ϸ��� y, �޴��� ���ư����� n�� ������ �ֽʽÿ�.\n\n");
	}
	return sel;
}

int deleteinfo(list *head)	//���ϳ��� ����
{
	list *p;				//������ ���
	int sel;
	char *fname, *lname;	//Ž���� ���� �̸�
	int id;					//Ž���� ���� ID

	system("cls");
	sel=searchselect();		//�������� Ž������ ����

	switch(sel)
	{
	case 1:
		if (inpsearchname(&fname, &lname) != 1)	//Ž���� ���� �̸��Է�
		{
			printf("�̸� �˻��������� �����Ҵ翡 �����Ͽ����ϴ�.\n");
			return MENU;
		}
		p=findlistbyname(head, fname, lname);	//list���� ������ Ž������ ���ã��
		break;
	case 2:
		inpsearchid(&id);
		p=findlistbyid(head, id);	//list���� ������ Ž������ ���ã��
		break;
	case 3:
		return PREV;	//�����޴��� ���ư���
	case 4:
		return MENU;	//���θ޴��� ���ư���
	}
	if(p==NULL)
	{
		printf("�ش� �л������� �����ϴ�.\n\n");
	}
	else
	{
		deletelist(p);
		printf("���� ���� �Ϸ�\n\n");
	}
	return 1;
}

int addinfo(list *head)		//���ϳ��� �߰�
{
	list *temp;

	system("cls");
	temp = (list *)malloc(sizeof(list)); //���� �Ҵ�
	if (!ckalloc(temp))					 //�����Ҵ� Ȯ��, ���н� 0��ȯ
	{
		return 0;
	}

	cleaninfo(&(temp->student));		  //info�� ��� �����ʹ� NULL��ä�� ����

	if (inputkeyboard(head, &temp) == -1) //Ű����� �Է�, �����ϸ� 0��ȯ
	{
		return 0;
	}

	inputlist(head, temp); //List�� ���ο� ������
	system("cls");
	printf("���� �߰� �Ϸ�.\n");
	prtline(temp->student);	//�߰��� ���� ª�� �����ֱ�
	printf("\n");
	return 1;
}

int retouchinfo_search(list *head)	//���ϳ��� ����
{
	list *p;	//������ ���
	int sel;
	char *fname, *lname;	//Ž���� ���� �̸�
	int id;					//Ž���� ���� ID

	system("cls");
	while (1)
	{
		sel = searchselect();

		switch (sel)
		{
		case 1:
			if (inpsearchname(&fname, &lname) != 1)	//Ž���� ���� �̸� �Է�
			{
				printf("�̸� �˻��������� �����Ҵ翡 �����Ͽ����ϴ�.\n");
				return MENU;
			}
			p = findlistbyname(head, fname, lname);	//list���� ���������� ���ã��
			break;
		case 2:
			inpsearchid(&id);
			p = findlistbyid(head, id);
			break;
		case 3:
			return PREV;	//�����޴��� ���ư���
		case 4:
			return MENU;	//���θ޴��� ���ư���
		}
		if (p == NULL)
		{
			system("cls");
			printf("�ش� �л������� �����ϴ�.\n\n");
		}
		else
		{
			sel = retouchinfo_retouch(head, p);	//�л����� ������ �����ϱ� ����
			if (sel == 0)
			{
				printf("���� ������ �����߽��ϴ�.\n");
				printf("�ش� ������ ������� �ʽ��ϴ�.\n\n");
			}
			else if (sel == MENU)
				return MENU;
			else if (sel == PREV)
			{
			}
			else
			{
				system("cls");
				printf("�������� �Ϸ�\n");
				prtline(p->student);
				printf("\n");
				break;
			}
		}
	}
	return 1;
}

int retouchitemselect()		//� �׸� �����Ұ��� ����
{
	int sel;

	printf("=====< ������ �׸� >=====\n");
	printf("  1. �̸�\n");
	printf("  2. �ּ�\n");
	printf("  3. ��ȭ��ȣ\n");
	printf("\n");
	printf("  4. �������� ���ư���\n");
	printf("  5. �޴��� ���ư���\n");
	printf("=========================\n");

	while(1)
	{
		printf("  ���� : ");
		scanf("%d", &sel);
		cleanstdin();

		if(sel>0 && sel<6)
			break;
		else
			printf("�߸��� �����Դϴ�.\n\n");
	}
	return sel;	
}

int retouchinfo_retouch(list *head, list *p)
{
	int sel;

	while (1)
	{
		system("cls");
		printf("���� : ");
		prtline(p->student);	//���� �����Ϸ��� ���� ���� �����ֱ�
		printf("\n\n");
		sel = retouchitemselect();	//������ �׸� ����

		switch (sel)
		{
		case 1:		//�̸�
			if (!retouch_name(p))
			{
				return 0;
			}
			break;
		case 2:		//�ּ�
			sel = retouch_address(p);
			if (sel == 0)
				return 0;
			break;
		case 3:		//��ȭ��ȣ
			if (!retouch_phonenumber(head, p))
			{
				return 0;
			}
			break;
		case 4:
			system("cls");
			return PREV;
		case 5:
			return MENU;
		}
		if (sel == PREV)
		{
		}
		else if (sel == MENU)
			return MENU;
		else
			break;
	}
	return 1;
}

int retouch_name(list *p)
{
	list temp;

	temp.student.fname=NULL;
	temp.student.lname=NULL;
	system("cls");
	if (!inpfirstname(&temp))				 //FirstName�Է�, ���н� 0��ȯ
	{
		return 0;
	}
	if (!inplastname(&temp))				 //LastName�Է�, ���н� 0��ȯ
	{
		freename(&(temp.student));
		return 0;
	}

	freename(&(p->student));				//���� ���� ��ȯ�� �� ���� �ֱ�
	p->student.fname=temp.student.fname;
	p->student.lname=temp.student.lname;
	return 1;
}

int retouch_address_menu()	//� �ּ� �����Ұ��� �����ϱ�
{
	int sel;

	system("cls");
	printf("==< ������ �ּ� �׸� ���� >==\n");
	printf("  1. ù��° �ּ�(��, ��)\n");
	printf("  2. �ι��� �ּ�(��, ��, ��)\n");
	printf("  3. ����° �ּ�(��, ��, ��)\n");
	printf("  4. ���θ�/ ���� �ּ�\n");
	printf("  5. �� �ּ�\n");
	printf("\n");
	printf("  6. �������� ���ư���\n");
	printf("  7. �޴��� ���ư���\n");
	printf("=============================\n");
	
	while (1)
	{
		printf("  ���� : ");
		scanf("%d", &sel);
		cleanstdin();

		if (sel > 0 && sel < 8)
			return sel;
		else
			printf("�߸��� �����Դϴ�.\n\n");
	}
}

int retouch_address(list *p)
{
	int sel;
	list temp;

	cleaninfo(&(temp.student));	//������ ���� �ӽó���� ��� �����ʹ� NULL�� ����

	sel = retouch_address_menu();

	switch (sel)
	{
	case 1:
		if (!(retouch_address_one(&temp, &p)))
			return 0;
		break;
	case 2:
		if (!(retouch_address_two(&temp, &p)))
			return 0;
		break;
	case 3:
		if (!(retouch_address_three(&temp, &p)))
			return 0;
		break;
	case 4:
		if (!(retouch_address_four(&temp, &p)))
			return 0;
		break;
	case 5:
		if (!(retouch_address_five(&temp, &p)))
			return 0;
		break;
	case 6:
		return PREV;
	case 7:
		return MENU;
	}
}

int retouch_address_one(list *temp, list **p)
{
	if (!(inpaddressone(temp)))	//������ ���� �Է�(�����Ҵ� ���н� 0��ȯ)
		return 0;
	else
	{
		free((*p)->student.adr.state);
		(*p)->student.adr.state = temp->student.adr.state;
		return 1;
	}
}

int retouch_address_two(list *temp, list **p)
{
	if (!(inpaddresstwo(temp)))	//������ ���� �Է�(�����Ҵ� ���н� 0��ȯ)
		return 0;
	else
	{
		free((*p)->student.adr.city);
		(*p)->student.adr.city = temp->student.adr.city;
		return 1;
	}
}

int retouch_address_three(list *temp, list **p)
{
	if (!(inpaddressthree(temp)))	//������ ���� �Է�(�����Ҵ� ���н� 0��ȯ)
		return 0;
	else
	{
		if ((*p)->student.adr.village != NULL)	//�ش� �ּҰ� ������쿡�� ��ȯ
			free((*p)->student.adr.village);
		(*p)->student.adr.village = temp->student.adr.village;
		return 1;
	}
}

int retouch_address_four(list *temp, list **p)
{
	if (!(inpaddressfour(temp)))	//������ ���� �Է�(�����Ҵ� ���н� 0��ȯ)
		return 0;
	else
	{
		free((*p)->student.adr.street);
		(*p)->student.adr.street = temp->student.adr.street;
		return 1;
	}
}

int retouch_address_five(list *temp, list **p)
{
	if (!(inpaddressfive(temp)))	//������ ���� �Է�(�����Ҵ� ���н� 0��ȯ)
		return 0;
	else
	{
		free((*p)->student.adr.home);
		(*p)->student.adr.home = temp->student.adr.home;
		return 1;
	}
}

int retouch_phonenumber(list *head, list *p)
{
	list temp;

	temp.student.pnum=NULL;

	if (!inppnum(head, &temp))               //PhoneNumber�Է�, ���н� 0��ȯ
	{
		return 0;
	}

	freephonenumber(&(p->student));
	p->student.pnum=temp.student.pnum;
	return 1;
}

int issuedisplaymenu()	//��¹�� �����ϱ�
{
	int sel;

	system("cls");
	printf("====<< student.db ���� ��� >>====\n");
	printf("  1. �̸��� ������ ���\n");
	printf("  2. �й��� ������ ���\n");
	printf("  3. �Է��� ������� ���\n");
	printf("  4. �л����� �˻��Ͽ� ���\n");
	printf("\n");
	printf("  5. �������� ���ư���\n");
	printf("  6. �޴��� ���ư���\n");
	printf("==================================\n");
	printf("  ���� : ");
	scanf("%d", &sel);
	cleanstdin();
	return sel;
}

void displayselect(int cdt)
{
	int sel;
	FILE *fp;

	if (cdt == -1)	//�Էºο��� �����Ҵ���н� ���θ޴��� ���ư�
	{
		return;
	}
	if (cdt == IN)	//�Է��� ���ϳ��� ����Ҷ� �״�� �������
	{
		fp = fopen("student.db", "rt");
		if(fp==NULL)
		{
			printf("������ �������� �ʽ��ϴ�.\n\n");
			return;
		}
		displayinfo(fp);
		fclose(fp);
		return;
	}
	if (cdt == MENU)	//���θ޴��� ���ư��� ���ý� �ƹ��͵� ���ϰ� ����
	{
		system("cls");
		return;
	}

	while (1)
	{
		sel = issuedisplaymenu();
		switch (sel)
		{
		case 1:
			displaytosort(1);	//1�� �ָ� �̸�����
			break;
		case 2:
			displaytosort(2);	//2�� �ָ� ID����
			break;
		case 3:
			fp = fopen("student.db", "rt");
			if(fp==NULL)
			{
			printf("������ �������� �ʽ��ϴ�.\n\n");
			return;
			}
			displayinfo(fp);
			fclose(fp);
			break;
		case 4:
			sel = displaysearch();
			break;
		case 5:
			system("cls");
			sel = MENU;
			break;
		case 6:
			system("cls");
			return;
		default:
			printf("�߸��� �����Դϴ�.\n");
			break;
		}
		if (sel == PREV)
		{
		}
		else
			break;
	}
}

void displaytosort(int sel)
{
	FILE *fp;		   //������ ���Ͽ� �ֱ����� ����������
	info ary[ARY_LEN]; //�л����������� ����ü�迭
	int index;         //�迭�� �������Է���� ���
	int ck;
	int namelen, adrlen;	//���Ͽ� �ֱ����� �̸�, �ּ� ���̱��ϱ�

	ck = filetoarray(ary, &index); //������ ������ �迭�� �б�
	if (ck == 0)                   //���ϰ��� 0�̸� �����Ҵ����, ���θ޴��� ���ư�
	{
		printf("��ġ : FileToArray\n\n");
		return;
	}
	else if (ck == -1)             //���ϰ��� -1�̸� ���Ͽ��½���, ���θ޴��� ���ư�
	{
		return;
	}
	if(index==0)	//������ ����Ϸ��� �ô��� ������ ������
	{
		printf("�Էµ� ������ �����ϴ�.\n");
		return;
	}

	if(sel==1)	//1:�̸����� 2:ID����
		sortbyname(ary, 0, index-1);
	else
		sortbyid(ary, 0, index-1);

	checklentoarray(ary, index, &namelen, &adrlen);	//�̸�, �ּұ��� ���ϱ�
	arraytofile(ary, index, namelen, adrlen);		//���ĵ� ���� ���Ͽ� �ֱ�(sortedstudent.db)
	fp=fopen("sortedstudent.db", "rt");				//���� ���ĵȰ� ����ϱ����� ���Ͽ���
	if (fp == NULL)
	{
		printf("������ ���ų� ���µ� �����Ͽ����ϴ�.\n");
		return;
	}
	displayinfo(fp);		//������ ���� ���
	fclose(fp);
	freearray(ary, index);	//�� �� ���� ��ȯ�ϱ�
}

int displaysearch()
{
	list *nht[HASH_NAME_LEN];	//�̸�Ž���� ���� Hash Table �����
	list *iht[HASH_ID_LEN];		//IDŽ���� ���� Hash Table �����
	list *p;					//Ž���Ǽ� ����� ���(���Ḯ��Ʈ)
	char *fname, *lname;		//Ž������ �̸�
	int id, sel, hck;			//Ž������ ID, ���� ���ǹ������� sel, hck

	clearhashtable(nht, HASH_NAME_LEN);	//�� hash table �ʱ�ȭ
	clearhashtable(iht, HASH_ID_LEN);
	hck = filetohash(nht, 1);			//���� ���Ͽ��� �̸������� hash table��
	if (hck == -1)
		return MENU;
	else if (hck == 0)
	{
		printf("���� �о������ �����Ҵ翡 �����߽��ϴ�.\n");
		freehash(nht, HASH_NAME_LEN);
		return MENU;
	}
	hck = filetohash(iht, 2);			//���Ͽ��� ID���� hash table��
	if (hck == -1)
		return MENU;
	else if (hck == 0)
	{
		printf("���� �о������ �����Ҵ翡 �����߽��ϴ�.\n");
		freehash(iht, HASH_ID_LEN);
		return MENU;
	}

	while(1)
	{
		system("cls");
		sel=searchselect();	//Ž���� ��� ����

		switch(sel)
		{
		case 1:
			if (inpsearchname(&fname, &lname) != 1)
			{
				printf("�̸� �˻��������� �����Ҵ翡 �����Ͽ����ϴ�.\n");
				return MENU;
			}
			p = hashingbyname(nht, fname, lname);	//�̸����� ����ã��(���������� ��� ����Ǽ� ����)
			break;
		case 2:
			inpsearchid(&id);
			p = hashingbyid(iht, id);
			if (p != NULL)					//����ã����� flink�� NULL�� ����(���������)
				p->flink = NULL;
			break;
		case 3:
			freehash(nht, HASH_NAME_LEN);	//�����޴��� ���ư������� ����� �޸� ��ȯ���� ��
			freehash(iht, HASH_ID_LEN);
			return PREV;
		case 4:
			freehash(nht, HASH_NAME_LEN);
			freehash(iht, HASH_ID_LEN);
			system("cls");
			return MENU;
		}

		displaysearchedinfo(p);	//ã�� ���� �����ֱ�

		sel=checkmoresearch();	//�� ã�ƺ����� ����
		if (sel == 'n')
			break;		
	}
	freehash(nht, HASH_NAME_LEN);
	freehash(iht, HASH_ID_LEN);
	system("cls");
	return 1;
}

int searchselect()	//�˻���� ����
{
	int sel;

	printf("====<< �˻� ��� >>====\n");
	printf("  1. �̸����� �˻�\n");
	printf("  2. �й����� �˻�\n");
	printf("\n");
	printf("  3. �������� ���ư���\n");
	printf("  4. �޴��� ���ư���\n");
	printf("=======================\n\n");
	while(1)
	{
		printf("  ���� : ");
		scanf("%d", &sel);
		cleanstdin();
		if(sel>0 && sel<5)
			break;
		else
			printf("�߸��� �����Դϴ�.\n\n");
	}
	return sel;
}

void displaysearchedinfo(list *p)
{
	int sel;

	system("cls");
	printf("========< ������ �л��� ���� >====================================================\n\n");

	if (p == NULL)
		printf("\n  �ش� �л������� �����ϴ�.\n\n");
	else
	{
		sel = 1;				//sel : ��ȣ���(1, 2, 3...)
		while (p != NULL)
		{
			printf("  %d. ", sel);
			prtline(p->student);
			p = p->flink;		//flink : ���������ǰ�� flink�� ���� �̾���(�ܼ�����)
			sel++;
		}
	}
}

char checkmoresearch()
{
	char sel;

	while(1)
	{
		printf("�� �˻��Ͻðڽ��ϱ�? (y/n) : ");
		scanf("%c", &sel);
		cleanstdin();
		if (sel == 'y' || sel == 'n')
			break;
		else
			printf("�� �˻��Ϸ��� y, �޴��� ���ư����� n�� ������ �ֽʽÿ�.\n\n");
	}
	return sel;
}

void displayinfo(FILE *stdinf)    //���α׷��� ������ �о ����ϴ°��� ����ϴºκ�
{
	int i;
	
	system("cls");
	printf("==============================< �л� ���� ��� >=================================================\n\n");
	while (!feof(stdinf))      //������ ��系�� �ܼ�â�� ���
	{
		i = fgetc(stdinf);	   //���Ϸκ��� �����ϳ��� �޾� ����i�� ����
		if(i=='#' || i=='_')   //������ ���� _, ���ٴ¶��� # ��ſ� �������
			putchar(' ');
		else
			putchar(i);        //���� �ϳ� ���
	}
	printf("\n");
	printf("=================================================================================================\n\n");
}

void listtofile(list *head, int namelen, int adrlen)  //����Ʈ���� ���Ϸ� ������ ����ϱ�
{
	FILE *stdinf;						   //student.db�� ���� ���������ͺ���
	list *p = head->llink;			 	   //p�� ù��°���

	int num = 1;						   //Num�κ� ���Ͽ�����

	if(p == head)						   //������ ���ٸ� ������ ������
	{
		remove("student.db");
		return;
	}

	stdinf = fopen("student.db", "wt");    //����, �ؽ�Ʈ���� ����(������ ������ ����, ������ ������ ������ ����)
	if (stdinf == NULL)                    //���� ���� �����Ѵٸ�
	{
		printf("���� ���¿� �����Ͽ����ϴ�.\n");
		printf("���α׷��� �����մϴ�.\n");
		exit(1);						   //���α׷� ��������
	}

	headtofile(&stdinf, namelen, adrlen);  //������ �������κ�(Num  Name  ....) ����

	while (p != head)					   //�����
	{
		fprintf(stdinf, "%-3d  ", num);    //Num�κ� �����Է�, 3ĭ ��������(999�� ���� �Է°���)

		nametofile(&stdinf, &p, namelen);  //�̸� ���Ͽ�����
		idtofile(&stdinf, &p);			   //ID ���Ͽ�����
		addresstofile(&stdinf, &p, adrlen);//Address ���Ͽ� ����
		phonenumbertofile(&stdinf, &p);    //Phonenumber ���Ͽ� ����

		num++;							   //Num1����
		p = p->llink;					   //�����������
	}

	freelist(head);						   //�����Ҵ���� list ��ȯ�ϱ�
	fclose(stdinf);						   //���� �ݱ�
}

void headtofile(FILE **stdinf, int namelen, int adrlen) //���� ���κ� Num  Name .... ����
{
	int j;									 //�ݺ������

	fprintf(*stdinf, "Num  Name  ");         //�̸��� �ּҿ� �����Ǵ� ĭ�� ���� �ԷµǴ� ���������� ���������� ������
	for (j = 0; j < namelen - 4; j++)        //Name�� 4ĭ�����ϹǷ� �װǻ��� ����ֱ�
		fputc(' ', *stdinf);
	fprintf(*stdinf, "Student-ID  Address  ");
	for (j = 0; j < adrlen - 7; j++)         //Address�� 7ĭ�����ϹǷ� �װǻ��� ����ֱ�
		fputc(' ', *stdinf);
	fprintf(*stdinf, "PhoneNumber\n");
}

void nametofile(FILE **stdinf, list **p, int namelen) //���Ͽ� �̸�����
{
	int len = 0; //�̸����̱��ϱ�
	int j;		 //�ݺ������

	fprintf(*stdinf, "%s %s  ", (*p)->student.fname, (*p)->student.lname);            //�̸��Է� : ��(����)�̸�(����)(����)
	len = namelen - (strlen((*p)->student.fname) + strlen((*p)->student.lname) + 1);  //�̸����� ���鹮�� ����ĭ�� �˾Ƴ���(�̸� �ִ���� - �Է��� �̸�����)
	for (j = 0; j < len; j++)														  //�̸����� ���� �������ŭ �������
		fputc(' ', *stdinf);
}

void idtofile(FILE **stdinf, list **p) //���Ͽ� ID����
{
	fprintf(*stdinf, "%d  ", (*p)->student.id); //Student-ID �Է� : ID(����)(����)
}

void addresstofile(FILE **stdinf, list **p, int adrlen) //���Ͽ� Address����
{
	int j = 0;   //�ּұ��� ����, �ݺ��뺯��
	int len = 0; //�ּұ��� ���ϱ�


	fprintf(*stdinf, "%s ", (*p)->student.adr.state);  //���Ͽ� ����
	j += strlen((*p)->student.adr.state) + 1;          //�� ���ڿ��� ����+1(�ּһ����� ����)�� �����ּұ��̿� ���Ѵ�


	fprintf(*stdinf, "%s ", (*p)->student.adr.city);   //�� ���� �ݺ�
	j += strlen((*p)->student.adr.city) + 1;

	if ((*p)->student.adr.village != NULL)			   //3��° �ּҴ� NULL�� �ü��ִ�
	{
		fprintf(*stdinf, "%s ", (*p)->student.adr.village);
		j += strlen((*p)->student.adr.village) + 1;
	}
	else
	{
		fprintf(*stdinf, "%s ", "#");
		j += 2;
	}

	fprintf(*stdinf, "%s ", (*p)->student.adr.street);
	j += strlen((*p)->student.adr.street) + 1;

	fprintf(*stdinf, "%s ", (*p)->student.adr.home);
	j += strlen((*p)->student.adr.home) + 1;
	
	j--;						//j-- �ϸ� �����Է��� �ּұ���
	len = adrlen - j;			//�ּұ��� �ִ밪-�Է��ұ���=�������
	for (j = 0; j < len; j++)   //����ĭ��ŭ �����Է�
		fputc(' ', *stdinf);
}

void phonenumbertofile(FILE **stdinf, list **p)   //���Ͽ� PhoneNumber����
{
	fprintf(*stdinf, " %s\n", (*p)->student.pnum); //PhoneNumber �Է�
}

void arraytofile(info *ary, int index, int namelen, int adrlen)  //����Ʈ���� ���Ϸ� ������ ����ϱ�
{
	FILE *stdinf;						   //student.db�� ���� ���������ͺ���
	int i;

	int num = 1;						   //Num�κ� ���Ͽ�����

	stdinf = fopen("sortedstudent.db", "wt");//����, �ؽ�Ʈ���� ����(������ ������ ����, ������ ������ ������ ����)
	if (stdinf == NULL)                    //���� ���� �����Ѵٸ�
	{
		printf("���� ���¿� �����Ͽ����ϴ�.\n");
		printf("���α׷��� �����մϴ�.\n");
		exit(1);						   //���α׷� ��������
	}

	headtofile(&stdinf, namelen, adrlen);  //������ �������κ�(Num  Name  ....) ����

	for(i=0;i<index;i++)
	{
		fprintf(stdinf, "%-3d  ", num);    //Num�κ� �����Է�, 3ĭ ��������(999�� ���� �Է°���)

		arynametofile(&stdinf, &(ary[i]), namelen);  //�̸� ���Ͽ�����
		aryidtofile(&stdinf, &(ary[i]));			 //ID ���Ͽ�����
		aryaddresstofile(&stdinf, &(ary[i]), adrlen);//Address ���Ͽ� ����
		aryphonenumbertofile(&stdinf, &(ary[i]));    //Phonenumber ���Ͽ� ����

		num++;							   //Num1����
	}

	fclose(stdinf);						   //���� �ݱ�
}

void arynametofile(FILE **stdinf, info *p, int namelen) //���Ͽ� �̸�����
{
	int len = 0; //�̸����̱��ϱ�
	int j;		 //�ݺ������

	fprintf(*stdinf, "%s %s  ", p->fname, p->lname);            //�̸��Է� : ��(����)�̸�(����)(����)
	len = namelen - (strlen(p->fname) + strlen(p->lname) + 1);  //�̸����� ���鹮�� ����ĭ�� �˾Ƴ���(�̸� �ִ���� - �Է��� �̸�����)
	for (j = 0; j < len; j++)									//�̸����� ���� �������ŭ �������
		fputc(' ', *stdinf);
}

void aryidtofile(FILE **stdinf, info *p) //���Ͽ� ID����
{
	fprintf(*stdinf, "%d  ", p->id); //Student-ID �Է� : ID(����)(����)
}

void aryaddresstofile(FILE **stdinf, info *p, int adrlen) //���Ͽ� Address����
{
	int j = 0;   //�ּұ��� ����, �ݺ��뺯��
	int len = 0; //�ּұ��� ���ϱ�


	fprintf(*stdinf, "%s ", p->adr.state);  //���Ͽ� ����
	j += strlen(p->adr.state) + 1;          //�� ���ڿ��� ����+1(�ּһ����� ����)�� �����ּұ��̿� ���Ѵ�


	fprintf(*stdinf, "%s ", p->adr.city);   //�� ���� �ݺ�
	j += strlen(p->adr.city) + 1;

	if (p->adr.village != NULL)			   //3��° �ּҴ� NULL�� �ü��ִ�
	{
		fprintf(*stdinf, "%s ", p->adr.village);
		j += strlen(p->adr.village) + 1;
	}
	else
	{
		fprintf(*stdinf, "%s ", "#");
		j += 2;
	}

	fprintf(*stdinf, "%s ", p->adr.street);
	j += strlen(p->adr.street) + 1;

	fprintf(*stdinf, "%s ", p->adr.home);
	j += strlen(p->adr.home) + 1;
	
	j--;							//j-- �ϸ� �����Է��� �ּұ���
	len = adrlen - j;			//�ּұ��� �ִ밪-�Է��ұ���=�������
	for (j = 0; j < len; j++)   //����ĭ��ŭ �����Է�
		fputc(' ', *stdinf);
}

void aryphonenumbertofile(FILE **stdinf, info *p)   //���Ͽ� PhoneNumber����
{
	fprintf(*stdinf, " %s\n", p->pnum); //PhoneNumber �Է�
}

int filetolist(list *head)		//���Ͽ��� ����Ʈ�� ������ �Է��ϱ�
{
	FILE *rd;									 //���� ����������
	list *temp;									 //�ӽó��
	int i;										 //�ݺ������

	temp = (list *)malloc(sizeof(list));		 //ù ��带���� �����Ҵ�
	if (!ckalloc(temp))							 //�����Ҵ� ���н� 0��ȯ
		return 0;

	rd = fopen("student.db", "rt");				 //���� ����
	if (rd == NULL)								 //���� ������ ���µ� �����ߴٸ�
	{
		printf("������ ���ų� ���µ� �����Ͽ����ϴ�.\n");
		return -1;
	}

	while (fgetc(rd) != '\n');					 //ù ��(Num  Name.....)�� ����
	fgetc(rd);									 //������ ��ĭ ����(Num�� ������ 5ĭ�� ��ĭ)

	while(1)									 //���� �� ���������� ���ѹݺ�
	{
		cleaninfo(&(temp->student));

		for (i = 0; i < 4; i++)					//Num�� ������ ĭ�� ������ 4ĭ ����
			fgetc(rd);

		if (nametolist(&rd, &temp) == 0)		//�̸� �б�, ���н� ���ϴݰ� ��ȯ
		{
			fclose(rd);
			return 0;
		}
		idtolist(&rd, &temp);					//ID �б�
		if (addresstolist(&rd, &temp) == 0)		//�ּ� �б�, ���н� ���ϴݰ� ��ȯ
		{
			fclose(rd);
			return 0;
		}
		if (phonenumbertolist(&rd, &temp) == 0)	//��ȭ��ȣ �б�, ���н� ���ϴݰ� ��ȯ
		{
			fclose(rd);
			return 0;
		}

		inputlist(head, temp);							    //����Ʈ�� �ֱ�
		temp=NULL;											//�پ�temp�� NULL �־��ֱ�

		if (fgetc(rd) == EOF)                               //Num�� ������ĭ�� ù��°ĭ �����ͼ� ������ �������� �˻�(�׷��� �ݺ��ҋ� 4ĭ��������)
			break;

		temp = (list *)malloc(sizeof(list));			    //������ �����Ͱ� �������� ��� �߰�����
		if (!ckalloc(temp))									//�����Ҵ�˻�, ���н� ���ϴݰ� ��ȯ
		{
			fclose(rd);
			return 0;
		}
	}
	fclose(rd);   //���ϴݱ�
	return 1;     //������ 1��ȯ
}

int nametolist(FILE **rd, list **temp)  //list�� Name �ֱ�
{
	char buff[BUFF_LEN];										//���ڿ� �ӽ������ �迭

	fscanf(*rd, "%s", buff);                                    //�迭�� ���ڿ��б�
	(*temp)->student.fname = (char *)malloc(strlen(buff) + 1);  //ũ�⸸ŭ �����Ҵ�
	if (!ckalloc((*temp)->student.fname))						//�����Ҵ�˻� ���н� temp��ȯ�� 0��ȯ
	{
		free(*temp);
		return 0;
	}
	strcpy((*temp)->student.fname, buff);                       //���ڿ� ����

	fscanf(*rd, "%s", buff);									//�迭�� ���ڿ��б�
	(*temp)->student.lname = (char *)malloc(strlen(buff) + 1);	//�����Ҵ�
	if (!ckalloc((*temp)->student.lname))						//�����Ҵ�˻� ���н� name��ȯ�� temp��ȯ �׸��� 0��ȯ
	{
		freename(&((*temp)->student));
		free(*temp);
		return 0;
	}
	strcpy((*temp)->student.lname, buff);						//���ڿ� ����
	return 1;													//������ 1��ȯ
}

void idtolist(FILE **rd, list **temp)	//list�� ID �ֱ�
{
	char buff[BUFF_LEN];				  //���ڿ� �ӽ������ �迭

	fscanf(*rd, "%s", buff);
	(*temp)->student.id = atoi(buff);     //���ڿ��� ������ ��ȯ�� ����
	fgetc(*rd); fgetc(*rd);               //ID�� �ּһ��� 2ĭ ����
}

int addresstolist(FILE **rd, list **temp) //list�� Address �ֱ�
{
	char buff[BUFF_LEN];						   //���ڿ� �ӽ������ �迭
 
	fscanf(*rd, "%s", buff);										//�迭�� ���ڿ��б�
	(*temp)->student.adr.state = (char *)malloc(strlen(buff) + 1);  //�����Ҵ�
	if (!ckalloc((*temp)->student.adr.state)	)					//�����Ҵ�Ȯ��, ���н� �迭�� �����Ҵ������� ��ȯ �׸��� 0��ȯ
	{
		freename(&((*temp)->student));
		freeaddress(&((*temp)->student));
		free(*temp);
		return 0;
	}
	strcpy((*temp)->student.adr.state, buff);                       //���ڿ� ����, ���� �Ʒ��� ������ ���� �ݺ�

	fscanf(*rd, "%s", buff);										//�迭�� ���ڿ��б�
	(*temp)->student.adr.city = (char *)malloc(strlen(buff) + 1);   //�����Ҵ�
	if (!ckalloc((*temp)->student.adr.city))						//�����Ҵ�Ȯ��, ���н� �迭�� �����Ҵ������� ��ȯ �׸��� 0��ȯ
	{
		freename(&((*temp)->student));
		freeaddress(&((*temp)->student));
		free(*temp);
		return 0;
	}
	strcpy((*temp)->student.adr.city, buff);                        //���ڿ� ����, ���� �Ʒ��� ������ ���� �ݺ�

	fscanf(*rd, "%s", buff);										//�迭�� ���ڿ��б�
	(*temp)->student.adr.village = (char *)malloc(strlen(buff) + 1);//�����Ҵ�
	if (!ckalloc((*temp)->student.adr.village))						//�����Ҵ�Ȯ��, ���н� �迭�� �����Ҵ������� ��ȯ �׸��� 0��ȯ
	{
		freename(&((*temp)->student));
		freeaddress(&((*temp)->student));
		free(*temp);
		return 0;
	}
	if(strcmp(buff, "#")==0)				//���� #�� ���ִٸ� ���°��̶�¶�
	{
		free((*temp)->student.adr.village);
		(*temp)->student.adr.village=NULL;
	}
	else
		strcpy((*temp)->student.adr.village, buff);                 //���ڿ� ����, ���� �Ʒ��� ������ ���� �ݺ�

	fscanf(*rd, "%s", buff);										//�迭�� ���ڿ��б�
	(*temp)->student.adr.street = (char *)malloc(strlen(buff) + 1); //�����Ҵ�
	if (!ckalloc((*temp)->student.adr.street))						//�����Ҵ�Ȯ��, ���н� �迭�� �����Ҵ������� ��ȯ �׸��� 0��ȯ
	{
		freename(&((*temp)->student));
		freeaddress(&((*temp)->student));
		free(*temp);
		return 0;
	}
	strcpy((*temp)->student.adr.street, buff);						//���ڿ� ����, ���� �Ʒ��� ������ ���� �ݺ�

	fscanf(*rd, "%s", buff);										//�迭�� ���ڿ��б�
	(*temp)->student.adr.home = (char *)malloc(strlen(buff) + 1);	//�����Ҵ�
	if (!ckalloc((*temp)->student.adr.home))						//�����Ҵ�Ȯ��, ���н� �迭�� �����Ҵ������� ��ȯ �׸��� 0��ȯ
	{
		freename(&((*temp)->student));
		freeaddress(&((*temp)->student));
		free(*temp);
		return 0;
	}
	strcpy((*temp)->student.adr.home, buff);                       //���ڿ� ����, ���� �Ʒ��� ������ ���� �ݺ�
	
	return 1; //������ 1��ȯ
}

int phonenumbertolist(FILE **rd, list **temp) //list�� PhoneNunber �ֱ�
{
	char buff[BUFF_LEN];				      //���ڿ� �ӽ������ �迭

	fscanf(*rd, "%s", buff);
	(*temp)->student.pnum = (char *)malloc(strlen(buff) + 1);	//�����Ҵ�
	if (!ckalloc((*temp)->student.pnum))						//�����Ҵ�˻�, ���н� name�� address��ȯ �� temp��ȯ �׸��� 0��ȯ
	{
		freename(&((*temp)->student));
		freeaddress(&((*temp)->student));
		free(*temp);
		return 0;
	}
	strcpy((*temp)->student.pnum, buff);	//���ڿ� ����
	fgetc(*rd);								//���๮�� ����
	return 1; //������ 1��ȯ
}

int filetoarray(info *ary, int *index)
{
	FILE *rd;									//���� ����������
	int i;										//�ݺ������
	*index = 0;								    //�ε��� �⺻�� 0�����ʱ�ȭ

	rd = fopen("student.db", "rt");				//���� ����
	if (rd == NULL)								//���� ������ ���µ� �����ߴٸ�
	{
		printf("������ ���ų� ���µ� �����Ͽ����ϴ�.\n");
		return -1;
	}

	while (fgetc(rd) != '\n');					 //ù ��(Num  Name.....)�� ����
	fgetc(rd);									 //������ ��ĭ ����(Num�� ������ 5ĭ�� ��ĭ)

	while (1)									 //���� �� ���������� ���ѹݺ�
	{
		cleaninfo(&(ary[*index]));

		for (i = 0; i < 4; i++)					 //Num�� ������ ĭ�� ������ 4ĭ ����
			fgetc(rd);

		if (nametoarray(&rd, &ary, *index) == 0) //�̸� �б�, ���н� ���ϴݰ� ��ȯ
		{
			fclose(rd);
			return 0;
		}
		idtoarray(&rd, &ary, *index);			 //ID �б�
		if (addresstoarray(&rd, &ary, *index) == 0) //�ּ� �б�, ���н� ���ϴݰ� ��ȯ
		{
			fclose(rd);
			return 0;
		}
		if (pnumbertoarray(&rd, &ary, *index) == 0) //��ȭ��ȣ �б�, ���н� ���ϴݰ� ��ȯ
		{
			fclose(rd);
			return 0;
		}

		(*index)++;

		if (fgetc(rd) == EOF)                       //Num�� ������ĭ�� ù��°ĭ �����ͼ� ������ �������� �˻�(�׷��� �ݺ��ҋ� 4ĭ��������)
			break;

		if (*index == ARY_LEN)  //�迭�� ��á�µ� ������ ������ �ʾ�����
		{
			printf("�迭�� ũ�Ⱑ �����մϴ�.\n");
			printf("���α׷��� ������ �ʿ��մϴ�\n");
			printf("200���� ������ �ԷµǾ����ϴ�.\n");
			break;
		}
	}

	fclose(rd);   //���ϴݱ�
	return 1;     //������ 1��ȯ
}

int nametoarray(FILE **rd, info **ary, int index) //�迭�� Name �ֱ�
{
	char buff[BUFF_LEN];									 //���ڿ� �ӽ������ �迭

	fscanf(*rd, "%s", buff);                                 //�迭�� ���ڿ��б�
	(*ary)[index].fname = (char *)malloc(strlen(buff) + 1);  //�����Ҵ�
	if (!ckalloc((*ary)[index].fname))						 //�����Ҵ�Ȯ��, ���н� �迭�� �����Ҵ������� ��ȯ �׸��� 0��ȯ
	{
		freearray(*ary, index + 1);
		return 0;
	}
	strcpy((*ary)[index].fname, buff);                       //���ڿ� ����, ���� �Ʒ��� ������ ���� �ݺ�

	fscanf(*rd, "%s", buff);
	(*ary)[index].lname = (char *)malloc(strlen(buff) + 1);
	if (!ckalloc((*ary)[index].lname))
	{
		freearray(*ary, index + 1);
		return 0;
	}
	strcpy((*ary)[index].lname, buff);
	return 1; //������ 1��ȯ
}

void idtoarray(FILE **rd, info **ary, int index) //�迭�� ID �ֱ�
{
	char buff[BUFF_LEN];				  //���ڿ� �ӽ������ �迭

	fscanf(*rd, "%s", buff);
	(*ary)[index].id = atoi(buff);        //���ڿ��� ������ ��ȯ�� ����
	fgetc(*rd); fgetc(*rd);               //ID�� �ּһ��� 2ĭ ����
}

int addresstoarray(FILE **rd, info **ary, int index) //�迭�� Address �ֱ�
{
	char buff[BUFF_LEN];						   //���ڿ� �ӽ������ �迭
	int i = 0;									   //�迭 index�� ����
 
	fscanf(*rd, "%s", buff);									 //�迭�� ���ڿ��б�
	(*ary)[index].adr.state = (char *)malloc(strlen(buff) + 1);  //�����Ҵ�
	if (!ckalloc((*ary)[index].adr.state))						 //�����Ҵ�Ȯ��, ���н� �迭�� �����Ҵ������� ��ȯ �׸��� 0��ȯ
	{
		freearray(*ary, index + 1);
		return 0;
	}
	strcpy((*ary)[index].adr.state, buff);                      //���ڿ� ����, ���� �Ʒ��� ������ ���� �ݺ�

	fscanf(*rd, "%s", buff);									//�迭�� ���ڿ��б�
	(*ary)[index].adr.city = (char *)malloc(strlen(buff) + 1);  //�����Ҵ�
	if (!ckalloc((*ary)[index].adr.city))						//�����Ҵ�Ȯ��, ���н� �迭�� �����Ҵ������� ��ȯ �׸��� 0��ȯ
	{
		freearray(*ary, index + 1);
		return 0;
	}
	strcpy((*ary)[index].adr.city, buff);                        //���ڿ� ����, ���� �Ʒ��� ������ ���� �ݺ�

	fscanf(*rd, "%s", buff);									 //�迭�� ���ڿ��б�
	(*ary)[index].adr.village = (char *)malloc(strlen(buff) + 1);//�����Ҵ�
	if (!ckalloc((*ary)[index].adr.village))					 //�����Ҵ�Ȯ��, ���н� �迭�� �����Ҵ������� ��ȯ �׸��� 0��ȯ
	{
		freearray(*ary, index + 1);
		return 0;
	}
	if(strcmp(buff, "#")==0)				//���� #�� ���ִٸ� ���°��̶�¶�
	{
		free((*ary)[index].adr.village);
		(*ary)[index].adr.village=NULL;
	}
	else
		strcpy((*ary)[index].adr.village, buff);                //���ڿ� ����, ���� �Ʒ��� ������ ���� �ݺ�

	fscanf(*rd, "%s", buff);									//�迭�� ���ڿ��б�
	(*ary)[index].adr.street = (char *)malloc(strlen(buff) + 1);//�����Ҵ�
	if (!ckalloc((*ary)[index].adr.street))						//�����Ҵ�Ȯ��, ���н� �迭�� �����Ҵ������� ��ȯ �׸��� 0��ȯ
	{
		freearray(*ary, index + 1);
		return 0;
	}
	strcpy((*ary)[index].adr.street, buff);                     //���ڿ� ����, ���� �Ʒ��� ������ ���� �ݺ�

	fscanf(*rd, "%s", buff);									//�迭�� ���ڿ��б�
	(*ary)[index].adr.home = (char *)malloc(strlen(buff) + 1);  //�����Ҵ�
	if (!ckalloc((*ary)[index].adr.home))						//�����Ҵ�Ȯ��, ���н� �迭�� �����Ҵ������� ��ȯ �׸��� 0��ȯ
	{
		freearray(*ary, index + 1);
		return 0;
	}
	strcpy((*ary)[index].adr.home, buff);                       //���ڿ� ����, ���� �Ʒ��� ������ ���� �ݺ�
	
	return 1; //������ 1��ȯ
}

int pnumbertoarray(FILE **rd, info **ary, int index) //�迭�� PhoneNunber �ֱ�
{
	char buff[BUFF_LEN];				//���ڿ� �ӽ������ �迭

	fscanf(*rd, "%s", buff);            //Phone_Number �����ϱ�
	(*ary)[index].pnum = (char *)malloc(strlen(buff) + 1); //�����Ҵ�
	if (!ckalloc((*ary)[index].pnum))	//�����Ҵ�Ȯ��, ���н� �迭�� �����Ҵ������� ��ȯ �׸��� 0��ȯ
	{
		freearray(*ary, index + 1);
		return 0;
	}
	strcpy((*ary)[index].pnum, buff);	//���ڿ� ����
	fgetc(*rd);                         //���๮�� ����
	return 1; //������ 1��ȯ
}

void checklentoarray(info *ary, int index, int *namelen, int *adrlen)
{
	int i, n, a;

	*namelen=0;
	*adrlen=0;

	for(i=0;i<index;i++)	//��� �迭��� �˻��ϸ� �ִ���� ���ϱ�
	{
		n=lenname(ary[i]);
		a=lenadrs(ary[i].adr);

		if(*namelen<n)
			*namelen=n;

		if(*adrlen<a)
			*adrlen=a;
	}
}

void checklentolist(list **head, int *namelen, int *adrlen)
{
	list *p=(*head)->llink;
	int n, a;

	*namelen=0;
	*adrlen=0;

	while(p!=*head)		//��� list�� ��� �˻��ؼ� �ִ���� ���ϱ�
	{
		n=lenname(p->student);
		a=lenadrs(p->student.adr);

		if(*namelen<n)
			*namelen=n;

		if(*adrlen<a)
			*adrlen=a;

		p=p->llink;
	}
}

void freelist(list *head)   //�����Ҵ���� ����Ʈ ��� ��ȯ����
{
	list *q = head->llink;  //q�� ù��°���
	list *p;			    //free�� ���� list������

	while (q != head)      //������������ ��� ����
	{
		p = q;             //p�� q�� ������� ����Ű����
		q = q->llink;      //q�� �������� �̵���

		freename(&(p->student));       //�̸�, �ּ�, ��ȭ��ȣ ��ȯ �� ����ȯ
		freeaddress(&(p->student));
		freephonenumber(&(p->student));
		free(p);
	}
}

void freearray(info *inf, int index) //�迭 �� ��� ��� ��ȯ
{
	int i;

	for (i = 0; i<index; i++)
	{
		freename(&(inf[i]));
		freeaddress(&(inf[i]));
		freephonenumber(&(inf[i]));
	}
}

void freename(info *p)  //name��ȯ(�ش����� ������������ if�� �����)
{
	if(p->fname)
		free(p->fname);
	if(p->lname)
		free(p->lname);
}

void freeaddress(info *p)  //�ּ� ��ȯ(�ش����� ������������ if�� �����)
{
	if(p->adr.state)
		free(p->adr.state);
	if(p->adr.city)
		free(p->adr.city);
	if(p->adr.village)
		free(p->adr.village);
	if(p->adr.street)
		free(p->adr.street);
	if(p->adr.home)
		free(p->adr.home);
}

void freephonenumber(info *p)  //��ȭ��ȣ ��ȯ(�ش����� ������������ if�� �����)
{
	if(p->pnum)
		free(p->pnum);
}

void inputlist(list *head, list *newnode) //���ο��带 list�� �߰�
{
	newnode->flink = head->flink;         //newnode�� �� ��ũ���� ����Ȱ��� �����Ƿ� ��������
	newnode->llink = head;
	head->flink->llink = newnode;
	head->flink = newnode;				  //������ head->flink�� llink���� ����ؾ��ϹǷ� �� �������� ����
}

list* findlistbyname(list *head, char *fname, char *lname)	//list���� �̸�ã��(����Ž��)
{
	list *temp[ARY_LEN];	//���������� ���� �����͹迭(list�� flink�� llink�� ���� ������ �ȵȴ�)
	list *p;
	int index=0;
	int sel;

	p=head->llink;			//p�� ù��° ���

	while(p!=head)			//��� ��� Ž��
	{
		if(strcmp(p->student.fname, fname)==0)
		{
			if(strcmp(p->student.lname, lname)==0)
			{
				temp[index]=p;	//���������� ã���� ����Ʈ �迭�� �־��ֱ�
				index++;
			}
		}
		p=p->llink;
	}

	if(index==0)		//������ ���ٸ� NULL ��ȯ
		return NULL;
	else if(index==1)	//������ �ϳ����̶�� �װ� ��ȯ
		return temp[0];
	else
	{
		sel=chooseone(temp, index);	//������ ��������� �����ϳ� ��� ��ȯ
		return temp[sel];
	}
}

int chooseone(list **temp, int len)	//���������� �Ѹ� ����
{
	int i;
	int sel;

	printf("���������� �ֽ��ϴ�.\n");
	printf("���� �Ѹ��� ������ �ֽʽÿ�(���� �Է�).\n\n");
	printf("-------< �� �� >-------\n");
	for(i=0; i<len; i++)
	{
		printf(" %d. ", i+1);
		prtline((temp[i])->student);
	}
	printf("-----------------------\n\n");
	while (1)
	{
		printf("  ���� : ");
		scanf("%d", &sel);
		cleanstdin();
		if (sel >= 1 && sel <= len)
			return sel-1;				//sel-1:�ش� �л��� index
		else
		{
			printf("�߸��� �����Դϴ�.\n");
			printf("�ٽ� �Է��� �ֽʽÿ�.\n");
		}
	}
}

list* findlistbyid(list *head, int id)
{
	list *p;

	p=head->llink;	//p�� ù��°���

	while(p!=head)	//����� Ž��
	{
		if(p->student.id==id)
			break;

		p=p->llink;
	}
	if(p==head)		//������ ������ NULL��ȯ
		return NULL;
	else
		return p;
}

void deletelist(list *p)	//list���� �����ϳ� �����
{
	(p->llink)->flink=p->flink;
	(p->flink)->llink=p->llink;

	free(p);
}

int filetohash(list **hash, int sel)
{
	FILE *rd;									 //���� ����������
	list *temp;									 //�ӽó��
	int i;										 //�ݺ������

	temp = (list *)malloc(sizeof(list));		 //ù ��带���� �����Ҵ�
	if (!ckalloc(temp))							 //�����Ҵ� ���н� 0��ȯ
		return 0;

	rd = fopen("student.db", "rt");				 //���� ����
	if (rd == NULL)								 //���� ������ ���µ� �����ߴٸ�
	{
		printf("������ ���ų� ���µ� �����Ͽ����ϴ�.\n");
		return -1;
	}

	while (fgetc(rd) != '\n');					 //ù ��(Num  Name.....)�� ����
	fgetc(rd);									 //������ ��ĭ ����(Num�� ������ 5ĭ�� ��ĭ)

	while (1)									 //���� �� ���������� ���ѹݺ�
	{
		cleaninfo(&(temp->student));

		for (i = 0; i < 4; i++)					//Num�� ������ ĭ�� ������ 4ĭ ����
			fgetc(rd);

		if (nametolist(&rd, &temp) == 0)		//�̸� �б�, ���н� ���ϴݰ� ��ȯ
		{
			fclose(rd);
			return 0;
		}
		idtolist(&rd, &temp);					//ID �б�
		if (addresstolist(&rd, &temp) == 0)		//�ּ� �б�, ���н� ���ϴݰ� ��ȯ
		{
			fclose(rd);
			return 0;
		}
		if (phonenumbertolist(&rd, &temp) == 0)	//��ȭ��ȣ �б�, ���н� ���ϴݰ� ��ȯ
		{
			fclose(rd);
			return 0;
		}

		inputhash(hash, temp, sel);				//Temp Hash�� �ֱ�
		temp = NULL;							//�پ�temp�� NULL �־��ֱ�

		if (fgetc(rd) == EOF)                   //Num�� ������ĭ�� ù��°ĭ �����ͼ� ������ �������� �˻�(�׷��� �ݺ��ҋ� 4ĭ��������)
			break;

		temp = (list *)malloc(sizeof(list));	//������ �����Ͱ� �������� ��� �߰�����
		if (!ckalloc(temp))						//�����Ҵ�˻�, ���н� ���ϴݰ� ��ȯ
		{
			fclose(rd);
			return 0;
		}
	}
	fclose(rd);   //���ϴݱ�
	return 1;     //������ 1��ȯ
}

void clearhashtable(list **hash, int len)
{
	int i;

	for (i = 0; i < len; i++)
	{
		hash[i] = NULL;	//��� NULL�� �ʱ�ȭ�س���
	}
}

int hashfunctionbyname(char *name)
{
	return (name[0] & 0xFF) % 100;	//�ѱ��� ù��°����Ʈ:FFFFFFB0~FFFFFFC8(&0xFF�� �ϸ� 176~200)
}

int hashfunctionbyid(int id)
{
	return id%HASH_ID_LEN;	//ID�� �迭���̸�ŭ ���� ������
}

void inputhash(list **hash, list *data, int sel)
{
	int index;

	if (sel == 1)	//1:�̸�, 2:ID
		index = hashfunctionbyname(data->student.fname);
	else if (sel == 2)
		index = hashfunctionbyid(data->student.id);

	data->llink = hash[index];	//�ߺ��Ǵ°�� ��������(���߿� ���°� �Ǿտ� ����)
	hash[index] = data;
}

list* hashingbyname(list **hash, char *fname, char *lname)
{
	int index;
	list *head;	//�������� �������� ������
	list *q;	//������ �����ִ� ������
	list *p;	//hash[index]�� ���� ���ϸ� �ȵǹǷ� �̰ɾ�
	list *rt;	//head�� ������ �����ؼ� ��ȯ�Ҷ� ���� ������
	rt = NULL;
	head = NULL;

	index = hashfunctionbyname(fname);	//���� �ε��� ���ϱ�
	p = hash[index];					//hash[index]�� p���ϱ�

	while (p != NULL)	//�ߺ��� ������ ������� ����������Ƿ� ���߿��� Ž��
	{
		if (strcmp(p->student.fname, fname) == 0)
		{
			if (strcmp(p->student.lname, lname) == 0)	//�̸��� ��� ���������� �����Ƿ� ����
			{
				q = p;
				p = p->llink;
				q->flink = head;
				head = q;
			}
			else
				p = p->llink;
		}
		else
			p = p->llink;
	}
	if(head!=NULL)
	{
		if(head->flink==NULL)	//������ �ִµ� �Ѹ��ϰ�� �״�� ��ȯ
			return head;
		else
		{
			minisort(head, &rt);	//������ ������ �й��� ������ ��ȯ
			return rt;
		}
	}
	return head;	//�� �ܿ��� �׳� head��ȯ(NULL�ϰ��� NULL��ȯ��)
}

void minisort(list *head, list **rt)	//list ��������
{
	list *big, *b, *p;
	while(head!=NULL)
	{
		big=head;	//���� ū�� ����
		b=big;		//b�� ū�� ���õȰ��� ������
		p=head->flink;	//������ ���� �������� �� ����
		while(p!=NULL)
		{
			if(big->student.id<p->student.id)	//big���� ū ������ ��Ÿ�����
			{
				b=big;		//b�� ���ο� ū���� �ٷξղ� ����Ŵ
				big=p;		//���ο�� big�̵�
			}
			p=p->flink;
		}
		if(big!=b)			//big�� b�� �ٸ���� big�� �� ���� big�� ������带 ����Ŵ
		{
			b->flink=big->flink;
		}
		else
		{
			head=big->flink;	//ó�� �����Ѱ� ����ū��� head�� �� ������ ����Ŵ
		}
		big->flink=(*rt);		//����ū�� head���� ����� rt�� �̾���
		(*rt)=big;
	}
}

list* hashingbyid(list **hash, int id)
{
	int index;
	list *p;	//hash[index]�� ���� ���ϸ� �ȵ�

	index = hashfunctionbyid(id);

	p = hash[index];

	while (p != NULL)
	{
		if (p->student.id == id)
			break;
		else
			p = p->llink;
	}

	return p;
}

void freehash(list **hash, int len)	//hash table �����Ҵ� ��ȯ�ϱ�
{
	int i;
	list *p;

	for (i = 0; i < len; i++)
	{
		while (hash[i] != NULL)
		{
			p = hash[i];
			hash[i] = hash[i]->llink;
			freename(&(p->student));       //�̸�, �ּ�, ��ȭ��ȣ ��ȯ �� ����ȯ
			freeaddress(&(p->student));
			freephonenumber(&(p->student));
			free(p);
		}
		hash[i] = NULL;
	}
}