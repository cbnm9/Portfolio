/*
컴퓨터공학과 2013150016 서민성

1차 : 학생정보(이름, ID, 주소, 핸드폰번호)를 입력받아 student.db파일에 저장하고,
student.db파일에 저장된 정보를 모니터에 출력해주는 프로그램.

2차 : 파일을 리스트와 구조체배열로 입력하고, 리스트와 배열을 사용해 내용 출력

3차 : 1. 메뉴 만들고, 되돌아가는 기능 추가
	  2. 학생정보 수정하는 기능 추가
	  3. 학생정보 정렬, 탐색후 출력하는 기능 추가

*/
#include<stdio.h>    //printf, scanf등 기본적인 기능을 위해 포함
#include<string.h>   //문자열 처리를 위해 포함
#include<stdlib.h>   //메모리 동적할당을 위해 포함
#include<time.h>     //난수발생 seed값을 주기위해 포함

#define BUFF_LEN 150       //정보입력할때 임시로쓸 배열최대길이
#define ARY_LEN 200		   //입력할 학생정보의 최대값(배열로 받을수있는 최대갯수)
#define HASH_ID_LEN 1000   //Hash Table 크기(ID 탐색용)
#define HASH_NAME_LEN 100  //Hash Table 크기(Name 탐색용)
#define NO_PHONE "No_Phone"//핸드폰이 없을떄 입력되는데이터
#define MAXNUM 2017999999  //입력받을 ID의 최대값
#define MINNUM 2010000000  //입력받을 ID의 최소값
#define RAND_ID (double)rand()/RAND_MAX*(MAXNUM-MINNUM)+MINNUM
/*
  rand()%max 로 한다면 max가 큰수일때 뒷부분이 앞부분보다 적은확률로 나오게됨
  rand에 범위지정할떄 랜덤값을 0~1사이의 실수로 만들고
  거기에 지정하고싶은 최댓값을 곱하면
  0~max 범위의 수가 동일한확률로 나옴
  (double)rand()/RAND_MAX*(maxnum-minnum) = 0~(maxnum-minnum) 의 난수출력
  위에 +minnum을 하면 minnum~maxnum 의 난수출력
*/

enum{ IN=10, TOUCH, OUT, EXIT, PREV, MENU }; //기능선택->{새로만들기, 수정하기, 출력하기, 프로그램종료, 이전으로 돌아가기, 메뉴로 돌아가기 }

//////////////////////////////////    구조체 정의    ///////////////////////////////////////////
                                                                                            ////
typedef struct {     //주소저장을 위한 구조체												////
	char *state;     //도																	////
	char *city;      //시,군,구																////
	char *village;   //읍,면,구('시'아래 '구'가 올수도있다)									////
	char *street;    //도로명																////
	char *home;      //상세주소																////
}address;																					////
																							////
typedef struct {     //학생정보 저장을 위한 구조체											////
	char *fname;     //firstname															////
	char *lname;     //lastname																////
	int id;          //student-ID															////
	address adr;     //address																////
	char *pnum;      //Phone-Number															////
}info;																						////
																							////
typedef struct list{       //학생정보저장을위한 리스트										////
	info student;          //학생정보														////
	struct list *flink;	   //자신보다 먼저 들어온 노드를 가리킴								////
	struct list *llink;	   //자신보다 나중에 들어온 노드를 가리킴							////
}list;																						////
																							////
////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////    함수선언    ////////////////////////////////////////////
                                                                                            ////
void cleanstdin();						   //표준입력버퍼 지워주는 함수                     ////
int ckalloc(void *);					   //동적할당 확인하는 함수                         ////
void ckno(char **);						   //문자열입력시 NO를 입력했는지 확인하는 함수     ////
void cleaninfo(info *);					   //Info의 각 멤버 NULL로 초기화					////
void prtline(info);						   //입력한정보 확인차 출력							////
int excname(char **);					   //공백, 숫자, 특수문자 예외처리					////
int excaddr(char **);					   //공백, 특수문자 예외처리						////
int excphone(char **);					   //공백, -를 제외한 문자 예외처리					////
void sortbyname(info *, int, int);		   //이름순 정렬									////
void sortbyid(info *, int, int);		   //ID순 정렬										////
int inpsearchname(char **, char **);	   //탐색을 위한 이름입력받기						////
void inpsearchid(int *);				   //탐색을 위한 id입력받기							////
                                                                                            ////
void issuemenu();						   //메뉴 보여주기									////
int intro();							   //프로그램 시작지 안내문자열 출력하는 함수       ////
int checksure(int);						   //선택한 기능이 확실한지 확인하기				////
void checkmenu(int);					   //위 함수에서 쓰이는 print문 출력				////
																							////
int inputinfo(int);						   //데이터 입력받은거 파일에 쓰는 함수             ////
int inputrepeat(list *, list **, int *, int *);//키보드로 정보입력하는거 반복해주는 함수	////
int inputkeyboard(list *, list **);		   //키보드로 입력받는 함수							////
void ckmoreinput(char *);				   //정보 더 입력할건지 묻는함수					////
																							////
int strinp(char **);					   //키보드로 입력받는함수                          ////
int inpfirstname(list *);				   //FirstName 입력하는 함수                        ////
int inplastname(list *);				   //LastName 입력하는 함수							////
void randomid(list *, list *);			   //ID 랜덤생성하는 함수                           ////
																							////
int inpaddress(list *);				       //주소 입력하는 함수                             ////
int inpaddressone(list *);				   //첫번째 주소입력								////
int inpaddresstwo(list *);				   //두번째 주소입력								////
int inpaddressthree(list *);			   //세번째 주소입력								////
int inpaddressfour(list *);				   //네번째 주소입력								////
int inpaddressfive(list *);				   //마지막 주소입력								////
																							////
int inppnum(list *, list *);               //전화번호 입력하는 함수                         ////
																							////
void cmpinfolen(info, int *, int *);	   //이름, 주소 최대길이 만큼 공백문자 넣어주는함수 ////
int lenname(info);		                   //이름길이 계산                                  ////
int lenadrs(address);	                   //주소길이 계산                                  ////
																							////
int issueretouchmenu();					   //수정할 기능 선택하는 메뉴						////
int retouchfile();						   //파일 수정										////
char checkmoreretouch();				   //더 수정할건지 묻기								////
int deleteinfo(list *);					   //삭제											////
int addinfo(list *);					   //추가											////
int retouchinfo_search(list *);			   //수정할 항목 선택								////
int retouchitemselect();				   //어떤 항목 수정할지묻는 메뉴					//// 
int retouchinfo_retouch(list *, list *);   //수정할 항목 수정								////
int retouch_name(list *);				   //이름 재입력									////
int retouch_address_menu();				   //수정할 주소항목 선택							////
																							////
int retouch_address(list *);			   //주소 수정										////
int retouch_address_one(list *, list **);  //첫번째 주소 수정								////
int retouch_address_two(list *, list **);  //두번째 주소 수정								////
int retouch_address_three(list *, list **);//세번째 주소 수정								////
int retouch_address_four(list *, list **); //네번째 주소 수정								////
int retouch_address_five(list *, list **); //마지막 주소 수정								////
																							////
int retouch_phonenumber(list *, list *);   //전화번호 재입력								////
																							////
int issuedisplaymenu();					   //출력하는 방식 선택 메뉴						////
void displayselect(int);				   //출력하는 방식 선택하는 함수					////
void displaytosort(int);				   //정렬후 출력하는 함수							////
																							////
int displaysearch();					   //탐색해서 출력하는 함수							////
int searchselect();						   //탐색할 방식 선택								////
void displaysearchedinfo(list *);		   //선택한 정보 출력								////
char checkmoresearch();					   //더 탐색할지 확인								////
																							////
void displayinfo(FILE *);				   //파일의 데이터를 모니터에 출력하는 함수         ////
                                                                                            ////
void listtofile(list *, int, int);		   //list에서 파일로 데이터 쓰기				    ////
void headtofile(FILE **, int, int);		   //파일 가장윗부분 파일에 쓰기					////
void nametofile(FILE **, list **, int);	   //Name 파일에 쓰기								////
void idtofile(FILE **, list **);		   //ID 파일에 쓰기									////
void addresstofile(FILE **, list **, int); //Address 파일에 쓰기							////
void phonenumbertofile(FILE **, list **);  //PhoneNumber 파일에 쓰기						////
																							////
void arraytofile(info *, int, int, int);   //배열에서 파일로 데이터 쓰기					////
void arynametofile(FILE **, info *, int);  //Name 파일에 쓰기								////
void aryidtofile(FILE **, info *);		   //ID 파일에 쓰기									////
void aryaddresstofile(FILE **, info *, int);//Address 파일에 쓰기							////
void aryphonenumbertofile(FILE **, info *);//PhoneNumber 파일에 쓰기						////
																							////
int filetolist(list *);					   //파일에서 list로 데이터 읽기                    ////
int nametolist(FILE **, list **);		   //Name 파일에서 읽기								////
void idtolist(FILE **, list **);		   //ID 파일에서 읽기								////
int addresstolist(FILE **, list **);	   //Address 파일에서 읽기							////
int phonenumbertolist(FILE **, list **);   //PhoneNumber 파일에서 읽기						////
void checklentolist(list **, int *, int *);//이름, 주소 길이구하기							////
																							////
int filetoarray(info *, int *);			   //파일에서 배열로 데이터 읽기					////
int nametoarray(FILE **, info **, int);	   //Name 파일에서 읽기								////
void idtoarray(FILE **, info **, int);	   //ID 파일에서 읽기								////
int addresstoarray(FILE **, info **, int); //Address 파일에서 읽기							////
int pnumbertoarray(FILE **, info **, int); //PhoneNunber 파일에서 읽기						////
void checklentoarray(info *, int, int *, int *);//이름, 주소 길이구하기						////
																							////
void freelist(list *);                     //리스트 동적할당 해제하기                       ////
void freearray(info *, int);			   //배열 동적할당 해제하기							////
void freename(info *);					   //name에 동적할당받은부분 해제					////
void freeaddress(info *);				   //address에 동적할당받은부분 해제				////
void freephonenumber(info *);			   //전화번호에 동적할당받은부분 해제				////
																							////
void inputlist(list *, list *);			   //list에 노드추가하기							////
list* findlistbyname(list *, char *, char *);//이름으로 탐색								////
int chooseone(list **, int);			   //여러 동명이인중 한명 선택						////
list* findlistbyid(list *, int);		   //ID로 탐색										////
void deletelist(list *);				   //노드하나 삭제									////
																							////
int filetohash(list **, int);				//파일에서 hash로 데이터 읽기					////
void clearhashtable(list **, int);				//hash table 초기화							////
int hashfunctionbyname(char *);				//Name에 대한 key값 찾기						////
int hashfunctionbyid(int);					//ID에 대한 key값 찾기							////
void inputhash(list **, list *, int);		//hash table에 데이터 넣기						////
list* hashingbyname(list **, char *, char *);//Name으로 탐색								////
void minisort(list *, list **);				//탐색된 동명이인들 ID순 정렬					////
list* hashingbyid(list **, int);			//ID로 탐색										////
void freehash(list **, int);				//해시테이블 동적할당 해제						////
																							////
////////////////////////////////////////////////////////////////////////////////////////////////

int main(void)
{
	int cdt;                    //기능별 프로그램동작을 컨트롤해주는 변수

	while (1)                   //프로그램 종료할때까지 반복
	{
		cdt = intro();          //프로그램시작 시 안내해주는 문자열 출력
		if (cdt == EXIT) break; //종료한다고 선택하면 프로그램종료
		cdt = inputinfo(cdt)  ; //학생정보 student.db파일에 입력
		displayselect(cdt);     //student.db파일의 데이터 모니터에 출력
	}

	return 0;
}

