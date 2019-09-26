/*
	컴퓨터공학과 2013150016 서민성

	<<movies.txt>>
	전체 데이터의 수: 27279개
	유효 데이터의 수: 27258개
	오류 데이터의 수: 21개 - 년도 정보가 없는 것

	<<Program>>
	movies.txt에 있는 약 3만개정도의 데이터를 사용해
	다양한 정렬 알고리즘으로 정렬을 수행하고
	그 수행시간으로 비교 분석하는 프로그램
	movies.txt는 수정 없이 e-class에 있는 그대로 사용

	프로그램 수행시간: 약 30~40분

	2학년 자료구조과목에서 제출한 과제를
	이번 과제의 내용에 맞게 수정해서 사용
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//배열의 크기와 유효 데이터의 수
#define ARRAY_SIZE 30000	//배열의 크기
#define DATA_NUM 27258		//유효 데이터의 갯수: 27258개

//테스트할 정렬 알고리즘의 갯수
#define SORT_NUM 7

//정렬 기준 설정 값
#define NAME_SORT 1	//제목으로 정렬
#define YEAR_SORT 2	//년도로 정렬

//원소간 비교에 사용
#define LEFT_BIG 1	//왼쪽이 크다
#define RIGHT_BIG 2	//오른쪽이 크다
#define EQUAL 3		//같다

//제목으로 기수정렬을 수행하기 위해 설정
#define CHAR_MAX 255 //문자 하나가 가질 수 있는 최대 값

//영화 정보를 담을 구조체
struct movies_ {
	char *name;	//제목
	int year;	//년도
	char *genr;	//장르

	int name_len;	//제목의 길이
};
typedef struct movies_ Movies;


enum{SELECT_TIME=0, INSERT_TIME, BUBBLE_TIME, 
	 MERGE_TIME, QUICK_TIME, HEAP_TIME, RADIX_TIME};

//파일 읽기 테스트용 변수
int number_of_data=0;	//전체 행의 갯수(데이터의 갯수)
int invalid_data=0;	//잘못된 데이터의 갯수

////////////////////////////////////////////////
//               함수 선언                    //
////////////////////////////////////////////////

void error_handling(char*);			//오류처리-프로그램 강제종료

void swapary(Movies *, int, int);	//배열의 두 요소 교환
void copyary(Movies *, Movies *, int);	//배열 복사하기

int compare(Movies, Movies, int);	//비교하기, 여기서 아래 두 함수 호출
int compare_name(Movies, Movies);	//이름으로 비교
int compare_year(Movies, Movies);	//년도로 비교

int* getRandomIndex(int);	//랜덤 인덱스 얻기

void printTimeOfSorts(Movies *, int);	//시간 측정
void timeofsort(Movies*, int, int);		//각 데이터 수 당 시간측정
void timeofsort_all(Movies*, int);		//모든 데이터에 대한 정렬 시간 측정
double checktime(void(*sort)(Movies *, int, int, int), Movies *, int, int);	//시간측정
void printtime(double *);			//시간 출력

int readFile(Movies *);					//파일 한줄씩 읽기
int disassemble(Movies *, char *, int);	// 한줄마다 구조체로 분해해 넣기
void disassemble_test(Movies *, int);	//구조체에 잘 들어갔나 테스트
void simple_test(Movies *, int);		//정렬 테스트, 위 함수로 먼저 하고 이걸로 두번째 확인

void indexsort(int *, int);	//랜덤 추출한 인덱스를 정렬하기 위함, 삽입 정렬 사용

/* 정렬 알고리즘: 함수 포인터 사용을 위해 매개변수 갯수 통일 */
/* 선택, 삽입, 버블, 합병, 퀵 정렬의 출처: 2학년 자료구조 과제 */
void selectsort(Movies *, int, int, int);	//선택 정렬
void insertsort(Movies *, int, int, int);	//삽입 정렬
void bubblesort(Movies *, int, int, int);	//버블 정렬

void mergesort(Movies *, int, int, int);	//합병 정렬
void merge(Movies *, int, int, int, int);

void quicksort(Movies *, int, int, int);	//퀵 정렬
int partition(Movies *, int, int, int);

//힙 정렬(https://www.programiz.com/dsa/heap-sort)
void heapSort(Movies arr[], int, int, int);
void heapify(Movies arr[], int, int, int);