/////////////////////////////////////     함수 정의     ////////////////////////////////////////
                                                                                            ////
void cleanstdin()              //키보드 입력버퍼 비우기
{
	while (getchar() != '\n'); //입력버퍼에 있는 데이터'\n'까지 가져옴
}

int ckalloc(void *vp) //void포인터 사용함으로서 다양한 자료형을 이 함수 하나로 검사가능
{
	if (vp == NULL)   //동적할당에 실패했다면 0반환
	{
		printf("동적할당을 실패했습니다.\n");
		return 0;
	}

	return 1;		  //동적할당에 성공했다면 1반환
}

void ckno(char **ckn)            //정보입력시 안하는부분(n입력한부분)확인해주는 함수
{
	if (strcmp(*ckn, "n") == 0)  //만약 "n"를 입력했다면
	{
		free(*ckn);              //해당 메모리를 반환한다
		*ckn = NULL;             //그리고 NULL포인터를 넣어준다
	}
}

void cleaninfo(info *p)     //각 멤버 기본값 NULL로 설정하기(직접 정의한 각 멤버free함수를 위해)
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

void prtline(info pl) //학생정보 한줄에출력
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

int excname(char **name)		//이름 및 1~3번째 주소 입력에 대한 예외탐색 함수
{
	int len = strlen(*name);	//탐색할 문자수
	int i;						//반복용 변수
	char t;						//비교할때 편하게 하기위한 변수

	for (i = 0; i < len; i++)	//공백문자(스페이스 or Tap) 가 있을때 2반환
	{
		t = (*name)[i];
		if (t==' '||t=='\t')	//t가 공백문자 또는 탭문자일떄 if문 실행됨
			return 2;
	}

	for (i = 0; i < len; i++)	//한글이 아닌데 영어도 아닐때 3 반환
	{
		t = (*name)[i];
		if (((t & 0x80) == 0) && !(t>='a'&&t<='z' || t>='A'&&t<='Z'))
			return 3;
	}

	return 1;	//위에서 return 안했으면 예외 없는것
}

int excaddr(char **addr)		//도로명/지번 주소에 대한 예외탐색 함수
{
	int len = strlen(*addr);	//탐색할 문자수
	int i;						//반복용 변수
	char t;						//비교할때 편하게 하기위한 변수

	for (i = 0; i < len; i++)	//공백문자(스페이스 or Tap) 가 있을때 2반환
	{
		t = (*addr)[i];
		if (t == ' ' || t == '\t')	//t가 공백문자 또는 탭문자일떄 if문 실행됨
			return 2;
	}

	for (i = 0; i < len; i++)		//한글 영어 숫자 - 모두 아닐때 3반환
	{
		t = (*addr)[i];
		if (((t & 0x80) == 0) && !(t >= 'a'&&t <= 'z' || t >= 'A'&&t <= 'Z'))
			if (!(t >= '0'&&t <= '9') && (t != '-'))
				return 3;
	}

	return 1;	//위에서 return 안했으면 예외 없는것
}

int excphone(char **pn)			//전화번호에 대한 예외탐색 함수
{
	int len = strlen(*pn);
	int i;
	char t;

	if ((len == 1) && ((*pn)[0] == 'n')) //전화기가 없다는 입력이 들어왔을때
		return 1;

	for (i = 0; i < len; i++)			 //공백문자(스페이스 or Tap) 가 있을때 2반환
	{
		t = (*pn)[i];
		if (t == ' ' || t == '\t')
			return 2;
	}

	for (i = 0; i < len; i++)		//숫자랑 - 외 다른게 있을떄 3반환
	{
		t = (*pn)[i];
		if (!(t >= '0'&&t <= '9') && (t != '-'))
			return 3;
	}

	return 1;	//정상입력시 1반환
}