//기수-계수 정렬(https://www.geeksforgeeks.org/radix-sort/)
void radixsort(Movies arr[], int, int, int);
void radixsort_year(Movies arr[], Movies output[], int count[], int, int);
void radixsort_name(Movies arr[], Movies output[], int count[], int, int);
void countSort(Movies arr[], Movies output[], int count[], int, int, int, int);
int getMax(Movies arr[], int);
int getIdx(Movies, int, int);

////////////////////////////////////////
//            함수 정의               //
////////////////////////////////////////

//Utilities
/*
str: Error: 이후 출력할 오류메시지
*/
void error_handling(char *str) {
	printf("Error: %s", str);
	system("cls");
	exit(1);
}

/*정렬하는데 걸린 시간 데이터 수 별 출력
ary: 정렬할 배열
index: 데이터의 갯수
*/
void printTimeOfSorts(Movies *ary, int index) {
	//1. 100개의 데이터 정렬에 대해
	printf("==< 데이터 %d개 사용 >================\n", 100);
	timeofsort(ary, index, 100);

	//2. 1000개의 데이터 정렬에 대해
	printf("==< 데이터 %d개 사용 >===============\n", 1000);
	timeofsort(ary, index, 1000);
	
	//3. 10000개의 데이터 정렬에 대해
	printf("==< 데이터 %d개 사용 >==============\n", 10000);
	timeofsort(ary, index, 10000);
	
	//4. 전체 데이터 정렬에 대해
	printf("==< 데이터 %d개 사용 >==============\n", index);
	timeofsort_all(ary, index);
}

/*정렬 수행 후 평균시간 구해 결과 출력
ary: 원본 배열
index: 데이터의 갯수
num: 배열 중 정렬할 데이터의 수
*/
void timeofsort(Movies* ary, int index, int num)
{
	//원본 배열로 부터 저장할 배열을 만든다
	Movies *sort;
	//원본 배열에서 가져올 랜덤 원소들의 인덱스
	int *idx;

	double sorttime[SORT_NUM];	//걸린시간 저장할 배열
	int i, j;					//반복용변수

	sort = (Movies*)malloc(sizeof(Movies)*num);
	if (sort == NULL)
		error_handling("malloc() error - timeofsort");

	for (i = 0; i<SORT_NUM; i++)//먼저 걸린시간은 모두 0으로 초기화
		sorttime[i] = 0;

	//100번 정렬 후 평균시간 구함
	//매 정렬마다 새로운 num개의 랜덤 원소로 정렬 수행
	for (i = 0; i<100; i++)		//100번 반복
	{
		//랜덤 원소들의 인덱스 가져오기
		idx = getRandomIndex(num);

		//원본 배열과 랜덤 인덱스로 정렬할 대상 배열 만들기
		for (j = 0; j < num; j++) {
			sort[j] = ary[idx[j]];
		}

		sorttime[SELECT_TIME] += checktime(selectsort, sort, num, 0);	//선택정렬
		sorttime[INSERT_TIME] += checktime(insertsort, sort, num, 0);	//삽입정렬
		sorttime[BUBBLE_TIME] += checktime(bubblesort, sort, num, 0);	//버블정렬
		sorttime[MERGE_TIME] += checktime(mergesort, sort, 0, num - 1);	//합병정렬
		sorttime[QUICK_TIME] += checktime(quicksort, sort, 0, num - 1);	//퀵  정렬
		sorttime[HEAP_TIME] += checktime(heapSort, sort, num, 0);		//힙  정렬
		sorttime[RADIX_TIME] += checktime(radixsort, sort, num, 0);		//기수정렬

		free(idx);
	}

	for (i = 0; i<SORT_NUM; i++)		//100번반복했으니 걸린시간의 평균을 구해줌
		sorttime[i] = sorttime[i] / 100;

	printtime(sorttime);				//걸린시간을 출력해주는 함수 호출
	free(sort);
}

/*모든 유효데이터에대해 정렬 수행 후 평균시간 구해 결과 출력
ary: 배열
index: 데이터의 갯수
*/
void timeofsort_all(Movies* ary, int index) {
	
	double sorttime[SORT_NUM];	//걸린시간 저장할 배열
	int i;						//반복용변수

	for (i = 0; i<SORT_NUM; i++)//먼저 걸린시간은 모두 0으로 초기화
		sorttime[i] = 0;

	//100번 정렬 후 평균시간 구함
	//매 정렬마다 새로운 num개의 랜덤 원소로 정렬 수행
	for (i = 0; i<100; i++)		//100번 반복
	{
		sorttime[SELECT_TIME] += checktime(selectsort, ary, index, 0);	//선택정렬
		sorttime[INSERT_TIME] += checktime(insertsort, ary, index, 0);	//삽입정렬
		sorttime[BUBBLE_TIME] += checktime(bubblesort, ary, index, 0);	//버블정렬
		sorttime[MERGE_TIME] += checktime(mergesort, ary, 0, index - 1);//합병정렬
		sorttime[QUICK_TIME] += checktime(quicksort, ary, 0, index - 1);//퀵  정렬
		sorttime[HEAP_TIME] += checktime(heapSort, ary, index, 0);		//힙  정렬
		sorttime[RADIX_TIME] += checktime(radixsort, ary, index, 0);	//기수정렬
	}

	for (i = 0; i<SORT_NUM; i++)		//100번반복했으니 걸린시간의 평균을 구해줌
		sorttime[i] = sorttime[i] / 100;

	printtime(sorttime);				//걸린시간을 출력해주는 함수 호출
}

/*정렬 한번 하는데 걸리는 시간 측정
sort: 수행할 정렬의 함수포인터
ary, arg1, arg2, arg3: 위 함수에 사용되는 매개변수들
*/
double checktime(void (*sort)(Movies *, int, int, int), Movies *ary, int arg1, int arg2)
{
	clock_t start, finsh;	//시간측정하기위한 변수(시작, 끝)
	Movies *temp;//ary가 정렬되면 안됨, 정렬할 배열은 temp
	int num;

	//num = 정렬할 데이터의 갯수
	if (arg1) num = arg1;
	else num = arg2 + 1;

	temp = (Movies*)malloc(sizeof(Movies)*num);
	if (temp == NULL)
		error_handling("malloc error - checktime");

	copyary(ary, temp, num);		//temp는 ary랑 같은값 가지고있음

	start=clock();						//정렬전 시간
	sort(temp, arg1, arg2, NAME_SORT);	//정렬하기(이름 정렬 이후 년도 정렬)
	sort(temp, arg1, arg2, YEAR_SORT);
	finsh=clock();						//정렬후 시간

	free(temp);

	return (double)(finsh-start)/CLOCKS_PER_SEC;	 //걸린시간=정렬후시간-정렬전시간
}

/* 걸린 시간 평균 출력 */
void printtime(double *sorttime)	//걸린시간 평균 화면에 출력
{
	printf("  1. 선택정렬 : %f\n", sorttime[SELECT_TIME]);
	printf("  2. 삽입정렬 : %f\n", sorttime[INSERT_TIME]);
	printf("  3. 버블정렬 : %f\n", sorttime[BUBBLE_TIME]);
	printf("  4. 합병정렬 : %f\n", sorttime[MERGE_TIME]);
	printf("  5. 퀵  정렬 : %f\n", sorttime[QUICK_TIME]);
	printf("  6. 힙  정렬 : %f\n", sorttime[HEAP_TIME]);
	printf("  7. 기수정렬 : %f\n", sorttime[RADIX_TIME]);
	printf("=======================================\n\n");
}

/*랜덤한 인덱스를 얻는 함수
n: 얻을 인덱스의 수
*/
int* getRandomIndex(int n) {
	int i, j, k, ck;
	int *idx;

	idx = (int*)malloc(sizeof(int)*n);
	if (idx == NULL)
		error_handling("malloc() error - getRandomIndex");

	srand(time(NULL));

	//값의 중복 검사를 하며 100개의 랜덤 인덱스 추출
	for (i = 0; i < n; i++) {
		k = rand() % DATA_NUM;

		while (1) {
			ck = 0;

			for (j = 0; j < i; j++)
				if (idx[j] == k)
					ck = 1;

			if (ck == 0)
				break;
			else
				k = rand() % DATA_NUM;
		}

		idx[i] = k;
	}

	//위에서 추출한 인덱스 정렬
	indexsort(idx, n);

	return idx;
}

/*
ary: 대상 배열
i, j: 서로 위치를 바꿀 배열의 인덱스
*/
void swapary(Movies *ary, int i, int j)	//배열의 두 요소 위치 바꾸기
{
	Movies temp = ary[i];	//temp에 ary[i]값 넣기, ary[i]는 빈공간
	ary[i] = ary[j];	//빈공간에 ary[j]넣기, ary[j]는 빈공간
	ary[j] = temp;		//빈공간에 temp값(ary[i]의 값)넣기
}