void sortbyname(info *ary, int left, int right)
{
	info p = ary[left];		//pivot은 가장 왼쪽요소, 이때 ary[left]는 빈공간이나 다름없음
	int l = left;			//left대신 l사용(아래에서 left써야하므로 값이 변하면안됨)
	int r = right;			//right대신 r사용

	while (l < r)			//l<r일때만 반복문실행(l=r되면 나감)
	{
		while ((l < r))
		{
			if(strcmp(ary[r].fname, p.fname)==0)		//fname이 같을땐 lname으로 비교
			{
				if(strcmp(ary[r].lname, p.lname)<0)		//lname까지 같을때도 그냥 넘어감(정렬의 안정성)
					break;
			}
			else if(strcmp(ary[r].fname, p.fname)<0)
				break;

			r--;
		}

		if (l != r)							//이게 실행됬다는것은 ary[r]<pivot이라는것
		{
			ary[l] = ary[r];				//빈공간인 ary[l]에 ary[r]의값을대입, ary[r]이 빈공간이됨
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

		if (l != r)							//이게 실행됬다는것은 ary[l]>pivot이라는것
		{
			ary[r] = ary[l];				//빈공간인 ary[r]에 ary[l]값을 대입, ary[l]이 빈공간이됨
			r--;							//여기서 미리r값을 줄이면 맨위 while문 조건검사 안해도됨
		}
											//만약 위 두 if문이 실행되지않았다는것은 l==r이라는것이고, ary[l]=ary[r]=빈공간
	}

	ary[l] = p;								//최종적으로 빈공간에 pivot을 넣으면 pivot 왼쪽은 작은값, 오른쪽은 큰값이 있게됨
	//p = l;								//이때 index l은 pivot의 위치를 가리킴

	if (left < l)							//pivot의 위치가 left보다 오른쪽이라면 (pivot 왼쪽에 값이 존재할때만)
		sortbyname(ary, left, l - 1);		//왼쪽에대해 퀵정렬실행
	if (right > l)							//pivot의 위치가 right보다 왼쪽이라면 (pivot 오른쪽에 값이 존재할때만)
		sortbyname(ary, l + 1, right);		//오른쪽에대해 퀵정렬실행
}

void sortbyid(info *ary, int left, int right)
{
	info p = ary[left];		//pivot은 가장 왼쪽요소, 이때 ary[left]는 빈공간이나 다름없음
	int l = left;			//left대신 l사용(아래에서 left써야하므로 값이 변하면안됨)
	int r = right;			//right대신 r사용

	while (l < r)			//l<r일때만 반복문실행(l=r되면 나감)
	{
		while ((ary[r].id >= p.id) && (l < r))	//l==r이 되거나, ary[r]이 pivot보다 작은값일때 그만
			r--;

		if (l != r)							//이게 실행됬다는것은 ary[r]<pivot이라는것
		{
			ary[l] = ary[r];				//빈공간인 ary[l]에 ary[r]의값을대입, ary[r]이 빈공간이됨
		}

		while ((ary[l].id <= p.id) && (l < r))	//l==r이 되거나, ary[l]이 pivot보다 큰값일때 그만
			l++;

		if (l != r)							//이게 실행됬다는것은 ary[l]>pivot이라는것
		{
			ary[r] = ary[l];				//빈공간인 ary[r]에 ary[l]값을 대입, ary[l]이 빈공간이됨
			r--;							//여기서 미리r값을 줄이면 맨위 while문 조건검사 안해도됨
		}
											//만약 위 두 if문이 실행되지않았다는것은 l==r이라는것이고, ary[l]=ary[r]=빈공간
	}

	ary[l] = p;								//최종적으로 빈공간에 pivot을 넣으면 pivot 왼쪽은 작은값, 오른쪽은 큰값이 있게됨
	//p = l;								//이때 index l은 pivot의 위치를 가리킴

	if (left < l)							//pivot의 위치가 left보다 오른쪽이라면 (pivot 왼쪽에 값이 존재할때만)
		sortbyid(ary, left, l - 1);			//왼쪽에대해 퀵정렬실행
	if (right > l)							//pivot의 위치가 right보다 왼쪽이라면 (pivot 오른쪽에 값이 존재할때만)
		sortbyid(ary, l + 1, right);		//오른쪽에대해 퀵정렬실행
}

int inpsearchname(char **fname, char **lname) //탐색을 위한 이름 입력
{
	system("cls");
	printf("==< 탐색을 위한 이름 입력 >==\n");
	printf("@@ 영문 및 한글만 입력가능합니다. @@\n\n");
	while (1)
	{
		printf("%-15s", "First Name(성)"); printf("%c", ':');
		if (!strinp(fname))  //동적할당 실패했다면 0반환
		{
			return 0;
		}
		if (excname(fname) != 1)			//성 입력에 대한 예외처리, 예외가 발생했다면
		{
			printf("입력 : %s\n", *fname);
			printf("성에 영문 및 한글 외 문자, 숫자 또는 공백이 포함되었습니다.\n");
			printf("다시 입력해 주십시오.\n\n");
			free(*fname);
		}
		else												//정상적으로 입력되었다면
			break;
	}
	while (1)
	{
		printf("%-15s", "Last Name(이름)"); printf("%c", ':');
		if (!strinp(lname))  //동적할당 실패했다면 0반환
		{
			free(*fname);
			return 0;
		}
		if (excname(lname) != 1)			//이름 입력에 대한 예외처리, 예외가 발생했다면
		{
			printf("입력 : %s\n", *lname);
			printf("성에 영문 및 한글 외 문자, 숫자 또는 공백이 포함되었습니다.\n");
			printf("다시 입력해 주십시오.\n\n");
			free(*lname);
		}
		else												//정상적으로 입력되었다면
			break;
	}
	return 1; //정상적으로 입력되었다면
}

void inpsearchid(int *id) //탐색을 위한 학번 입력
{
	system("cls");
	printf("==< 탐색을 위한 학번 입력 >==\n");
	printf("@@ 숫자만 입력가능합니다. @@\n");
	printf("@@ 공백과 그 이후는 입력되지 않습니다. @@\n\n");
	while (1)
	{
		printf("  학번 : ");
		scanf("%d", id);
		cleanstdin();
		if (*id >= MINNUM && *id <= MAXNUM)
			break;
		else
		{
			printf("%d : 잘못된 입력입니다.\n", *id);
			printf("다시 입력해 주십시오.\n\n");
		}
	}
}

void issuemenu() //Main-Menu
{
	printf("====< 학생 정보 관리 프로그램 >====\n\n");
	printf("  1. 학생정보 새로만들기\n");
	printf("\n");
	printf("  2. 학생정보 수정하기\n");
	printf("  3. 학생정보 출력하기\n");
	printf("\n");
	printf("  0. 프로그램 종료하기\n\n");
	printf("===================================\n");
}

int intro()   //프로그램의 시작시 메뉴선택하는곳
{
	int cdt;  //기능 선택하기

	while (1)  //기능선택 올바르게할때까지 반복
	{
		issuemenu(); //메뉴 보여주기

		printf(" 작업 선택>> "); 
		scanf("%d", &cdt);
		cleanstdin();           //scanf로 입력후 버퍼 비워줌으로 공백문자오류 방지

		switch (cdt)            //switch-case 사용하여 지정된 숫자 외 입력들어온경우 잘못된 선택처리
		{
		case 1:                 //입력하겠다고 선택시
			if(checksure(cdt))	//선택한 기능 실행할지 확인
				return IN;
			else
				break;
		case 2:                 //수정선택
			if(checksure(cdt))
				return TOUCH;
			else
				break;
		case 3:                 //출력선택
			if(checksure(cdt))
				return OUT;
			else
				break;
		case 0:					//종료선택
			return EXIT;
		default:                //지정된 기능 외 이상한 입력 들어왔을때 선택으로 돌아감
			printf("잘못된 선택입니다.\n");
			break;
		}
		system("cls");
	}
}

int checksure(int cdt)	//확실한 선택인지 묻기
{
	char sel;

	checkmenu(cdt);	//각 선택에 대한 문장 출력

	while (1)
	{
		printf("계속 하시겠습니까? (y/n) : ");
		scanf("%c", &sel);
		cleanstdin();        //scanf로 입력후 버퍼 비워줌으로 공백문자오류 방지

		if (sel == 'y')
		{
			return 1;
		}
		else if (sel == 'n') //n선택시 다시 메뉴로 돌아가기
			return 0;
		else
		{
			printf("잘못된 선택입니다. y또는 n을 선택해주세요\n");
		}
	}
}

void checkmenu(int cdt)	//각 선택에 대한 문장
{
	system("cls");
	switch(cdt)
	{
	case 1:
		printf("==========< 새로 만들기 선택 >==========\n\n");
		printf(" 기존 student.db파일은 삭제됩니다.\n\n");
		break;
	case 2:
		printf("==========< 정보 수정하기 선택 >=========\n\n");
		printf(" 정보추가, 삭제, 수정작업을 진행합니다.\n\n");
		break;
	case 3:
		printf("==========< 정보 출력하기 선택 >=========\n\n");
		printf(" student.db파일의 내용을 화면에 출력합니다.\n\n");
		break;
	default:
		printf("그럴리가 없습니다.\n");
		break;
	}
}

int inputinfo(int a)      //프로그램에 정보입력, 파일에 쓰기 담당하는부분
{
	list head;            //head노드(head.llink=첫노드, head.flink=마지막노드)
	list *temp;           //임시노드
	int namelen=0;		  //이름에 배정될 칸수
	int addresslen=0;     //주소에 배정될 칸수

	head.llink = &head;   //list의 가장 앞 요소
	head.flink = &head;   //list의 가장 뒤 요소

	srand(time(NULL));    //난수발생 seed값 바꿔주기

	if(a==TOUCH)		  //만약 수정하기 선택했다면
	{
		a=retouchfile();  //수정하기 함수 호출
		if (a == PREV || a == MENU) //수정하기함수에서 이전 또는 메뉴로 돌아가길 원한다면
			return MENU;			//메뉴로 돌아가기(수정하기 이전=메뉴)
		else
			return IN;	  //수정했다면 IN을 리턴해 파일정보 출력해주기
	}
	else if(a==OUT)		  //출력하기 선택했다면 바로 출력으로가기
		return OUT;

	temp = (list *)malloc(sizeof(list)); //리스트 첫요소 생성
	if (!ckalloc(temp))					 //동적할당 확인, 실패시 -1반환(메뉴로 돌아가기)
	{
		printf("위치 : temp할당\n\n");
		return -1;
	}

	if (inputrepeat(&head, &temp, &namelen, &addresslen) == -1) //학생정보 반복입력, 실패시
	{
		listtofile(&head, namelen, addresslen); //실패전까지 입력한정보 파일에넣고
		return -1;								//-1반환
	}

	listtofile(&head, namelen, addresslen);   //리스트에서 파일로 데이터쓰기

	return IN; //입력했으면 1반환
}

int inputrepeat(list *head, list **temp, int *namelen, int *adrlen)
{
	char cdt;		   //학생정보 더 입력할건지 물어보는 조건문을 위한 변수

	while (1)          //학생정보 반복입력하여 list에 저장
	{
		system("cls"); //화면 비워주기

		printf("***학생정보입력***\n");

		cleaninfo(&((*temp)->student)); //입력하기전 모든멤버 NULL로 초기화해주기

		if (inputkeyboard(head, temp) == -1) //키보드로 입력, 실패하면 -1반환
			return -1;

		inputlist(head, *temp); //List에 새로운 노드삽입

		cmpinfolen((*temp)->student, namelen, adrlen); //이름, 주소길이 비교

		system("cls");

		printf("파일에 이렇게 입력됩니다.\n\n");
		prtline((*temp)->student); //방금 추가한 정보 한줄로 보여줌
		*temp = NULL;              //temp 다시 NULL로 비워주기

		ckmoreinput(&cdt); //학생정보 더 입력할건지 묻기
		if (cdt == 'n')
			break;         //학생정보 더 입력하지 않을거면 반복문 탈출하기

		*temp = (list *)malloc(sizeof(list)); //새로운 요소 만들기
		if (!ckalloc(*temp))                  //동적할당 확인, 실패시 -1반환
		{
			printf("위치 : temp할당\n\n");
			return -1;
		}
	}
	return 1; //성공시 1반환
}

int inputkeyboard(list *head, list **temp)	 //키보드로 정보 입력받는 함수
{
	if (!inpfirstname(*temp))				 //FirstName입력, 실패시 -1반환
	{
		printf("위치 : FirstName할당\n\n");
		free(*temp);						 //실패시 temp반환
		return -1;
	}
	if (!inplastname(*temp))				 //LastName입력, 실패시 -1반환
	{
		printf("위치 : LastName할당\n\n");
		freename(&((*temp)->student));		 //실패시 temp의 fname반환 후 temp반환
		free(*temp);
		return -1;
	}
	randomid(head, *temp);					 //ID 입력		
	if (!inpaddress(*temp))                  //Address입력, 실패시 -1반환
	{
		printf("위치 : Address할당\n\n");
		freename(&((*temp)->student));       //실패시 이름, 주소 입력한거까지 반환 후 temp반환
		freeaddress(&((*temp)->student));
		free(*temp);
		return -1;
	}
	if (!inppnum(head, *temp))               //PhoneNumber입력, 실패시 -1반환
	{
		printf("위치 : PhoneNumber할당\n\n");
		freename(&((*temp)->student));		 //실패시 이름, 주소반환후 temp반환
		freeaddress(&((*temp)->student));
		free(*temp);
		return -1;
	}
	return 1; //성공시 1반환
}

void ckmoreinput(char *cdt)
{
	while (1) //학생정보 더 입력할지 묻기
	{
		printf("학생정보를 더 입력 하시겠습니까?(y/n) : ");
		scanf("%c", cdt);
		cleanstdin();			  //scanf로 입력받고나서 버퍼지워주기

		if (*cdt == 'y' || *cdt == 'n') //y또는n제대로 입력되었는지 확인
			break;                      //제대로 입력됬으면 반복문 탈출
		else
			printf("y 또는 n 을 입력해 주십시오.\n");
	}
}

int strinp(char **inp)                        //키보드로부터 입력받아서 구조체멤버에 저장해주는함수
{
	char buff[BUFF_LEN];                      //이름, 주소, 전화번호 이걸로받아서 넉넉한배열 선언
	int wp = 1;

	while (wp)
	{
		wp = 0;                               //기본적으로 반복안함
		fgets(buff, sizeof(buff), stdin);     //공백문자 받기위해 fgets 함수 사용
		if (buff[strlen(buff) - 1] == '\n')   //문자열 끝에 들어간 개행문자를 널문자로 바꿔줌(fgets는 개행문자까지 가져옴)
			buff[strlen(buff) - 1] = '\0';
		else if (strlen(buff) >= BUFF_LEN)    //만약 문자열이 buff에 꽉찰때(넘칠때) 다시입력받아야함->반복
		{
			printf("너무 긴 문자열이 입력되었습니다.\n");
			printf("다시 입력해 주십시오.\n\n");
			printf("재입력 : ");
			wp = 1;
		}

		if (strlen(buff) <= 0)				   //문자 입력없이 엔터만 누른경우
		{
			printf("문자열을 입력해 주십시오.\n\n");
			printf("재입력 : ");
			wp = 1;
		}
	}
	*inp = (char *)malloc(strlen(buff) + 1);  //문자열길이 +1만큼 동적할당
	if (!ckalloc(*inp))                       //동작할당이 실패했으면 0반환되 !연산자에의해 if문 실행됨
	{
		return 0;                             //0반환 : 동적할당 실패
	}

	strcpy(*inp, buff);                       //문자열을 구조체에 복사해넣음

	return 1;                                 //1반환 : 동적할당 성공
}

int inpfirstname(list *temp)              //firstname입력
{
	printf("<1> 이름입력\n");
	printf("@@ 성과 이름은 영문 및 한글만 입력가능합니다. @@\n");
	printf("@@ 성과 이름에 공백을 포함시킬수 없습니다. @@\n\n");

	while (1)
	{
		printf("%-15s", "First Name(성)"); printf("%c", ':');
		if (!strinp(&(temp->student.fname)))  //동적할당 실패했다면 0반환
		{
			return 0;
		}
		if (excname(&(temp->student.fname)) != 1)			//성 입력에 대한 예외처리, 예외가 발생했다면
		{
			printf("입력 : %s\n", temp->student.fname);
			printf("성에 영문 및 한글 외 문자, 숫자 또는 공백이 포함되었습니다.\n");
			printf("다시 입력해 주십시오.\n\n");
			free(temp->student.fname);
		}
		else												//정상적으로 입력되었다면
			break;
	}

	return 1;                             //입력 성공시 1반환
}

int inplastname(list *temp)               //lastname 입력
{
	while (1)
	{
		printf("%-15s", "Last Name(이름)"); printf("%c", ':');
		if (!strinp(&(temp->student.lname)))  //동적할당 실패했다면 0반환
		{
			return 0;
		}
		if (excname(&(temp->student.lname)) != 1)		//이름 입력에 예외가 발생했다면
		{
			printf("입력 : %s\n", temp->student.lname);
			printf("이름에 영문 및 한글 외 문자, 숫자 또는 공백이 포함되었습니다.\n");
			printf("다시 입력해 주십시오.\n\n");
			free(temp->student.lname);
		}
		else											//정상적으로 입력되었다면
			break;
	}

	return 1;                             //입력 성공시 1반환
}

void randomid(list *head, list *temp)  //ID랜덤발생, 입력하는 함수
{
	list *p;  //요소 탐색을위한 리스트포인터
	int ck;

	while (1)
	{
		temp->student.id = RAND_ID; //minnum에서 maxnum까지 랜덤한수 뽑아주는거
		ck = 0;   //확인변수 초기화
		p = head->llink; //첫 요소부터 탐색
		while (p != head) //모든요소 탐색, 만약 첫요소라면 반복문이 실행안됨
		{
			if (p->student.id == temp->student.id) //ID가 중복되었다면
			{
				ck = 1;                                         //확인변수=1
				break;
			}
			p = p->llink;
		}
		if (ck != 1)    //ID가 중복되지않았다면
			break;   //나가기
	}

	printf("<2> ID 랜덤생성 : %d\n\n", temp->student.id);
}

int inpaddress(list *temp)   //주소 입력하는 함수
{
	if (!inpaddressone(temp))	//각 단계별 주소입력, 동적할당 실패시 0반환
		return 0;

	if (!inpaddresstwo(temp))
		return 0;

	if (!inpaddressthree(temp))
		return 0;

	if (!inpaddressfour(temp))
		return 0;

	if (!inpaddressfive(temp))
		return 0;

	return 1; //입력 성공시 1반환
}

int inpaddressone(list *temp)
{	
	int ck = 0;

	while (1)
	{
		system("cls");
		printf("<3-1> 첫번째 주소입력\n");
		printf("@@ 한글 밑 영어만 입력가능합니다. @@\n\n");

		if (ck == 1)
		{
			printf("입력 : %s\n", temp->student.adr.state);
			printf("잘못된 입력입니다. 다시 입력해 주십시오.\n\n");
			free(temp->student.adr.state);
			temp->student.adr.state = NULL;
			ck = 0;
		}

		printf("도, 시(특별시, 광역시 등) : ");

		if (!strinp(&(temp->student.adr.state)))		//동적할당실패시 0반환
			return 0;

		if (excname(&(temp->student.adr.state)) == 1)	//예외 없을시 1반환
			break;
		else											//예외 있으면 ck에 1대입
			ck = 1;
	}
	return 1;	//정상 입력시 1반환
}

int inpaddresstwo(list *temp)
{
	int ck = 0;

	while (1)
	{
		system("cls");
		printf("<3-2> 두번째 주소입력\n");
		printf("@@ 한글 밑 영어만 입력가능합니다. @@\n\n");

		if (ck == 1)
		{
			printf("입력 : %s\n", temp->student.adr.city);
			printf("잘못된 입력입니다. 다시 입력해 주십시오.\n\n");
			free(temp->student.adr.city);
			temp->student.adr.city = NULL;
			ck = 0;
		}

		printf("시, 군, 구 : ");

		if (!strinp(&(temp->student.adr.city)))			//동적할당실패시 0반환
			return 0;

		if (excname(&(temp->student.adr.city)) == 1)	//예외 없을시 1반환
			break;
		else											//예외 있으면 ck에 1대입
			ck = 1;
	}
	return 1;	//정상 입력시 1반환
}

int inpaddressthree(list *temp)
{
	int ck = 0;

	while (1)
	{
		system("cls");
		printf("<3-3> 세번째 주소입력\n");
		printf("@@ 한글 밑 영어만 입력가능합니다. @@\n");
		printf("@@ 해당사항이 없다면 n을 입력하십시오. @@\n\n");

		if (ck == 1)
		{
			printf("입력 : %s\n", temp->student.adr.village);
			printf("잘못된 입력입니다. 다시 입력해 주십시오.\n\n");
			free(temp->student.adr.village);
			temp->student.adr.village = NULL;
			ck = 0;
		}

		printf("구, 읍, 면 : ");

		if (!strinp(&(temp->student.adr.village)))		//동적할당실패시 0반환
			return 0;

		if (excname(&(temp->student.adr.village)) == 1)	//예외 없을시 1반환
			break;
		else											//예외 있으면 ck에 1대입
			ck = 1;
	}

	ckno(&(temp->student.adr.village));	//3번째 주소가 없을경우 확인
	return 1;	//정상 입력시 1반환
}

int inpaddressfour(list *temp)
{
	int ck = 0;

	while (1)
	{
		system("cls");
		printf("<3-4> 네번째 주소입력\n");
		printf("@@ 한글, 영어, 숫자, - 만 입력가능합니다. @@\n");
		printf("@@ 공백없이 입력해주십시오. @@\n\n");

		if (ck == 1)
		{
			printf("입력 : %s\n", temp->student.adr.street);
			printf("잘못된 입력입니다. 다시 입력해 주십시오.\n\n");
			free(temp->student.adr.street);
			temp->student.adr.street = NULL;
			ck = 0;
		}

		printf("도로명, 지번 : ");

		if (!strinp(&(temp->student.adr.street)))		//동적할당실패시 0반환
			return 0;

		if (excaddr(&(temp->student.adr.street)) == 1)	//예외 없을시 1반환
			break;
		else											//예외 있으면 ck에 1대입
			ck = 1;
	}
	return 1;	//정상 입력시 1반환
}

int inpaddressfive(list *temp)
{
	int len;
	int i;

	system("cls");
	printf("<3-5> 마지막 상세주소 입력\n\n");

	printf("상세주소 : ");

	if (!strinp(&(temp->student.adr.home)))		//동적할당실패시 0반환
		return 0;

	len = strlen(temp->student.adr.home);

	for (i = 0; i < len; i++)					//공백부분에 _을 대신넣기
	{
		if ((temp->student.adr.home)[i] == ' ' || (temp->student.adr.home)[i] == '\t')
			(temp->student.adr.home)[i] = '_';
	}

	return 1;	//정상 입력시 1반환
}

int inppnum(list *head, list *temp) //전화번호 입력받는 함수
{
	list *p;
	int ck = 0;

	while (1)
	{
		system("cls");
		printf("<4> 전화번호입력\n");
		printf("@@ 전화번호는 xxx-xxxx-xxxx 형식으로 입력해주십시오. @@\n");
		printf("@@ 전화번호에는 숫자와 - 만 입력 가능합니다. @@\n");
		printf("@@ 전화기가 없으면 n 을 입력해 주십시오. @@\n\n");

		if (ck == 1)
		{
			printf("전화번호가 중복되었습니다. : %s\n", temp->student.pnum);
			printf("다시 입력해 주십시오.\n\n");
			free(temp->student.pnum);                      //저장했던곳 반환
		}
		else if (ck == 2)
		{
			printf("전화번호에 숫자와 - 외 다른문자 또는 공백이 들어갔습니다.\n");
			printf("다시 입력해 주십시오\n\n");
		}

		printf("전화번호 : ");
		if (!strinp(&(temp->student.pnum))) //전화번호 입력, 동적할당 실패시 0반환
			return 0;

		ck = 0;           //확인변수 초기화

		if (excphone(&(temp->student.pnum)) != 1)
		{
			printf("입력 : %s\n", temp->student.pnum);
			free(temp->student.pnum);
			temp->student.pnum = NULL;
			ck = 2;
		}

		p = head->llink;  //첫 요소부터 탐색
		while (p != head && (temp->student.pnum != NULL)) //모두탐색, 첫노드일경우 반복문 실행안됨
		{
			if (strcmp(temp->student.pnum, p->student.pnum) == 0) //전화번호가 중복됬다면
			{
				ck = 1;                                         //확인변수=1
				break;
			}
			p = p->llink;
		}
		if (ck != 1 && ck != 2)    //전화번호가 중복되지않았고 예외가 없었으면
			break;				   //나가기
	}

	ckno(&(temp->student.pnum));    //n 입력했는지 확인하기

	if (temp->student.pnum == NULL) //전화기가 없다면
	{
		temp->student.pnum = (char *)malloc(strlen(NO_PHONE) + 1); //전화기가 없다는 뜻의 문자열을 넣음
		if (!ckalloc(temp->student.pnum))                        //동적할당 실패시 0반환
			return 0;
		strcpy(temp->student.pnum, NO_PHONE);
		(temp->student.pnum)[strlen(NO_PHONE)] = '\0';
	}
	return 1; //성공시 1반환
}

void cmpinfolen(info inf, int *nlen, int * adlen) //이름, 주소 최대길이 구하는함수
{
	int temp;		       //길이구하는 함수 두번호출되지않게하기위해(조건문에서만 호출하게함)

	if ((temp = lenname(inf)) > *nlen)       //조건검사 하면서 변수에 값 대입
		*nlen = temp;						 //더 길면 그 값 대입

	if ((temp = lenadrs(inf.adr)) > *adlen)  //위랑 같음
		*adlen = temp;
}

int lenname(info name)   //이름, 주소 최대길이 구하는 함수에서 이름길이 뽑을떄 사용하는 함수
{
	return strlen(name.fname) + strlen(name.lname) + 1;  //fname + lname + 1(성, 이름사이에 공백) 리턴
}

int lenadrs(address adr)  //이름, 주소 최대길이 구하는 함수에서 주소길이 뽑을떄 사용하는 함수
{
	int len = 0;     //총 주소 길이
	int eac = 0;	 //주소 갯수(도, 시, 구, 도로명, 상세주소)

	len += strlen(adr.state);   //그 문자열의 길이를 len에 더해줌
	eac++;                      //문자열갯수++

	len += strlen(adr.city);
	eac++;

	if (adr.village != NULL)
	{
		len += strlen(adr.village);
		eac++;
	}
	else
	{
		len += 1;			//NULL인경우 그자리에 #이 들어가야하므로 글자수 1개로 함
		eac++;
	}

	len += strlen(adr.street);
	eac++;

	len += strlen(adr.home);

	len += eac;    //주소의 길이 + 공백의 갯수

	return len;    //주소 총 길이 반환(주소사이 공백 포함)
}

int issueretouchmenu()	//수정하는 함수 메뉴선택하는 곳
{
	int sel;

	system("cls");
	printf("====<< student.db 내용 수정 >>====\n");
	printf("  1. 학생정보 추가\n");
	printf("  2. 학생정보 수정\n");
	printf("  3. 학생정보 삭제\n");
	printf("\n");
	printf("  4. 이전으로 돌아가기\n");
	printf("===================================\n\n");
	
	while(1)
	{
		printf("  선택 : ");
		scanf("%d", &sel);
		cleanstdin();

		if(sel==1 || sel==2 || sel==3 || sel==4)
			break;
		else
			printf("잘못된 선택입니다.\n\n");
	}
	return sel;	
}

int retouchfile()	//파일내용 수정함수(삭제, 추가, 수정)
{
	int sel;				//여러가지 조건검사를 위한 변수
	char ck;				//더 수정할지 여부 확인하는 변수
	list head;				//파일을 list로 가져올곳
	int namelen, adrlen;	//다시 list로 데이터 넣기위해 이름, 주소 길이구하기
	list *hd=&head;			//아래에 head의 이중포인터 줘야하는데 &(&head)이런식으로는 안되서 포인터변수 사용

	head.flink=&head;		//head노드 초기화
	head.llink=&head;

	sel = filetolist(&head);	//file에서 list로 받기, -1:파일오픈실패 0:동적할당실패
	if (sel == -1)
		return MENU;
	else if (sel == 0)
	{
		printf("파일 읽어오는중 동적할당에 실패했습니다.\n");
		freelist(&head);
		return MENU;
	}

	while(1)
	{
		sel=issueretouchmenu();	//메뉴에서 하나 선택하기

		switch(sel)
		{
		case 1:
			sel = addinfo(&head);	//학생정보 추가
			if(sel==0)
				printf("학생정보 추가에 실패하였습니다.\n\n");
			break;
		case 2:
			sel=retouchinfo_search(&head);	//학생정보 수정
			break;
		case 3:
			sel=deleteinfo(&head);		//학생정보 삭제
			break;
		case 4:
			checklentolist(&hd, &namelen, &adrlen);	//이전으로(메인메뉴로)돌아가기 - 현재까지 수정내용 파일에 옮김
			listtofile(&head, namelen, adrlen);
			return PREV;
		}
		if (sel == MENU)		//각 수정메뉴에서 메인메뉴로 돌아가기 선택시 돌아감
		{
			checklentolist(&hd, &namelen, &adrlen);
			listtofile(&head, namelen, adrlen);
			return MENU;
		}
		else if (sel == PREV)	//각 수정메뉴에서 이전메뉴로 돌아가기 선택시 아무기능 안함(다시 위에꺼 반복함)
		{
		}
		else
		{
			ck = checkmoreretouch();	//다른 학생정보도 수정할건지 묻기
			if (ck == 'n')
				break;
		}
	}
	checklentolist(&hd, &namelen, &adrlen);
	listtofile(&head, namelen, adrlen);
	system("cls");
	return 1;	//성공적 수정시 1반환
}

char checkmoreretouch()	//다른 학생정보도 수정할것인가?
{
	char sel;

	while(1)
	{
		printf("다른 학생 정보도 수정하시겠습니까? (y/n) : ");
		scanf("%c", &sel);
		cleanstdin();
		if (sel == 'y' || sel == 'n')
			break;
		else
			printf("수정하려면 y, 메뉴로 돌아가려면 n을 선택해 주십시오.\n\n");
	}
	return sel;
}

int deleteinfo(list *head)	//파일내용 삭제
{
	list *p;				//삭제할 노드
	int sel;
	char *fname, *lname;	//탐색을 위한 이름
	int id;					//탐색을 위한 ID

	system("cls");
	sel=searchselect();		//어떤방식으로 탐색할지 고르기

	switch(sel)
	{
	case 1:
		if (inpsearchname(&fname, &lname) != 1)	//탐색을 위한 이름입력
		{
			printf("이름 검색과정에서 동적할당에 실패하였습니다.\n");
			return MENU;
		}
		p=findlistbyname(head, fname, lname);	//list에서 순차적 탐색으로 노드찾기
		break;
	case 2:
		inpsearchid(&id);
		p=findlistbyid(head, id);	//list에서 순차적 탐색으로 노드찾기
		break;
	case 3:
		return PREV;	//이전메뉴로 돌아가기
	case 4:
		return MENU;	//메인메뉴로 돌아가기
	}
	if(p==NULL)
	{
		printf("해당 학생정보가 없습니다.\n\n");
	}
	else
	{
		deletelist(p);
		printf("정보 삭제 완료\n\n");
	}
	return 1;
}

int addinfo(list *head)		//파일내용 추가
{
	list *temp;

	system("cls");
	temp = (list *)malloc(sizeof(list)); //동적 할당
	if (!ckalloc(temp))					 //동적할당 확인, 실패시 0반환
	{
		return 0;
	}

	cleaninfo(&(temp->student));		  //info의 모든 포인터는 NULL인채로 시작

	if (inputkeyboard(head, &temp) == -1) //키보드로 입력, 실패하면 0반환
	{
		return 0;
	}

	inputlist(head, temp); //List에 새로운 노드삽입
	system("cls");
	printf("정보 추가 완료.\n");
	prtline(temp->student);	//추가한 정보 짧게 보여주기
	printf("\n");
	return 1;
}

int retouchinfo_search(list *head)	//파일내용 수정
{
	list *p;	//수정할 노드
	int sel;
	char *fname, *lname;	//탐색을 위한 이름
	int id;					//탐색을 위한 ID

	system("cls");
	while (1)
	{
		sel = searchselect();

		switch (sel)
		{
		case 1:
			if (inpsearchname(&fname, &lname) != 1)	//탐색을 위한 이름 입력
			{
				printf("이름 검색과정에서 동적할당에 실패하였습니다.\n");
				return MENU;
			}
			p = findlistbyname(head, fname, lname);	//list에서 순차적으로 노드찾기
			break;
		case 2:
			inpsearchid(&id);
			p = findlistbyid(head, id);
			break;
		case 3:
			return PREV;	//이전메뉴로 돌아가기
		case 4:
			return MENU;	//메인메뉴로 돌아가기
		}
		if (p == NULL)
		{
			system("cls");
			printf("해당 학생정보가 없습니다.\n\n");
		}
		else
		{
			sel = retouchinfo_retouch(head, p);	//학생정보 있으면 수정하기 실행
			if (sel == 0)
			{
				printf("정보 수정에 실패했습니다.\n");
				printf("해당 정보는 변경되지 않습니다.\n\n");
			}
			else if (sel == MENU)
				return MENU;
			else if (sel == PREV)
			{
			}
			else
			{
				system("cls");
				printf("정보수정 완료\n");
				prtline(p->student);
				printf("\n");
				break;
			}
		}
	}
	return 1;
}

int retouchitemselect()		//어떤 항목 수정할건지 고르기
{
	int sel;

	printf("=====< 수정할 항목 >=====\n");
	printf("  1. 이름\n");
	printf("  2. 주소\n");
	printf("  3. 전화번호\n");
	printf("\n");
	printf("  4. 이전으로 돌아가기\n");
	printf("  5. 메뉴로 돌아가기\n");
	printf("=========================\n");

	while(1)
	{
		printf("  선택 : ");
		scanf("%d", &sel);
		cleanstdin();

		if(sel>0 && sel<6)
			break;
		else
			printf("잘못된 선택입니다.\n\n");
	}
	return sel;	
}

int retouchinfo_retouch(list *head, list *p)
{
	int sel;

	while (1)
	{
		system("cls");
		printf("정보 : ");
		prtline(p->student);	//먼저 수정하려는 현재 정보 보여주기
		printf("\n\n");
		sel = retouchitemselect();	//수정할 항목 고르기

		switch (sel)
		{
		case 1:		//이름
			if (!retouch_name(p))
			{
				return 0;
			}
			break;
		case 2:		//주소
			sel = retouch_address(p);
			if (sel == 0)
				return 0;
			break;
		case 3:		//전화번호
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
	if (!inpfirstname(&temp))				 //FirstName입력, 실패시 0반환
	{
		return 0;
	}
	if (!inplastname(&temp))				 //LastName입력, 실패시 0반환
	{
		freename(&(temp.student));
		return 0;
	}

	freename(&(p->student));				//기존 정보 반환후 새 정보 넣기
	p->student.fname=temp.student.fname;
	p->student.lname=temp.student.lname;
	return 1;
}

int retouch_address_menu()	//어떤 주소 수정할건지 선택하기
{
	int sel;

	system("cls");
	printf("==< 수정할 주소 항목 선택 >==\n");
	printf("  1. 첫번째 주소(도, 시)\n");
	printf("  2. 두번쨰 주소(시, 군, 구)\n");
	printf("  3. 세번째 주소(구, 읍, 면)\n");
	printf("  4. 도로명/ 지번 주소\n");
	printf("  5. 상세 주소\n");
	printf("\n");
	printf("  6. 이전으로 돌아가기\n");
	printf("  7. 메뉴로 돌아가기\n");
	printf("=============================\n");
	
	while (1)
	{
		printf("  선택 : ");
		scanf("%d", &sel);
		cleanstdin();

		if (sel > 0 && sel < 8)
			return sel;
		else
			printf("잘못된 선택입니다.\n\n");
	}
}

int retouch_address(list *p)
{
	int sel;
	list temp;

	cleaninfo(&(temp.student));	//수정을 위한 임시노드의 모든 포인터는 NULL로 시작

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
	if (!(inpaddressone(temp)))	//수정할 내용 입력(동적할당 실패시 0반환)
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
	if (!(inpaddresstwo(temp)))	//수정할 내용 입력(동적할당 실패시 0반환)
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
	if (!(inpaddressthree(temp)))	//수정할 내용 입력(동적할당 실패시 0반환)
		return 0;
	else
	{
		if ((*p)->student.adr.village != NULL)	//해당 주소가 있을경우에만 반환
			free((*p)->student.adr.village);
		(*p)->student.adr.village = temp->student.adr.village;
		return 1;
	}
}

int retouch_address_four(list *temp, list **p)
{
	if (!(inpaddressfour(temp)))	//수정할 내용 입력(동적할당 실패시 0반환)
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
	if (!(inpaddressfive(temp)))	//수정할 내용 입력(동적할당 실패시 0반환)
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

	if (!inppnum(head, &temp))               //PhoneNumber입력, 실패시 0반환
	{
		return 0;
	}

	freephonenumber(&(p->student));
	p->student.pnum=temp.student.pnum;
	return 1;
}

int issuedisplaymenu()	//출력방식 선택하기
{
	int sel;

	system("cls");
	printf("====<< student.db 내용 출력 >>====\n");
	printf("  1. 이름순 정렬후 출력\n");
	printf("  2. 학번순 정렬후 출력\n");
	printf("  3. 입력한 순서대로 출력\n");
	printf("  4. 학생정보 검색하여 출력\n");
	printf("\n");
	printf("  5. 이전으로 돌아가기\n");
	printf("  6. 메뉴로 돌아가기\n");
	printf("==================================\n");
	printf("  선택 : ");
	scanf("%d", &sel);
	cleanstdin();
	return sel;
}

void displayselect(int cdt)
{
	int sel;
	FILE *fp;

	if (cdt == -1)	//입력부에서 동적할당실패시 메인메뉴로 돌아감
	{
		return;
	}
	if (cdt == IN)	//입력후 파일내용 출력할땐 그대로 출력해줌
	{
		fp = fopen("student.db", "rt");
		if(fp==NULL)
		{
			printf("파일이 존재하지 않습니다.\n\n");
			return;
		}
		displayinfo(fp);
		fclose(fp);
		return;
	}
	if (cdt == MENU)	//메인메뉴로 돌아가기 선택시 아무것도 안하고 리턴
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
			displaytosort(1);	//1을 주면 이름정렬
			break;
		case 2:
			displaytosort(2);	//2를 주면 ID정렬
			break;
		case 3:
			fp = fopen("student.db", "rt");
			if(fp==NULL)
			{
			printf("파일이 존재하지 않습니다.\n\n");
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
			printf("잘못된 선택입니다.\n");
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
	FILE *fp;		   //정렬후 파일에 넣기위한 파일포인터
	info ary[ARY_LEN]; //학생정보를위한 구조체배열
	int index;         //배열의 어디까지입력됬는지 기억
	int ck;
	int namelen, adrlen;	//파일에 넣기위한 이름, 주소 길이구하기

	ck = filetoarray(ary, &index); //파일의 내용을 배열에 읽기
	if (ck == 0)                   //리턴값이 0이면 동적할당실패, 메인메뉴로 돌아감
	{
		printf("위치 : FileToArray\n\n");
		return;
	}
	else if (ck == -1)             //리턴값이 -1이면 파일오픈실패, 메인메뉴로 돌아감
	{
		return;
	}
	if(index==0)	//정렬후 출력하려고 봤더니 정보가 없을때
	{
		printf("입력된 정보가 없습니다.\n");
		return;
	}

	if(sel==1)	//1:이름정렬 2:ID정렬
		sortbyname(ary, 0, index-1);
	else
		sortbyid(ary, 0, index-1);

	checklentoarray(ary, index, &namelen, &adrlen);	//이름, 주소길이 구하기
	arraytofile(ary, index, namelen, adrlen);		//정렬된 정보 파일에 넣기(sortedstudent.db)
	fp=fopen("sortedstudent.db", "rt");				//이제 정렬된거 출력하기위해 파일열기
	if (fp == NULL)
	{
		printf("파일이 없거나 여는데 실패하였습니다.\n");
		return;
	}
	displayinfo(fp);		//파일의 정보 출력
	fclose(fp);
	freearray(ary, index);	//다 쓴 정보 반환하기
}

int displaysearch()
{
	list *nht[HASH_NAME_LEN];	//이름탐색을 위한 Hash Table 만들기
	list *iht[HASH_ID_LEN];		//ID탐색을 위한 Hash Table 만들기
	list *p;					//탐색되서 출력할 노드(연결리스트)
	char *fname, *lname;		//탐색에쓸 이름
	int id, sel, hck;			//탐색에쓸 ID, 여러 조건문을위한 sel, hck

	clearhashtable(nht, HASH_NAME_LEN);	//각 hash table 초기화
	clearhashtable(iht, HASH_ID_LEN);
	hck = filetohash(nht, 1);			//먼저 파일에서 이름을위한 hash table로
	if (hck == -1)
		return MENU;
	else if (hck == 0)
	{
		printf("파일 읽어오는중 동적할당에 실패했습니다.\n");
		freehash(nht, HASH_NAME_LEN);
		return MENU;
	}
	hck = filetohash(iht, 2);			//파일에서 ID위한 hash table로
	if (hck == -1)
		return MENU;
	else if (hck == 0)
	{
		printf("파일 읽어오는중 동적할당에 실패했습니다.\n");
		freehash(iht, HASH_ID_LEN);
		return MENU;
	}

	while(1)
	{
		system("cls");
		sel=searchselect();	//탐색할 방식 고르기

		switch(sel)
		{
		case 1:
			if (inpsearchname(&fname, &lname) != 1)
			{
				printf("이름 검색과정에서 동적할당에 실패하였습니다.\n");
				return MENU;
			}
			p = hashingbyname(nht, fname, lname);	//이름으로 정보찾기(동명이인의 경우 연결되서 나옴)
			break;
		case 2:
			inpsearchid(&id);
			p = hashingbyid(iht, id);
			if (p != NULL)					//정보찾은경우 flink를 NULL로 해줌(출력을위함)
				p->flink = NULL;
			break;
		case 3:
			freehash(nht, HASH_NAME_LEN);	//이전메뉴로 돌아가기위해 사용한 메모리 반환먼저 함
			freehash(iht, HASH_ID_LEN);
			return PREV;
		case 4:
			freehash(nht, HASH_NAME_LEN);
			freehash(iht, HASH_ID_LEN);
			system("cls");
			return MENU;
		}

		displaysearchedinfo(p);	//찾은 정보 보여주기

		sel=checkmoresearch();	//더 찾아볼건지 묻기
		if (sel == 'n')
			break;		
	}
	freehash(nht, HASH_NAME_LEN);
	freehash(iht, HASH_ID_LEN);
	system("cls");
	return 1;
}

int searchselect()	//검색방식 묻기
{
	int sel;

	printf("====<< 검색 방식 >>====\n");
	printf("  1. 이름으로 검색\n");
	printf("  2. 학번으로 검색\n");
	printf("\n");
	printf("  3. 이전으로 돌아가기\n");
	printf("  4. 메뉴로 돌아가기\n");
	printf("=======================\n\n");
	while(1)
	{
		printf("  선택 : ");
		scanf("%d", &sel);
		cleanstdin();
		if(sel>0 && sel<5)
			break;
		else
			printf("잘못된 선택입니다.\n\n");
	}
	return sel;
}

void displaysearchedinfo(list *p)
{
	int sel;

	system("cls");
	printf("========< 선택한 학생의 정보 >====================================================\n\n");

	if (p == NULL)
		printf("\n  해당 학생정보가 없습니다.\n\n");
	else
	{
		sel = 1;				//sel : 번호출력(1, 2, 3...)
		while (p != NULL)
		{
			printf("  %d. ", sel);
			prtline(p->student);
			p = p->flink;		//flink : 동명이인의경우 flink를 통해 이어짐(단순연결)
			sel++;
		}
	}
}

char checkmoresearch()
{
	char sel;

	while(1)
	{
		printf("더 검색하시겠습니까? (y/n) : ");
		scanf("%c", &sel);
		cleanstdin();
		if (sel == 'y' || sel == 'n')
			break;
		else
			printf("더 검색하려면 y, 메뉴로 돌아가려면 n을 선택해 주십시오.\n\n");
	}
	return sel;
}

void displayinfo(FILE *stdinf)    //프로그램에 파일을 읽어서 출력하는것을 담당하는부분
{
	int i;
	
	system("cls");
	printf("==============================< 학생 정보 출력 >=================================================\n\n");
	while (!feof(stdinf))      //파일의 모든내용 콘솔창에 출력
	{
		i = fgetc(stdinf);	   //파일로부터 문자하나를 받아 변수i에 저장
		if(i=='#' || i=='_')   //공백대신 넣은 _, 없다는뜻의 # 대신에 공백출력
			putchar(' ');
		else
			putchar(i);        //문자 하나 출력
	}
	printf("\n");
	printf("=================================================================================================\n\n");
}

void listtofile(list *head, int namelen, int adrlen)  //리스트에서 파일로 데이터 출력하기
{
	FILE *stdinf;						   //student.db를 위한 파일포인터변수
	list *p = head->llink;			 	   //p는 첫번째노드

	int num = 1;						   //Num부분 파일에쓰기

	if(p == head)						   //내용이 없다면 파일을 삭제함
	{
		remove("student.db");
		return;
	}

	stdinf = fopen("student.db", "wt");    //쓰기, 텍스트모드로 열기(파일이 없으면 생성, 파일이 있으면 삭제후 생성)
	if (stdinf == NULL)                    //파일 열기 실패한다면
	{
		printf("파일 오픈에 실패하였습니다.\n");
		printf("프로그램을 종료합니다.\n");
		exit(1);						   //프로그램 강제종료
	}

	headtofile(&stdinf, namelen, adrlen);  //파일의 가장윗부분(Num  Name  ....) 쓰기

	while (p != head)					   //모든요소
	{
		fprintf(stdinf, "%-3d  ", num);    //Num부분 숫자입력, 3칸 지정해줌(999개 정보 입력가능)

		nametofile(&stdinf, &p, namelen);  //이름 파일에쓰기
		idtofile(&stdinf, &p);			   //ID 파일에쓰기
		addresstofile(&stdinf, &p, adrlen);//Address 파일에 쓰기
		phonenumbertofile(&stdinf, &p);    //Phonenumber 파일에 쓰기

		num++;							   //Num1증가
		p = p->llink;					   //다음요소참조
	}

	freelist(head);						   //동적할당받은 list 반환하기
	fclose(stdinf);						   //파일 닫기
}

void headtofile(FILE **stdinf, int namelen, int adrlen) //파일 윗부분 Num  Name .... 쓰기
{
	int j;									 //반복제어변수

	fprintf(*stdinf, "Num  Name  ");         //이름과 주소에 배정되는 칸의 수는 입력되는 정보에따라 유동적으로 설정함
	for (j = 0; j < namelen - 4; j++)        //Name이 4칸차지하므로 그건빼고 공백넣기
		fputc(' ', *stdinf);
	fprintf(*stdinf, "Student-ID  Address  ");
	for (j = 0; j < adrlen - 7; j++)         //Address가 7칸차지하므로 그건빼고 공백넣기
		fputc(' ', *stdinf);
	fprintf(*stdinf, "PhoneNumber\n");
}

void nametofile(FILE **stdinf, list **p, int namelen) //파일에 이름쓰기
{
	int len = 0; //이름길이구하기
	int j;		 //반복제어변수

	fprintf(*stdinf, "%s %s  ", (*p)->student.fname, (*p)->student.lname);            //이름입력 : 성(공백)이름(공백)(공백)
	len = namelen - (strlen((*p)->student.fname) + strlen((*p)->student.lname) + 1);  //이름옆에 공백문자 넣을칸수 알아내기(이름 최대길이 - 입력한 이름길이)
	for (j = 0; j < len; j++)														  //이름옆에 넣을 공백수만큼 공백넣음
		fputc(' ', *stdinf);
}

void idtofile(FILE **stdinf, list **p) //파일에 ID쓰기
{
	fprintf(*stdinf, "%d  ", (*p)->student.id); //Student-ID 입력 : ID(공백)(공백)
}

void addresstofile(FILE **stdinf, list **p, int adrlen) //파일에 Address쓰기
{
	int j = 0;   //주소길이 누적, 반복용변수
	int len = 0; //주소길이 구하기


	fprintf(*stdinf, "%s ", (*p)->student.adr.state);  //파일에 쓰고
	j += strlen((*p)->student.adr.state) + 1;          //그 문자열의 길이+1(주소사이의 공백)을 현재주소길이에 더한다


	fprintf(*stdinf, "%s ", (*p)->student.adr.city);   //위 내용 반복
	j += strlen((*p)->student.adr.city) + 1;

	if ((*p)->student.adr.village != NULL)			   //3번째 주소는 NULL이 올수있다
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
	
	j--;						//j-- 하면 현재입력할 주소길이
	len = adrlen - j;			//주소길이 최대값-입력할길이=공백길이
	for (j = 0; j < len; j++)   //남는칸만큼 공백입력
		fputc(' ', *stdinf);
}

void phonenumbertofile(FILE **stdinf, list **p)   //파일에 PhoneNumber쓰기
{
	fprintf(*stdinf, " %s\n", (*p)->student.pnum); //PhoneNumber 입력
}

void arraytofile(info *ary, int index, int namelen, int adrlen)  //리스트에서 파일로 데이터 출력하기
{
	FILE *stdinf;						   //student.db를 위한 파일포인터변수
	int i;

	int num = 1;						   //Num부분 파일에쓰기

	stdinf = fopen("sortedstudent.db", "wt");//쓰기, 텍스트모드로 열기(파일이 없으면 생성, 파일이 있으면 삭제후 생성)
	if (stdinf == NULL)                    //파일 열기 실패한다면
	{
		printf("파일 오픈에 실패하였습니다.\n");
		printf("프로그램을 종료합니다.\n");
		exit(1);						   //프로그램 강제종료
	}

	headtofile(&stdinf, namelen, adrlen);  //파일의 가장윗부분(Num  Name  ....) 쓰기

	for(i=0;i<index;i++)
	{
		fprintf(stdinf, "%-3d  ", num);    //Num부분 숫자입력, 3칸 지정해줌(999개 정보 입력가능)

		arynametofile(&stdinf, &(ary[i]), namelen);  //이름 파일에쓰기
		aryidtofile(&stdinf, &(ary[i]));			 //ID 파일에쓰기
		aryaddresstofile(&stdinf, &(ary[i]), adrlen);//Address 파일에 쓰기
		aryphonenumbertofile(&stdinf, &(ary[i]));    //Phonenumber 파일에 쓰기

		num++;							   //Num1증가
	}

	fclose(stdinf);						   //파일 닫기
}

void arynametofile(FILE **stdinf, info *p, int namelen) //파일에 이름쓰기
{
	int len = 0; //이름길이구하기
	int j;		 //반복제어변수

	fprintf(*stdinf, "%s %s  ", p->fname, p->lname);            //이름입력 : 성(공백)이름(공백)(공백)
	len = namelen - (strlen(p->fname) + strlen(p->lname) + 1);  //이름옆에 공백문자 넣을칸수 알아내기(이름 최대길이 - 입력한 이름길이)
	for (j = 0; j < len; j++)									//이름옆에 넣을 공백수만큼 공백넣음
		fputc(' ', *stdinf);
}

void aryidtofile(FILE **stdinf, info *p) //파일에 ID쓰기
{
	fprintf(*stdinf, "%d  ", p->id); //Student-ID 입력 : ID(공백)(공백)
}

void aryaddresstofile(FILE **stdinf, info *p, int adrlen) //파일에 Address쓰기
{
	int j = 0;   //주소길이 누적, 반복용변수
	int len = 0; //주소길이 구하기


	fprintf(*stdinf, "%s ", p->adr.state);  //파일에 쓰고
	j += strlen(p->adr.state) + 1;          //그 문자열의 길이+1(주소사이의 공백)을 현재주소길이에 더한다


	fprintf(*stdinf, "%s ", p->adr.city);   //위 내용 반복
	j += strlen(p->adr.city) + 1;

	if (p->adr.village != NULL)			   //3번째 주소는 NULL이 올수있다
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
	
	j--;							//j-- 하면 현재입력할 주소길이
	len = adrlen - j;			//주소길이 최대값-입력할길이=공백길이
	for (j = 0; j < len; j++)   //남는칸만큼 공백입력
		fputc(' ', *stdinf);
}

void aryphonenumbertofile(FILE **stdinf, info *p)   //파일에 PhoneNumber쓰기
{
	fprintf(*stdinf, " %s\n", p->pnum); //PhoneNumber 입력
}

int filetolist(list *head)		//파일에서 리스트로 데이터 입력하기
{
	FILE *rd;									 //읽을 파일포인터
	list *temp;									 //임시노드
	int i;										 //반복제어변수

	temp = (list *)malloc(sizeof(list));		 //첫 노드를위한 동적할당
	if (!ckalloc(temp))							 //동적할당 실패시 0반환
		return 0;

	rd = fopen("student.db", "rt");				 //파일 열기
	if (rd == NULL)								 //만약 파일을 여는데 실패했다면
	{
		printf("파일이 없거나 여는데 실패하였습니다.\n");
		return -1;
	}

	while (fgetc(rd) != '\n');					 //첫 줄(Num  Name.....)등 버림
	fgetc(rd);									 //다음줄 한칸 버림(Num에 배정된 5칸중 한칸)

	while(1)									 //파일 다 읽을때까지 무한반복
	{
		cleaninfo(&(temp->student));

		for (i = 0; i < 4; i++)					//Num에 배정된 칸중 나머지 4칸 버림
			fgetc(rd);

		if (nametolist(&rd, &temp) == 0)		//이름 읽기, 실패시 파일닫고 반환
		{
			fclose(rd);
			return 0;
		}
		idtolist(&rd, &temp);					//ID 읽기
		if (addresstolist(&rd, &temp) == 0)		//주소 읽기, 실패시 파일닫고 반환
		{
			fclose(rd);
			return 0;
		}
		if (phonenumbertolist(&rd, &temp) == 0)	//전화번호 읽기, 실패시 파일닫고 반환
		{
			fclose(rd);
			return 0;
		}

		inputlist(head, temp);							    //리스트에 넣기
		temp=NULL;											//다쓴temp에 NULL 넣어주기

		if (fgetc(rd) == EOF)                               //Num에 배정된칸중 첫번째칸 가져와서 파일이 끝났는지 검사(그래서 반복할떄 4칸씩만버림)
			break;

		temp = (list *)malloc(sizeof(list));			    //가져올 데이터가 더있으면 노드 추가생성
		if (!ckalloc(temp))									//동적할당검사, 실패시 파일닫고 반환
		{
			fclose(rd);
			return 0;
		}
	}
	fclose(rd);   //파일닫기
	return 1;     //성공시 1반환
}

int nametolist(FILE **rd, list **temp)  //list에 Name 넣기
{
	char buff[BUFF_LEN];										//문자열 임시저장용 배열

	fscanf(*rd, "%s", buff);                                    //배열에 문자열읽기
	(*temp)->student.fname = (char *)malloc(strlen(buff) + 1);  //크기만큼 동적할당
	if (!ckalloc((*temp)->student.fname))						//동적할당검사 실패시 temp반환후 0반환
	{
		free(*temp);
		return 0;
	}
	strcpy((*temp)->student.fname, buff);                       //문자열 저장

	fscanf(*rd, "%s", buff);									//배열에 문자열읽기
	(*temp)->student.lname = (char *)malloc(strlen(buff) + 1);	//동적할당
	if (!ckalloc((*temp)->student.lname))						//동적할당검사 실패시 name반환후 temp반환 그리고 0반환
	{
		freename(&((*temp)->student));
		free(*temp);
		return 0;
	}
	strcpy((*temp)->student.lname, buff);						//문자열 저장
	return 1;													//성공시 1반환
}

void idtolist(FILE **rd, list **temp)	//list에 ID 넣기
{
	char buff[BUFF_LEN];				  //문자열 임시저장용 배열

	fscanf(*rd, "%s", buff);
	(*temp)->student.id = atoi(buff);     //문자열을 정수로 변환해 저장
	fgetc(*rd); fgetc(*rd);               //ID와 주소사이 2칸 버림
}

int addresstolist(FILE **rd, list **temp) //list에 Address 넣기
{
	char buff[BUFF_LEN];						   //문자열 임시저장용 배열
 
	fscanf(*rd, "%s", buff);										//배열에 문자열읽기
	(*temp)->student.adr.state = (char *)malloc(strlen(buff) + 1);  //동적할당
	if (!ckalloc((*temp)->student.adr.state)	)					//동적할당확인, 실패시 배열의 동적할당받은멤버 반환 그리고 0반환
	{
		freename(&((*temp)->student));
		freeaddress(&((*temp)->student));
		free(*temp);
		return 0;
	}
	strcpy((*temp)->student.adr.state, buff);                       //문자열 복사, 저장 아래도 동일한 과정 반복

	fscanf(*rd, "%s", buff);										//배열에 문자열읽기
	(*temp)->student.adr.city = (char *)malloc(strlen(buff) + 1);   //동적할당
	if (!ckalloc((*temp)->student.adr.city))						//동적할당확인, 실패시 배열의 동적할당받은멤버 반환 그리고 0반환
	{
		freename(&((*temp)->student));
		freeaddress(&((*temp)->student));
		free(*temp);
		return 0;
	}
	strcpy((*temp)->student.adr.city, buff);                        //문자열 복사, 저장 아래도 동일한 과정 반복

	fscanf(*rd, "%s", buff);										//배열에 문자열읽기
	(*temp)->student.adr.village = (char *)malloc(strlen(buff) + 1);//동적할당
	if (!ckalloc((*temp)->student.adr.village))						//동적할당확인, 실패시 배열의 동적할당받은멤버 반환 그리고 0반환
	{
		freename(&((*temp)->student));
		freeaddress(&((*temp)->student));
		free(*temp);
		return 0;
	}
	if(strcmp(buff, "#")==0)				//만약 #이 들어가있다면 없는곳이라는뜻
	{
		free((*temp)->student.adr.village);
		(*temp)->student.adr.village=NULL;
	}
	else
		strcpy((*temp)->student.adr.village, buff);                 //문자열 복사, 저장 아래도 동일한 과정 반복

	fscanf(*rd, "%s", buff);										//배열에 문자열읽기
	(*temp)->student.adr.street = (char *)malloc(strlen(buff) + 1); //동적할당
	if (!ckalloc((*temp)->student.adr.street))						//동적할당확인, 실패시 배열의 동적할당받은멤버 반환 그리고 0반환
	{
		freename(&((*temp)->student));
		freeaddress(&((*temp)->student));
		free(*temp);
		return 0;
	}
	strcpy((*temp)->student.adr.street, buff);						//문자열 복사, 저장 아래도 동일한 과정 반복

	fscanf(*rd, "%s", buff);										//배열에 문자열읽기
	(*temp)->student.adr.home = (char *)malloc(strlen(buff) + 1);	//동적할당
	if (!ckalloc((*temp)->student.adr.home))						//동적할당확인, 실패시 배열의 동적할당받은멤버 반환 그리고 0반환
	{
		freename(&((*temp)->student));
		freeaddress(&((*temp)->student));
		free(*temp);
		return 0;
	}
	strcpy((*temp)->student.adr.home, buff);                       //문자열 복사, 저장 아래도 동일한 과정 반복
	
	return 1; //성공시 1반환
}

int phonenumbertolist(FILE **rd, list **temp) //list에 PhoneNunber 넣기
{
	char buff[BUFF_LEN];				      //문자열 임시저장용 배열

	fscanf(*rd, "%s", buff);
	(*temp)->student.pnum = (char *)malloc(strlen(buff) + 1);	//동적할당
	if (!ckalloc((*temp)->student.pnum))						//동적할당검사, 실패시 name과 address반환 후 temp반환 그리고 0반환
	{
		freename(&((*temp)->student));
		freeaddress(&((*temp)->student));
		free(*temp);
		return 0;
	}
	strcpy((*temp)->student.pnum, buff);	//문자열 저장
	fgetc(*rd);								//개행문자 버림
	return 1; //성공시 1반환
}

int filetoarray(info *ary, int *index)
{
	FILE *rd;									//읽을 파일포인터
	int i;										//반복제어변수
	*index = 0;								    //인덱스 기본값 0으로초기화

	rd = fopen("student.db", "rt");				//파일 열기
	if (rd == NULL)								//만약 파일을 여는데 실패했다면
	{
		printf("파일이 없거나 여는데 실패하였습니다.\n");
		return -1;
	}

	while (fgetc(rd) != '\n');					 //첫 줄(Num  Name.....)등 버림
	fgetc(rd);									 //다음줄 한칸 버림(Num에 배정된 5칸중 한칸)

	while (1)									 //파일 다 읽을때까지 무한반복
	{
		cleaninfo(&(ary[*index]));

		for (i = 0; i < 4; i++)					 //Num에 배정된 칸중 나머지 4칸 버림
			fgetc(rd);

		if (nametoarray(&rd, &ary, *index) == 0) //이름 읽기, 실패시 파일닫고 반환
		{
			fclose(rd);
			return 0;
		}
		idtoarray(&rd, &ary, *index);			 //ID 읽기
		if (addresstoarray(&rd, &ary, *index) == 0) //주소 읽기, 실패시 파일닫고 반환
		{
			fclose(rd);
			return 0;
		}
		if (pnumbertoarray(&rd, &ary, *index) == 0) //전화번호 읽기, 실패시 파일닫고 반환
		{
			fclose(rd);
			return 0;
		}

		(*index)++;

		if (fgetc(rd) == EOF)                       //Num에 배정된칸중 첫번째칸 가져와서 파일이 끝났는지 검사(그래서 반복할떄 4칸씩만버림)
			break;

		if (*index == ARY_LEN)  //배열이 꽉찼는데 파일이 끝나지 않았을때
		{
			printf("배열의 크기가 부족합니다.\n");
			printf("프로그램의 수정이 필요합니다\n");
			printf("200명의 정보만 입력되었습니다.\n");
			break;
		}
	}

	fclose(rd);   //파일닫기
	return 1;     //성공시 1반환
}

int nametoarray(FILE **rd, info **ary, int index) //배열에 Name 넣기
{
	char buff[BUFF_LEN];									 //문자열 임시저장용 배열

	fscanf(*rd, "%s", buff);                                 //배열에 문자열읽기
	(*ary)[index].fname = (char *)malloc(strlen(buff) + 1);  //동적할당
	if (!ckalloc((*ary)[index].fname))						 //동적할당확인, 실패시 배열의 동적할당받은멤버 반환 그리고 0반환
	{
		freearray(*ary, index + 1);
		return 0;
	}
	strcpy((*ary)[index].fname, buff);                       //문자열 복사, 저장 아래도 동일한 과정 반복

	fscanf(*rd, "%s", buff);
	(*ary)[index].lname = (char *)malloc(strlen(buff) + 1);
	if (!ckalloc((*ary)[index].lname))
	{
		freearray(*ary, index + 1);
		return 0;
	}
	strcpy((*ary)[index].lname, buff);
	return 1; //성공시 1반환
}

void idtoarray(FILE **rd, info **ary, int index) //배열에 ID 넣기
{
	char buff[BUFF_LEN];				  //문자열 임시저장용 배열

	fscanf(*rd, "%s", buff);
	(*ary)[index].id = atoi(buff);        //문자열을 정수로 변환해 저장
	fgetc(*rd); fgetc(*rd);               //ID와 주소사이 2칸 버림
}

int addresstoarray(FILE **rd, info **ary, int index) //배열에 Address 넣기
{
	char buff[BUFF_LEN];						   //문자열 임시저장용 배열
	int i = 0;									   //배열 index용 변수
 
	fscanf(*rd, "%s", buff);									 //배열에 문자열읽기
	(*ary)[index].adr.state = (char *)malloc(strlen(buff) + 1);  //동적할당
	if (!ckalloc((*ary)[index].adr.state))						 //동적할당확인, 실패시 배열의 동적할당받은멤버 반환 그리고 0반환
	{
		freearray(*ary, index + 1);
		return 0;
	}
	strcpy((*ary)[index].adr.state, buff);                      //문자열 복사, 저장 아래도 동일한 과정 반복

	fscanf(*rd, "%s", buff);									//배열에 문자열읽기
	(*ary)[index].adr.city = (char *)malloc(strlen(buff) + 1);  //동적할당
	if (!ckalloc((*ary)[index].adr.city))						//동적할당확인, 실패시 배열의 동적할당받은멤버 반환 그리고 0반환
	{
		freearray(*ary, index + 1);
		return 0;
	}
	strcpy((*ary)[index].adr.city, buff);                        //문자열 복사, 저장 아래도 동일한 과정 반복

	fscanf(*rd, "%s", buff);									 //배열에 문자열읽기
	(*ary)[index].adr.village = (char *)malloc(strlen(buff) + 1);//동적할당
	if (!ckalloc((*ary)[index].adr.village))					 //동적할당확인, 실패시 배열의 동적할당받은멤버 반환 그리고 0반환
	{
		freearray(*ary, index + 1);
		return 0;
	}
	if(strcmp(buff, "#")==0)				//만약 #이 들어가있다면 없는곳이라는뜻
	{
		free((*ary)[index].adr.village);
		(*ary)[index].adr.village=NULL;
	}
	else
		strcpy((*ary)[index].adr.village, buff);                //문자열 복사, 저장 아래도 동일한 과정 반복

	fscanf(*rd, "%s", buff);									//배열에 문자열읽기
	(*ary)[index].adr.street = (char *)malloc(strlen(buff) + 1);//동적할당
	if (!ckalloc((*ary)[index].adr.street))						//동적할당확인, 실패시 배열의 동적할당받은멤버 반환 그리고 0반환
	{
		freearray(*ary, index + 1);
		return 0;
	}
	strcpy((*ary)[index].adr.street, buff);                     //문자열 복사, 저장 아래도 동일한 과정 반복

	fscanf(*rd, "%s", buff);									//배열에 문자열읽기
	(*ary)[index].adr.home = (char *)malloc(strlen(buff) + 1);  //동적할당
	if (!ckalloc((*ary)[index].adr.home))						//동적할당확인, 실패시 배열의 동적할당받은멤버 반환 그리고 0반환
	{
		freearray(*ary, index + 1);
		return 0;
	}
	strcpy((*ary)[index].adr.home, buff);                       //문자열 복사, 저장 아래도 동일한 과정 반복
	
	return 1; //성공시 1반환
}

int pnumbertoarray(FILE **rd, info **ary, int index) //배열에 PhoneNunber 넣기
{
	char buff[BUFF_LEN];				//문자열 임시저장용 배열

	fscanf(*rd, "%s", buff);            //Phone_Number 저장하기
	(*ary)[index].pnum = (char *)malloc(strlen(buff) + 1); //동적할당
	if (!ckalloc((*ary)[index].pnum))	//동적할당확인, 실패시 배열의 동적할당받은멤버 반환 그리고 0반환
	{
		freearray(*ary, index + 1);
		return 0;
	}
	strcpy((*ary)[index].pnum, buff);	//문자열 저장
	fgetc(*rd);                         //개행문자 버림
	return 1; //성공시 1반환
}

void checklentoarray(info *ary, int index, int *namelen, int *adrlen)
{
	int i, n, a;

	*namelen=0;
	*adrlen=0;

	for(i=0;i<index;i++)	//모든 배열요소 검사하며 최대길이 구하기
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

	while(p!=*head)		//모든 list의 노드 검사해서 최대길이 구하기
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

void freelist(list *head)   //동적할당받은 리스트 모두 반환해줌
{
	list *q = head->llink;  //q는 첫번째노드
	list *p;			    //free를 위한 list포인터

	while (q != head)      //마지막노드까지 모두 실행
	{
		p = q;             //p가 q와 같은노드 가리키게함
		q = q->llink;      //q는 다음노드로 이동함

		freename(&(p->student));       //이름, 주소, 전화번호 반환 후 노드반환
		freeaddress(&(p->student));
		freephonenumber(&(p->student));
		free(p);
	}
}

void freearray(info *inf, int index) //배열 각 멤버 모두 반환
{
	int i;

	for (i = 0; i<index; i++)
	{
		freename(&(inf[i]));
		freeaddress(&(inf[i]));
		freephonenumber(&(inf[i]));
	}
}

void freename(info *p)  //name반환(해당멤버에 값이있을때만 if문 실행됨)
{
	if(p->fname)
		free(p->fname);
	if(p->lname)
		free(p->lname);
}

void freeaddress(info *p)  //주소 반환(해당멤버에 값이있을때만 if문 실행됨)
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

void freephonenumber(info *p)  //전화번호 반환(해당멤버에 값이있을때만 if문 실행됨)
{
	if(p->pnum)
		free(p->pnum);
}

void inputlist(list *head, list *newnode) //새로운노드를 list에 추가
{
	newnode->flink = head->flink;         //newnode의 각 링크에는 저장된값이 없으므로 먼저수정
	newnode->llink = head;
	head->flink->llink = newnode;
	head->flink = newnode;				  //위에서 head->flink의 llink값을 사용해야하므로 맨 마지막에 수정
}

list* findlistbyname(list *head, char *fname, char *lname)	//list에서 이름찾기(순차탐색)
{
	list *temp[ARY_LEN];	//동명이인을 위한 포인터배열(list의 flink나 llink를 통해 이으면 안된다)
	list *p;
	int index=0;
	int sel;

	p=head->llink;			//p는 첫번째 노드

	while(p!=head)			//모든 노드 탐색
	{
		if(strcmp(p->student.fname, fname)==0)
		{
			if(strcmp(p->student.lname, lname)==0)
			{
				temp[index]=p;	//동명이인을 찾으면 포인트 배열에 넣어주기
				index++;
			}
		}
		p=p->llink;
	}

	if(index==0)		//정보가 없다면 NULL 반환
		return NULL;
	else if(index==1)	//정보가 하나뿐이라면 그거 반환
		return temp[0];
	else
	{
		sel=chooseone(temp, index);	//정보가 여러개라면 그중하나 골라서 반환
		return temp[sel];
	}
}

int chooseone(list **temp, int len)	//동명이인중 한명 고르기
{
	int i;
	int sel;

	printf("동명이인이 있습니다.\n");
	printf("이중 한명을 선택해 주십시오(숫자 입력).\n\n");
	printf("-------< 목 록 >-------\n");
	for(i=0; i<len; i++)
	{
		printf(" %d. ", i+1);
		prtline((temp[i])->student);
	}
	printf("-----------------------\n\n");
	while (1)
	{
		printf("  선택 : ");
		scanf("%d", &sel);
		cleanstdin();
		if (sel >= 1 && sel <= len)
			return sel-1;				//sel-1:해당 학생의 index
		else
		{
			printf("잘못된 선택입니다.\n");
			printf("다시 입력해 주십시오.\n");
		}
	}
}

list* findlistbyid(list *head, int id)
{
	list *p;

	p=head->llink;	//p는 첫번째노드

	while(p!=head)	//모든노드 탐색
	{
		if(p->student.id==id)
			break;

		p=p->llink;
	}
	if(p==head)		//정보가 없으면 NULL반환
		return NULL;
	else
		return p;
}

void deletelist(list *p)	//list에서 정보하나 지우기
{
	(p->llink)->flink=p->flink;
	(p->flink)->llink=p->llink;

	free(p);
}

int filetohash(list **hash, int sel)
{
	FILE *rd;									 //읽을 파일포인터
	list *temp;									 //임시노드
	int i;										 //반복제어변수

	temp = (list *)malloc(sizeof(list));		 //첫 노드를위한 동적할당
	if (!ckalloc(temp))							 //동적할당 실패시 0반환
		return 0;

	rd = fopen("student.db", "rt");				 //파일 열기
	if (rd == NULL)								 //만약 파일을 여는데 실패했다면
	{
		printf("파일이 없거나 여는데 실패하였습니다.\n");
		return -1;
	}

	while (fgetc(rd) != '\n');					 //첫 줄(Num  Name.....)등 버림
	fgetc(rd);									 //다음줄 한칸 버림(Num에 배정된 5칸중 한칸)

	while (1)									 //파일 다 읽을때까지 무한반복
	{
		cleaninfo(&(temp->student));

		for (i = 0; i < 4; i++)					//Num에 배정된 칸중 나머지 4칸 버림
			fgetc(rd);

		if (nametolist(&rd, &temp) == 0)		//이름 읽기, 실패시 파일닫고 반환
		{
			fclose(rd);
			return 0;
		}
		idtolist(&rd, &temp);					//ID 읽기
		if (addresstolist(&rd, &temp) == 0)		//주소 읽기, 실패시 파일닫고 반환
		{
			fclose(rd);
			return 0;
		}
		if (phonenumbertolist(&rd, &temp) == 0)	//전화번호 읽기, 실패시 파일닫고 반환
		{
			fclose(rd);
			return 0;
		}

		inputhash(hash, temp, sel);				//Temp Hash에 넣기
		temp = NULL;							//다쓴temp에 NULL 넣어주기

		if (fgetc(rd) == EOF)                   //Num에 배정된칸중 첫번째칸 가져와서 파일이 끝났는지 검사(그래서 반복할떄 4칸씩만버림)
			break;

		temp = (list *)malloc(sizeof(list));	//가져올 데이터가 더있으면 노드 추가생성
		if (!ckalloc(temp))						//동적할당검사, 실패시 파일닫고 반환
		{
			fclose(rd);
			return 0;
		}
	}
	fclose(rd);   //파일닫기
	return 1;     //성공시 1반환
}

void clearhashtable(list **hash, int len)
{
	int i;

	for (i = 0; i < len; i++)
	{
		hash[i] = NULL;	//모두 NULL로 초기화해놓음
	}
}

int hashfunctionbyname(char *name)
{
	return (name[0] & 0xFF) % 100;	//한글중 첫번째바이트:FFFFFFB0~FFFFFFC8(&0xFF를 하면 176~200)
}

int hashfunctionbyid(int id)
{
	return id%HASH_ID_LEN;	//ID를 배열길이만큼 나눈 나머지
}

void inputhash(list **hash, list *data, int sel)
{
	int index;

	if (sel == 1)	//1:이름, 2:ID
		index = hashfunctionbyname(data->student.fname);
	else if (sel == 2)
		index = hashfunctionbyid(data->student.id);

	data->llink = hash[index];	//중복되는경우 연결해줌(나중에 들어온걸 맨앞에 넣음)
	hash[index] = data;
}

list* hashingbyname(list **hash, char *fname, char *lname)
{
	int index;
	list *head;	//동명이인 연결해줄 포인터
	list *q;	//연결을 도와주는 포인터
	list *p;	//hash[index]의 값은 변하면 안되므로 이걸씀
	list *rt;	//head의 정보를 정렬해서 반환할때 쓰는 포인터
	rt = NULL;
	head = NULL;

	index = hashfunctionbyname(fname);	//먼저 인덱스 구하기
	p = hash[index];					//hash[index]를 p로하기

	while (p != NULL)	//중복된 정보가 있을경우 연결되있으므로 그중에서 탐색
	{
		if (strcmp(p->student.fname, fname) == 0)
		{
			if (strcmp(p->student.lname, lname) == 0)	//이름의 경우 동명이인이 있으므로 연결
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
		if(head->flink==NULL)	//정보가 있는데 한명일경우 그대로 반환
			return head;
		else
		{
			minisort(head, &rt);	//여러명 있으면 학번순 정렬후 반환
			return rt;
		}
	}
	return head;	//그 외에는 그냥 head반환(NULL일경우는 NULL반환됨)
}

void minisort(list *head, list **rt)	//list 선택정렬
{
	list *big, *b, *p;
	while(head!=NULL)
	{
		big=head;	//가장 큰거 선택
		b=big;		//b는 큰거 선택된것의 이전꺼
		p=head->flink;	//선택한 다음 정보부터 비교 시작
		while(p!=NULL)
		{
			if(big->student.id<p->student.id)	//big보다 큰 정보가 나타난경우
			{
				b=big;		//b는 새로운 큰정보 바로앞꺼 가리킴
				big=p;		//새로운게 big이됨
			}
			p=p->flink;
		}
		if(big!=b)			//big과 b가 다를경우 big의 전 노드는 big의 다음노드를 가리킴
		{
			b->flink=big->flink;
		}
		else
		{
			head=big->flink;	//처음 선택한게 가장큰경우 head가 그 다음껄 가리킴
		}
		big->flink=(*rt);		//가장큰걸 head에서 떼어내서 rt에 이어줌
		(*rt)=big;
	}
}

list* hashingbyid(list **hash, int id)
{
	int index;
	list *p;	//hash[index]의 값은 변하면 안됨

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

void freehash(list **hash, int len)	//hash table 동적할당 반환하기
{
	int i;
	list *p;

	for (i = 0; i < len; i++)
	{
		while (hash[i] != NULL)
		{
			p = hash[i];
			hash[i] = hash[i]->llink;
			freename(&(p->student));       //이름, 주소, 전화번호 반환 후 노드반환
			freeaddress(&(p->student));
			freephonenumber(&(p->student));
			free(p);
		}
		hash[i] = NULL;
	}
}