/*
ary: 원본 배열
temp: 복사된 배열
*/
void copyary(Movies *ary, Movies *temp, int num)
{
	int i;				//반복제어변수

	for(i=0;i<num;i++)	//temp에 ary배열 값 복사
		temp[i]=ary[i];
}

/*
left: 비교할 왼쪽 원소
right: 비교할 오른쪽 원소

LEFT_BIG, RIGHT_BIG, EQUAL 반환
*/
int compare(Movies left, Movies right, int flag) {
	int val;
	if (flag == NAME_SORT) val = compare_name(left, right);
	else if (flag == YEAR_SORT) val = compare_year(left, right);
	else error_handling("Invalid Use - compare(Movies, Movies, flag)");

	return val;
}

/*
left: 비교할 왼쪽 원소
right: 비교할 오른쪽 원소
*/
int compare_name(Movies left, Movies right) {
	int val;

	val = strcmp(left.name, right.name);

	if (val > 0) return LEFT_BIG;
	else if (val < 0) return RIGHT_BIG;
	else return EQUAL;
}

/*
left: 비교할 왼쪽 원소
right: 비교할 오른쪽 원소
*/
int compare_year(Movies left, Movies right) {
	if (left.year > right.year) return LEFT_BIG;
	else if (left.year < right.year) return RIGHT_BIG;
	else return EQUAL;
}

/*
movies: 읽은 파일의 내용을 저장할 구조체
*/
int readFile(Movies *movies) {

	FILE *fp;
	char buffer[300];
	int count = 0;
	int ck = 0;

	fp = fopen("movies.txt", "r");

	if (fp == NULL) error_handling("fopen()");

	//첫 줄은 버린다(movieId,title,genres 부분)
	fgets(buffer, 300, fp);

	while (!feof(fp)) {
		fgets(buffer, 300, fp);

		ck = disassemble(movies, buffer, count);

		//disassemble이 정상 수행 되었을 때만 count 증가
		if (ck == 0)
			count++;

		//전체 데이터의 갯수 확인용 카운트 증가
		number_of_data++;

		/*
		//파일 형식이 제대로 통일 되어 있지 않아서
		//오류가 자주 발생한다
		//오류의 원인을 파악하기 위한 줄
		if (count > 0 && count % 1000 == 0) {
		printf("%d\n", count);
		printf("%s\n", buffer);
		system("pause");
		}
		*/
	}

	fclose(fp);

	return count;
}

/*
movies: 영화정보 구조체
string: 파일에서 읽어온 한 행, 이것을 분해해서 구조체에 넣는다
index: 현재 저장해야할 구조체의 인덱스
*/
int disassemble(Movies *movies, char *string, int index) {
	int str_idx = 0;	//string에 대한 인덱스
	int flag = 0;		//영화 이름에 대한 분류(큰 따옴표 여부)
	int name_start, name_size;	//영화 이름 시작과 끝에 대한 인덱스
	int year_start;				//연도는 길이가 항상 4이므로 시작만 필요
	int genr_start, genr_size;	//장르의 시작과 끝에 대한 인덱스

	//연도가 없는 경우를 대비한 카운트
	//어느정도 반복했는데 연도를 못찾으면 그 영화 정보는 무시한다
	int count = 0;

	//string에 대한 서브 인덱스
	//파일 형식이 제대로 통일되어 있지않음
	//(년도)다음 바로 반점(,)이 오는 경우가 많고
	//(년도)다음 한칸 띄고 반점(,)이 오는 경우가 있음
	int sub_idx = 0;

	//처음 숫자와 반점까지 읽기 -> 필요없는 정보
	while (string[str_idx] != ',') str_idx++;
	str_idx++;

	//영화 이름이 큰 따옴표부터 시작한다면
	if (string[str_idx] == '"') {
		flag = 1;	//flag 설정(큰 따옴표의 경우 특수 처리)
		str_idx++;	//이 경우 다음 글자가 이름의 시작이므로 인덱스 증가
	}

	//이름의 시작 인덱스 설정
	name_start = str_idx;

	//타이틀에 큰 따옴표가 여러개 들어가있는게 있음
	if (flag == 1) while (string[str_idx] != '"' || string[str_idx + 1] != ',') str_idx++;
	else while (string[str_idx] != ',') str_idx++;

	//연도의 시작을 찾기위한 뒤로가기 탐색
	sub_idx = str_idx;
	while (string[sub_idx] != '(') {
		count++;
		sub_idx--;

		//8번 이상 뒤로가도 못찾으면 년도가 없는 정보인것
		//이경우 나간다
		if (count > 8) {
			invalid_data++;	//파일 읽기 테스트용 전역변수
			return -1;
		}
	}


	//위의 결과에 하나 증가시키면 연도의 시작부분을 가리키게된다
	sub_idx++;

	year_start = sub_idx;						//연도의 시작 인덱스 설정
	name_size = year_start - 2 - name_start;	//이름의 길이 설정

												//str_idx는 현재 " 또는 ,을 가리키고있다
												//아래 연산후 str_idx는 장르의 첫 글자를 가리킨다
	str_idx++;
	str_idx += flag;

	genr_start = str_idx;						//장르의 시작 인덱스 설정

	while (string[str_idx] != '\n') str_idx++;	//개행문자까지 읽기

	genr_size = str_idx - genr_start;			//장르의 길이 설정

	//구조체의 맴버변수에 동적할당 받기
	movies[index].name = (char *)malloc(sizeof(char)*(name_size + 1));
	movies[index].genr = (char *)malloc(sizeof(char)*(genr_size + 1));

	//동적할당받은 멤버변수 초기화
	memset(movies[index].name, 0, name_size + 1);
	memset(movies[index].genr, 0, genr_size + 1);

	//이름, 장르정보 복사
	memcpy(movies[index].name, &(string[name_start]), name_size);
	memcpy(movies[index].genr, &(string[genr_start]), genr_size);

	//구조체에 연도정보 넣기
	movies[index].year = atoi(&(string[year_start]));

	//구조체에 제목의 길이 넣기
	movies[index].name_len = strlen(movies[index].name);

	//성공시 0 반환
	return 0;
}

/*
movies: 영화정보 구조체
index: 데이터 갯수
*/
void disassemble_test(Movies *movies, int index) {

	int i, j;
	int *idx;

	idx = getRandomIndex(100);

	printf("%-110s %-6s %s\n", "Title", "Years", "Genre");

	//100개의 랜덤 인덱스에 대한 구조체에 저장된 정보 출력
	for (i = 0; i < 100; i++) {
		j = idx[i];
		printf("%-110s %-6d %s\n", movies[j].name, movies[j].year, movies[j].genr);
	}

	printf("\n");
	printf("Number of data: %d\n", number_of_data);
	printf("Invalid data: %d\n", invalid_data);
	printf("Valid data: %d\n\n", index);

	free(idx);
}

/*
movies: 영화정보 구조체
index: 데이터 갯수
*/
void simple_test(Movies *movies, int index) {
	int i;

	for (i = 0; i < 100; i++) {
		printf("%-110s %-6d %s\n", movies[i].name, movies[i].year, movies[i].genr);
	}

	printf("=============================\n");
	printf("=============================\n");

	for (i = index - 100; i < index; i++) {
		printf("%-110s %-6d %s\n", movies[i].name, movies[i].year, movies[i].genr);
	}
}

/*
ary: 인덱스 배열
n: 인덱스 배열의 원소 갯수
*/
void indexsort(int *ary, int n) {
	int sel;		//비교진행할값
	int i, j;		//반복제어변수

	for (i = 0; i < n - 1; i++)	//총 n-1번 반복
	{
		sel = ary[i + 1];		//i 다음요소를 뽑음
		j = i;					//j는 i랑 같음
		while ((j >= 0) && (ary[j] > sel))	//i요소부터 0번째요소까지 비교
		{
			ary[j + 1] = ary[j];		//sel로 뽑은 자리에 그 전 요소의 값을 넣음
			j--;						//j하나 감소
		}
		ary[j + 1] = sel;		//반복문이 종료됬다면 그 다음자리가 빈공간이됨 거기에 sel을 넣음
	}
}

//Sort Functions
/* 선택 정렬 (배열, 데이터 갯수, 0, NAME_SORT or YEAR_SORT)
ary: 배열
n: 데이터의 갯수
x: 0 (안쓰는 값)
flag: 이름 또는 연도로 정렬
*/
void selectsort(Movies *ary, int n, int x, int flag)	//선택 정렬
{
	int least;			//최소값
	int i, j;			//반복제어변수

	for (i = 0; i < n - 1; i++)		//첫번째 요소부터 시작
	{
		least = i;					//i 요소가 최소값이라고 가정
		for (j = i + 1; j < n; j++)	//i 다음 요소부터 끝까지 i 요소와 비교
		{
			if (compare(ary[j], ary[least], flag)==RIGHT_BIG)	//i요소가 최소값이 아니었다면
				least = j;				//최소값은 j가됨
		}								//반복해서 최종 최소값찾음
		if (least != i)					//가정한대로 i가 최소값이면 데이터 교환은 없음
			swapary(ary, i, least);
	}
}

/*삽입 정렬 (배열, 데이터 갯수, 0, NAME_SORT or YEAR_SORT)
ary: 정렬할 배열
n: 배열의 크기(데이터 갯수)
x: 0 (안쓰는 값)
flag: 이름 또는 연도로 정렬
*/
void insertsort(Movies *ary, int n, int x, int flag)	//삽입 정렬
{
	Movies sel;		//비교진행할값
	int i, j;			//반복제어변수

	for (i = 0; i < n - 1; i++)	//총 n-1번 반복
	{
		sel = ary[i+1];		//i 다음요소를 뽑음
		j = i;					//j는 i랑 같음
		while (j >= 0 && (compare(ary[j], sel, flag)==LEFT_BIG))	//i요소부터 0번째요소까지 비교
		{
			ary[j + 1] = ary[j];		//sel로 뽑은 자리에 그 전 요소의 값을 넣음
			j--;						//j하나 감소
		}
		ary[j + 1] = sel;		//반복문이 종료됬다면 그 다음자리가 빈공간이됨 거기에 sel을 넣음
	}
}

/*버블 정렬 (배열, 데이터 갯수, 0, NAME_SORT or YEAR_SORT)
ary: 배열
n: 데이터의 갯수
x: 0 (안쓰는 값)
flag: 이름 또는 연도로 정렬
*/
void bubblesort(Movies *ary, int n, int x, int flag)	//버블 정렬
{
	int i, j;			//반복제어변수

	for (i = 0; i < n - 1; i++)			//반복횟수 지정을 위한 반복문(n-1번 반복)
	{
		for (j = 0; j < n - 1 - i; j++) //처음엔 n-1번, 두번째 n-2번...마지막 1번 반복하기위해 n-1-i(i는 부터 n-2까지)
		{
			if (compare(ary[j], ary[j+1], flag)==LEFT_BIG)	//j요소가 그다음요소보다 크다면 서로위치교환
				swapary(ary, j, j + 1);
		}
	}
}

/*합병 정렬 (배열, 0, 데이터 갯수-1, NAME_SORT or YEAR_SORT)
ary: 정렬할 배열
left: 0 (정렬할 배열의 가장 왼쪽 인덱스)
right: 데이터 갯수 - 1 (정렬할 배열의 가장 오른쪽 인덱스)
flag: 이름 혹은 년도로 정렬 선택
*/
void mergesort(Movies *ary, int left, int right, int flag)	//합병 정렬
{
	int mid;	//mid값

	if(left<right)	//가장 왼쪽인덱스가 가장 오른쪽인덱스보다 작을때만 정렬
	{
		mid = (left + right) / 2;		//중간위치 구하기
		mergesort(ary, left, mid, flag);		//가장 왼쪽부터 중간까지를 또 반으로 나눔
		mergesort(ary, mid + 1, right, flag);//중간 다음부터 가장 오른쪽까지를 또 반으로나눔
		merge(ary, left, mid, right, flag);	//다 나눴으면 이제 정렬시작
	}
}
/*합병
ary: 배열
left: 왼쪽 인덱스
mid: 가운데 인덱스
right: 오른쪽 인덱스
flag: 이름 혹은 년도로 정렬 선택
*/
void merge(Movies *ary, int left, int mid, int right, int flag)	//합병
{
	int i = left;		//i는 왼쪽부분 첫 인덱스
	int j = mid + 1;	//j는 오른쪽부분 첫 인덱스
	int index = left;	//아래 임시배열 temp에 대한 인덱스
	Movies temp[ARRAY_SIZE];	//합병할때 쓸 임시배열

	while (i <= mid && j <= right)	//왼쪽인덱스나 오른쪽인덱스가 끝까지 갈때까지 반복
	{
		if(compare(ary[i], ary[j], flag)!=LEFT_BIG)
			temp[index++] = ary[i++];
		else
			temp[index++] = ary[j++];
	}
	if (i > mid)					//위 반복문 끝났는데 오른쪽이 남았다면
		while (j <= right)			//남은 오른쪽 값들 모두 옮김
			temp[index++] = ary[j++];
	else							//왼쪽이 남았다면
		while (i <= mid)			//왼쪽 값들 모두 옮김
			temp[index++] = ary[i++];

	for (i = left; i <= right; i++)	//변수 i 재활용, 임시배열에 넣은값을 배열에 넣기
		ary[i] = temp[i];
}

/*퀵 정렬 (배열, 0, 데이터 갯수-1, NAME_SORT or YEAR_SORT)
ary: 배열
left: 0 (정렬할 배열의 가장 왼쪽 인덱스)
right: 데이터 갯수 -1 (정렬할 배열의 가장 오른쪽 인덱스)
flag: 이름 혹은 년도로 정렬 선택

퀵정렬 코드는 교재의 의사코드를 따름
-> 원래 가지고있던 코드보다 코드 길이도 짧고 효율적으로 보여서 바꿈
*/
void quicksort(Movies *ary, int left, int right, int flag)
{
	int p; //Pivot
	if (left < right) {
		p = partition(ary, left, right, flag);
		quicksort(ary, left, p - 1, flag);
		quicksort(ary, p + 1, right, flag);
	}
}
/*피벗 찾기
ary: 배열
left: 가장 왼쪽 인덱스
right: 가장 오른쪽 인덱스
flag: 제목 또는 년도로 정렬할지 여부
*/
int partition(Movies *ary, int left, int right, int flag) {
	Movies x = ary[right];
	int i = left - 1;
	int j;
	for (j = left; j < right; j++) {
		if (compare(ary[j], x, flag) != LEFT_BIG) {
			i++;
			swapary(ary, i, j);
		}
	}
	swapary(ary, i + 1, right);

	return i + 1;
}

/* Heap Sort (배열, 데이터 갯수, 0, NAME_SORT or YEAR_SORT)
arr: 배열
n: 데이터의 갯수
x: 0 (안쓰는 값)
flag: 이름 혹은 년도로 정렬 선택
*/
void heapSort(Movies arr[], int n, int x, int flag)
{
	int i;
	Movies temp;
	// Build max heap
	for (i = n / 2 - 1; i >= 0; i--)
		heapify(arr, n, i, flag);

	// Heap sort
	for (i = n - 1; i >= 0; i--)
	{
		swapary(arr, 0, i);
		// Heapify root element to get highest element at root again
		heapify(arr, i, 0, flag);
	}
}
/* Build max heap
arr: 배열
n: 힙의 크기(데이터의 갯수)
i: max heap 기준 위치(서브 트리의 루트)
flag: 이름 혹은 년도로 정렬 선택
*/
void heapify(Movies arr[], int n, int i, int flag)
{
	// Find largest among root, left child and right child
	int largest = i;
	int l = 2 * i + 1;
	int r = 2 * i + 2;
	Movies temp;

	if ((l < n) && (compare(arr[l], arr[largest], flag)==LEFT_BIG))
		largest = l;

	if ((r < n) && (compare(arr[r], arr[largest], flag) == LEFT_BIG))
		largest = r;

	// Swap and continue heapifying if root is not largest
	if (largest != i)
	{
		swapary(arr, i, largest);
		heapify(arr, n, largest, flag);
	}
}
 
/* 기수 정렬 (배열, 데이터 갯수, 0, NAME_SORT or YEAR_SORT)
arr: 배열
n: 데이터의 갯수
x: 0 (안쓰는 값)
flag: 년도 또는 제목으로 정렬할지 여부
*/
void radixsort(Movies arr[], int n, int x, int flag)
{
	// Find the maximum number to know number of digits 
	int exp;
	int count_n;

	//계수정렬에 쓰이는 배열
	Movies *output;
	int *count;

	if (flag == YEAR_SORT) count_n = 10;
	else count_n = CHAR_MAX;

	// 동적할당은 여기서 받음
	output = (Movies*)malloc(sizeof(Movies)*n);
	if (output == NULL)
		error_handling("malloc() - radixsort_year");

	count = (int*)malloc(sizeof(int) * count_n);
	if (count == NULL)
		error_handling("malloc() - radix_name, count");

	// flag에 따라 이름 또는 년도로 정렬
	if (flag == YEAR_SORT) radixsort_year(arr, output, count, n, flag);
	else radixsort_name(arr, output, count, n, flag);

	free(output);
	free(count);
}
/* Radix Sort_YEAR
arr: 배열
output: 정렬 결과를 임시 저장할 배열
count: 계수정렬용 배열
n: 데이터의 갯수
flag: 이름 또는 년도로 정렬할지 여부
*/
void radixsort_year(Movies arr[], Movies output[], int count[], int n, int flag)
{
	int exp;
	int m = 9000;

	for (exp = 1; m / exp > 0; exp *= 10)
		countSort(arr, output, count, n, 10, exp, flag);
}
/* Radix Sort_NAME
arr: 배열
output: 정렬 결과를 임시로 저장할 배열
count: 계수 정렬용 배열
n: 데이터의 갯수
flag: 이름 또는 년도로 정렬할지 여부
*/
void radixsort_name(Movies arr[], Movies output[], int count[], int n, int flag)
{
	int exp;
	int m = getMax(arr, n);

	// 문자열[m-1]부터 문자열[0]까지 계수정렬
	for (exp = m - 1; exp >= 0; exp--)
		countSort(arr, output, count, n, CHAR_MAX, exp, flag);
}
/* Count Sort_NAME
arr: 배열
output: 계수정렬에 쓰이는 임시 배열
count: 계수 정렬에 쓰이는 count 배열
n: 데이터 갯수
count_n: 계수의 수(숫자의 경우 10)
exp: 자릿수
flag: 이름 또는 년도로 정렬 여부
*/
void countSort(Movies arr[], Movies output[], int count[], int n, int count_n, int exp, int flag)
{
	int i;
	
	// count배열 초기화
	memset(count, 0, sizeof(int)*count_n);

	// Store count of occurrences in count[] 
	for (i = 0; i < n; i++) {
		count[getIdx(arr[i], exp, flag)]++;
	}

	// Change count[i] so that count[i] now contains actual 
	//  position of this digit in output[] 
	for (i = 1; i < count_n; i++)
		count[i] += count[i - 1];

	// Build the output array 
	for (i = n - 1; i >= 0; i--)
	{
		output[count[getIdx(arr[i], exp, flag)] - 1] = arr[i];
		count[getIdx(arr[i], exp, flag)]--;
	}

	// Copy the output array to arr[], so that arr[] now 
	// contains sorted numbers according to current digit 
	for (i = 0; i < n; i++)
		arr[i] = output[i];
}
/*가장 긴 제목의 길이 가져오기
arr: 배열
n: 데이터의 갯수
*/
int getMax(Movies arr[], int n)
{
	int i;
	int mx = arr[0].name_len;
	for (i = 1; i < n; i++)
		if (arr[i].name_len > mx)
			mx = arr[i].name_len;
	return mx;
}
/*count배열의 인덱스 값 계산
obj: 인덱스 값 계산 기준 원소
exp: 자릿수
flag: 이름 또는 년도로 정렬
*/
int getIdx(Movies obj, int exp, int flag) {
	if (flag == NAME_SORT) {
		if (obj.name_len < exp + 1)
			return 0;
		else
			return (unsigned char)obj.name[exp];
	}
	else {
		return (obj.year / exp) % 10;
	}
	
}

////////////////////////////////////////
//           메인 함수                //
////////////////////////////////////////

int main()
{
	Movies movies[ARRAY_SIZE];
	int max_index = 0;
	int flag1, flag2;

	//파일 읽어들이기
	max_index = readFile(movies);

	//정렬시간 측정, 결과내기
	printTimeOfSorts(movies, max_index);


	//바로 아래에있는 /* 을 없애면 테스트 코드 사용 가능, /*만 추가하면 다시 주석으로 바뀜
	/*
	flag2 = YEAR_SORT;
	flag1 = NAME_SORT;

	//selectsort(movies, max_index, 0, flag1);
	//selectsort(movies, max_index, 0, flag2);

	//insertsort(movies, max_index, 0, flag1);
	//insertsort(movies, max_index, 0, flag2);

	bubblesort(movies, max_index, 0, flag1);
	bubblesort(movies, max_index, 0, flag2);

	//mergesort(movies, 0, max_index - 1, flag1);
	//mergesort(movies, 0, max_index - 1, flag2);

	//quicksort(movies, 0, max_index - 1, flag1);
	//quicksort(movies, 0, max_index - 1, flag2);

	//heapSort(movies, max_index, 0, flag1);
	//heapSort(movies, max_index, 0, flag2);

	//radixsort(movies, max_index, 0, flag1);
	//radixsort(movies, max_index, 0, flag2);


	simple_test(movies, max_index);
	//disassemble_test(movies, max_index);
	/**/

	system("pause");
	return 0;
}