/*
	��ǻ�Ͱ��а� 2013150016 ���μ�

	<<movies.txt>>
	��ü �������� ��: 27279��
	��ȿ �������� ��: 27258��
	���� �������� ��: 21�� - �⵵ ������ ���� ��

	<<Program>>
	movies.txt�� �ִ� �� 3���������� �����͸� �����
	�پ��� ���� �˰������� ������ �����ϰ�
	�� ����ð����� �� �м��ϴ� ���α׷�
	movies.txt�� ���� ���� e-class�� �ִ� �״�� ���

	���α׷� ����ð�: �� 30~40��

	2�г� �ڷᱸ�����񿡼� ������ ������
	�̹� ������ ���뿡 �°� �����ؼ� ���
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//�迭�� ũ��� ��ȿ �������� ��
#define ARRAY_SIZE 30000	//�迭�� ũ��
#define DATA_NUM 27258		//��ȿ �������� ����: 27258��

//�׽�Ʈ�� ���� �˰����� ����
#define SORT_NUM 7

//���� ���� ���� ��
#define NAME_SORT 1	//�������� ����
#define YEAR_SORT 2	//�⵵�� ����

//���Ұ� �񱳿� ���
#define LEFT_BIG 1	//������ ũ��
#define RIGHT_BIG 2	//�������� ũ��
#define EQUAL 3		//����

//�������� ��������� �����ϱ� ���� ����
#define CHAR_MAX 255 //���� �ϳ��� ���� �� �ִ� �ִ� ��

//��ȭ ������ ���� ����ü
struct movies_ {
	char *name;	//����
	int year;	//�⵵
	char *genr;	//�帣

	int name_len;	//������ ����
};
typedef struct movies_ Movies;


enum{SELECT_TIME=0, INSERT_TIME, BUBBLE_TIME, 
	 MERGE_TIME, QUICK_TIME, HEAP_TIME, RADIX_TIME};

//���� �б� �׽�Ʈ�� ����
int number_of_data=0;	//��ü ���� ����(�������� ����)
int invalid_data=0;	//�߸��� �������� ����

////////////////////////////////////////////////
//               �Լ� ����                    //
////////////////////////////////////////////////

void error_handling(char*);			//����ó��-���α׷� ��������

void swapary(Movies *, int, int);	//�迭�� �� ��� ��ȯ
void copyary(Movies *, Movies *, int);	//�迭 �����ϱ�

int compare(Movies, Movies, int);	//���ϱ�, ���⼭ �Ʒ� �� �Լ� ȣ��
int compare_name(Movies, Movies);	//�̸����� ��
int compare_year(Movies, Movies);	//�⵵�� ��

int* getRandomIndex(int);	//���� �ε��� ���

void printTimeOfSorts(Movies *, int);	//�ð� ����
void timeofsort(Movies*, int, int);		//�� ������ �� �� �ð�����
void timeofsort_all(Movies*, int);		//��� �����Ϳ� ���� ���� �ð� ����
double checktime(void(*sort)(Movies *, int, int, int), Movies *, int, int);	//�ð�����
void printtime(double *);			//�ð� ���

int readFile(Movies *);					//���� ���پ� �б�
int disassemble(Movies *, char *, int);	// ���ٸ��� ����ü�� ������ �ֱ�
void disassemble_test(Movies *, int);	//����ü�� �� ���� �׽�Ʈ
void simple_test(Movies *, int);		//���� �׽�Ʈ, �� �Լ��� ���� �ϰ� �̰ɷ� �ι�° Ȯ��

void indexsort(int *, int);	//���� ������ �ε����� �����ϱ� ����, ���� ���� ���

/* ���� �˰���: �Լ� ������ ����� ���� �Ű����� ���� ���� */
/* ����, ����, ����, �պ�, �� ������ ��ó: 2�г� �ڷᱸ�� ���� */
void selectsort(Movies *, int, int, int);	//���� ����
void insertsort(Movies *, int, int, int);	//���� ����
void bubblesort(Movies *, int, int, int);	//���� ����

void mergesort(Movies *, int, int, int);	//�պ� ����
void merge(Movies *, int, int, int, int);

void quicksort(Movies *, int, int, int);	//�� ����
int partition(Movies *, int, int, int);

//�� ����(https://www.programiz.com/dsa/heap-sort)
void heapSort(Movies arr[], int, int, int);
void heapify(Movies arr[], int, int, int);

//���-��� ����(https://www.geeksforgeeks.org/radix-sort/)
void radixsort(Movies arr[], int, int, int);
void radixsort_year(Movies arr[], Movies output[], int count[], int, int);
void radixsort_name(Movies arr[], Movies output[], int count[], int, int);
void countSort(Movies arr[], Movies output[], int count[], int, int, int, int);
int getMax(Movies arr[], int);
int getIdx(Movies, int, int);

////////////////////////////////////////
//            �Լ� ����               //
////////////////////////////////////////

//Utilities
/*
str: Error: ���� ����� �����޽���
*/
void error_handling(char *str) {
	printf("Error: %s", str);
	system("cls");
	exit(1);
}

/*�����ϴµ� �ɸ� �ð� ������ �� �� ���
ary: ������ �迭
index: �������� ����
*/
void printTimeOfSorts(Movies *ary, int index) {
	//1. 100���� ������ ���Ŀ� ����
	printf("==< ������ %d�� ��� >================\n", 100);
	timeofsort(ary, index, 100);

	//2. 1000���� ������ ���Ŀ� ����
	printf("==< ������ %d�� ��� >===============\n", 1000);
	timeofsort(ary, index, 1000);
	
	//3. 10000���� ������ ���Ŀ� ����
	printf("==< ������ %d�� ��� >==============\n", 10000);
	timeofsort(ary, index, 10000);
	
	//4. ��ü ������ ���Ŀ� ����
	printf("==< ������ %d�� ��� >==============\n", index);
	timeofsort_all(ary, index);
}

/*���� ���� �� ��սð� ���� ��� ���
ary: ���� �迭
index: �������� ����
num: �迭 �� ������ �������� ��
*/
void timeofsort(Movies* ary, int index, int num)
{
	//���� �迭�� ���� ������ �迭�� �����
	Movies *sort;
	//���� �迭���� ������ ���� ���ҵ��� �ε���
	int *idx;

	double sorttime[SORT_NUM];	//�ɸ��ð� ������ �迭
	int i, j;					//�ݺ��뺯��

	sort = (Movies*)malloc(sizeof(Movies)*num);
	if (sort == NULL)
		error_handling("malloc() error - timeofsort");

	for (i = 0; i<SORT_NUM; i++)//���� �ɸ��ð��� ��� 0���� �ʱ�ȭ
		sorttime[i] = 0;

	//100�� ���� �� ��սð� ����
	//�� ���ĸ��� ���ο� num���� ���� ���ҷ� ���� ����
	for (i = 0; i<100; i++)		//100�� �ݺ�
	{
		//���� ���ҵ��� �ε��� ��������
		idx = getRandomIndex(num);

		//���� �迭�� ���� �ε����� ������ ��� �迭 �����
		for (j = 0; j < num; j++) {
			sort[j] = ary[idx[j]];
		}

		sorttime[SELECT_TIME] += checktime(selectsort, sort, num, 0);	//��������
		sorttime[INSERT_TIME] += checktime(insertsort, sort, num, 0);	//��������
		sorttime[BUBBLE_TIME] += checktime(bubblesort, sort, num, 0);	//��������
		sorttime[MERGE_TIME] += checktime(mergesort, sort, 0, num - 1);	//�պ�����
		sorttime[QUICK_TIME] += checktime(quicksort, sort, 0, num - 1);	//��  ����
		sorttime[HEAP_TIME] += checktime(heapSort, sort, num, 0);		//��  ����
		sorttime[RADIX_TIME] += checktime(radixsort, sort, num, 0);		//�������

		free(idx);
	}

	for (i = 0; i<SORT_NUM; i++)		//100���ݺ������� �ɸ��ð��� ����� ������
		sorttime[i] = sorttime[i] / 100;

	printtime(sorttime);				//�ɸ��ð��� ������ִ� �Լ� ȣ��
	free(sort);
}

/*��� ��ȿ�����Ϳ����� ���� ���� �� ��սð� ���� ��� ���
ary: �迭
index: �������� ����
*/
void timeofsort_all(Movies* ary, int index) {
	
	double sorttime[SORT_NUM];	//�ɸ��ð� ������ �迭
	int i;						//�ݺ��뺯��

	for (i = 0; i<SORT_NUM; i++)//���� �ɸ��ð��� ��� 0���� �ʱ�ȭ
		sorttime[i] = 0;

	//100�� ���� �� ��սð� ����
	//�� ���ĸ��� ���ο� num���� ���� ���ҷ� ���� ����
	for (i = 0; i<100; i++)		//100�� �ݺ�
	{
		sorttime[SELECT_TIME] += checktime(selectsort, ary, index, 0);	//��������
		sorttime[INSERT_TIME] += checktime(insertsort, ary, index, 0);	//��������
		sorttime[BUBBLE_TIME] += checktime(bubblesort, ary, index, 0);	//��������
		sorttime[MERGE_TIME] += checktime(mergesort, ary, 0, index - 1);//�պ�����
		sorttime[QUICK_TIME] += checktime(quicksort, ary, 0, index - 1);//��  ����
		sorttime[HEAP_TIME] += checktime(heapSort, ary, index, 0);		//��  ����
		sorttime[RADIX_TIME] += checktime(radixsort, ary, index, 0);	//�������
	}

	for (i = 0; i<SORT_NUM; i++)		//100���ݺ������� �ɸ��ð��� ����� ������
		sorttime[i] = sorttime[i] / 100;

	printtime(sorttime);				//�ɸ��ð��� ������ִ� �Լ� ȣ��
}

/*���� �ѹ� �ϴµ� �ɸ��� �ð� ����
sort: ������ ������ �Լ�������
ary, arg1, arg2, arg3: �� �Լ��� ���Ǵ� �Ű�������
*/
double checktime(void (*sort)(Movies *, int, int, int), Movies *ary, int arg1, int arg2)
{
	clock_t start, finsh;	//�ð������ϱ����� ����(����, ��)
	Movies *temp;//ary�� ���ĵǸ� �ȵ�, ������ �迭�� temp
	int num;

	//num = ������ �������� ����
	if (arg1) num = arg1;
	else num = arg2 + 1;

	temp = (Movies*)malloc(sizeof(Movies)*num);
	if (temp == NULL)
		error_handling("malloc error - checktime");

	copyary(ary, temp, num);		//temp�� ary�� ������ ����������

	start=clock();						//������ �ð�
	sort(temp, arg1, arg2, NAME_SORT);	//�����ϱ�(�̸� ���� ���� �⵵ ����)
	sort(temp, arg1, arg2, YEAR_SORT);
	finsh=clock();						//������ �ð�

	free(temp);

	return (double)(finsh-start)/CLOCKS_PER_SEC;	 //�ɸ��ð�=�����Ľð�-�������ð�
}

/* �ɸ� �ð� ��� ��� */
void printtime(double *sorttime)	//�ɸ��ð� ��� ȭ�鿡 ���
{
	printf("  1. �������� : %f\n", sorttime[SELECT_TIME]);
	printf("  2. �������� : %f\n", sorttime[INSERT_TIME]);
	printf("  3. �������� : %f\n", sorttime[BUBBLE_TIME]);
	printf("  4. �պ����� : %f\n", sorttime[MERGE_TIME]);
	printf("  5. ��  ���� : %f\n", sorttime[QUICK_TIME]);
	printf("  6. ��  ���� : %f\n", sorttime[HEAP_TIME]);
	printf("  7. ������� : %f\n", sorttime[RADIX_TIME]);
	printf("=======================================\n\n");
}

/*������ �ε����� ��� �Լ�
n: ���� �ε����� ��
*/
int* getRandomIndex(int n) {
	int i, j, k, ck;
	int *idx;

	idx = (int*)malloc(sizeof(int)*n);
	if (idx == NULL)
		error_handling("malloc() error - getRandomIndex");

	srand(time(NULL));

	//���� �ߺ� �˻縦 �ϸ� 100���� ���� �ε��� ����
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

	//������ ������ �ε��� ����
	indexsort(idx, n);

	return idx;
}

/*
ary: ��� �迭
i, j: ���� ��ġ�� �ٲ� �迭�� �ε���
*/
void swapary(Movies *ary, int i, int j)	//�迭�� �� ��� ��ġ �ٲٱ�
{
	Movies temp = ary[i];	//temp�� ary[i]�� �ֱ�, ary[i]�� �����
	ary[i] = ary[j];	//������� ary[j]�ֱ�, ary[j]�� �����
	ary[j] = temp;		//������� temp��(ary[i]�� ��)�ֱ�
}

/*
ary: ���� �迭
temp: ����� �迭
*/
void copyary(Movies *ary, Movies *temp, int num)
{
	int i;				//�ݺ������

	for(i=0;i<num;i++)	//temp�� ary�迭 �� ����
		temp[i]=ary[i];
}

/*
left: ���� ���� ����
right: ���� ������ ����

LEFT_BIG, RIGHT_BIG, EQUAL ��ȯ
*/
int compare(Movies left, Movies right, int flag) {
	int val;
	if (flag == NAME_SORT) val = compare_name(left, right);
	else if (flag == YEAR_SORT) val = compare_year(left, right);
	else error_handling("Invalid Use - compare(Movies, Movies, flag)");

	return val;
}

/*
left: ���� ���� ����
right: ���� ������ ����
*/
int compare_name(Movies left, Movies right) {
	int val;

	val = strcmp(left.name, right.name);

	if (val > 0) return LEFT_BIG;
	else if (val < 0) return RIGHT_BIG;
	else return EQUAL;
}

/*
left: ���� ���� ����
right: ���� ������ ����
*/
int compare_year(Movies left, Movies right) {
	if (left.year > right.year) return LEFT_BIG;
	else if (left.year < right.year) return RIGHT_BIG;
	else return EQUAL;
}

/*
movies: ���� ������ ������ ������ ����ü
*/
int readFile(Movies *movies) {

	FILE *fp;
	char buffer[300];
	int count = 0;
	int ck = 0;

	fp = fopen("movies.txt", "r");

	if (fp == NULL) error_handling("fopen()");

	//ù ���� ������(movieId,title,genres �κ�)
	fgets(buffer, 300, fp);

	while (!feof(fp)) {
		fgets(buffer, 300, fp);

		ck = disassemble(movies, buffer, count);

		//disassemble�� ���� ���� �Ǿ��� ���� count ����
		if (ck == 0)
			count++;

		//��ü �������� ���� Ȯ�ο� ī��Ʈ ����
		number_of_data++;

		/*
		//���� ������ ����� ���� �Ǿ� ���� �ʾƼ�
		//������ ���� �߻��Ѵ�
		//������ ������ �ľ��ϱ� ���� ��
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
movies: ��ȭ���� ����ü
string: ���Ͽ��� �о�� �� ��, �̰��� �����ؼ� ����ü�� �ִ´�
index: ���� �����ؾ��� ����ü�� �ε���
*/
int disassemble(Movies *movies, char *string, int index) {
	int str_idx = 0;	//string�� ���� �ε���
	int flag = 0;		//��ȭ �̸��� ���� �з�(ū ����ǥ ����)
	int name_start, name_size;	//��ȭ �̸� ���۰� ���� ���� �ε���
	int year_start;				//������ ���̰� �׻� 4�̹Ƿ� ���۸� �ʿ�
	int genr_start, genr_size;	//�帣�� ���۰� ���� ���� �ε���

	//������ ���� ��츦 ����� ī��Ʈ
	//������� �ݺ��ߴµ� ������ ��ã���� �� ��ȭ ������ �����Ѵ�
	int count = 0;

	//string�� ���� ���� �ε���
	//���� ������ ����� ���ϵǾ� ��������
	//(�⵵)���� �ٷ� ����(,)�� ���� ��찡 ����
	//(�⵵)���� ��ĭ ��� ����(,)�� ���� ��찡 ����
	int sub_idx = 0;

	//ó�� ���ڿ� �������� �б� -> �ʿ���� ����
	while (string[str_idx] != ',') str_idx++;
	str_idx++;

	//��ȭ �̸��� ū ����ǥ���� �����Ѵٸ�
	if (string[str_idx] == '"') {
		flag = 1;	//flag ����(ū ����ǥ�� ��� Ư�� ó��)
		str_idx++;	//�� ��� ���� ���ڰ� �̸��� �����̹Ƿ� �ε��� ����
	}

	//�̸��� ���� �ε��� ����
	name_start = str_idx;

	//Ÿ��Ʋ�� ū ����ǥ�� ������ ���ִ°� ����
	if (flag == 1) while (string[str_idx] != '"' || string[str_idx + 1] != ',') str_idx++;
	else while (string[str_idx] != ',') str_idx++;

	//������ ������ ã������ �ڷΰ��� Ž��
	sub_idx = str_idx;
	while (string[sub_idx] != '(') {
		count++;
		sub_idx--;

		//8�� �̻� �ڷΰ��� ��ã���� �⵵�� ���� �����ΰ�
		//�̰�� ������
		if (count > 8) {
			invalid_data++;	//���� �б� �׽�Ʈ�� ��������
			return -1;
		}
	}


	//���� ����� �ϳ� ������Ű�� ������ ���ۺκ��� ����Ű�Եȴ�
	sub_idx++;

	year_start = sub_idx;						//������ ���� �ε��� ����
	name_size = year_start - 2 - name_start;	//�̸��� ���� ����

												//str_idx�� ���� " �Ǵ� ,�� ����Ű���ִ�
												//�Ʒ� ������ str_idx�� �帣�� ù ���ڸ� ����Ų��
	str_idx++;
	str_idx += flag;

	genr_start = str_idx;						//�帣�� ���� �ε��� ����

	while (string[str_idx] != '\n') str_idx++;	//���๮�ڱ��� �б�

	genr_size = str_idx - genr_start;			//�帣�� ���� ����

	//����ü�� �ɹ������� �����Ҵ� �ޱ�
	movies[index].name = (char *)malloc(sizeof(char)*(name_size + 1));
	movies[index].genr = (char *)malloc(sizeof(char)*(genr_size + 1));

	//�����Ҵ���� ������� �ʱ�ȭ
	memset(movies[index].name, 0, name_size + 1);
	memset(movies[index].genr, 0, genr_size + 1);

	//�̸�, �帣���� ����
	memcpy(movies[index].name, &(string[name_start]), name_size);
	memcpy(movies[index].genr, &(string[genr_start]), genr_size);

	//����ü�� �������� �ֱ�
	movies[index].year = atoi(&(string[year_start]));

	//����ü�� ������ ���� �ֱ�
	movies[index].name_len = strlen(movies[index].name);

	//������ 0 ��ȯ
	return 0;
}

/*
movies: ��ȭ���� ����ü
index: ������ ����
*/
void disassemble_test(Movies *movies, int index) {

	int i, j;
	int *idx;

	idx = getRandomIndex(100);

	printf("%-110s %-6s %s\n", "Title", "Years", "Genre");

	//100���� ���� �ε����� ���� ����ü�� ����� ���� ���
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
movies: ��ȭ���� ����ü
index: ������ ����
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
ary: �ε��� �迭
n: �ε��� �迭�� ���� ����
*/
void indexsort(int *ary, int n) {
	int sel;		//�������Ұ�
	int i, j;		//�ݺ������

	for (i = 0; i < n - 1; i++)	//�� n-1�� �ݺ�
	{
		sel = ary[i + 1];		//i ������Ҹ� ����
		j = i;					//j�� i�� ����
		while ((j >= 0) && (ary[j] > sel))	//i��Һ��� 0��°��ұ��� ��
		{
			ary[j + 1] = ary[j];		//sel�� ���� �ڸ��� �� �� ����� ���� ����
			j--;						//j�ϳ� ����
		}
		ary[j + 1] = sel;		//�ݺ����� �����ٸ� �� �����ڸ��� ������̵� �ű⿡ sel�� ����
	}
}

//Sort Functions
/* ���� ���� (�迭, ������ ����, 0, NAME_SORT or YEAR_SORT)
ary: �迭
n: �������� ����
x: 0 (�Ⱦ��� ��)
flag: �̸� �Ǵ� ������ ����
*/
void selectsort(Movies *ary, int n, int x, int flag)	//���� ����
{
	int least;			//�ּҰ�
	int i, j;			//�ݺ������

	for (i = 0; i < n - 1; i++)		//ù��° ��Һ��� ����
	{
		least = i;					//i ��Ұ� �ּҰ��̶�� ����
		for (j = i + 1; j < n; j++)	//i ���� ��Һ��� ������ i ��ҿ� ��
		{
			if (compare(ary[j], ary[least], flag)==RIGHT_BIG)	//i��Ұ� �ּҰ��� �ƴϾ��ٸ�
				least = j;				//�ּҰ��� j����
		}								//�ݺ��ؼ� ���� �ּҰ�ã��
		if (least != i)					//�����Ѵ�� i�� �ּҰ��̸� ������ ��ȯ�� ����
			swapary(ary, i, least);
	}
}

/*���� ���� (�迭, ������ ����, 0, NAME_SORT or YEAR_SORT)
ary: ������ �迭
n: �迭�� ũ��(������ ����)
x: 0 (�Ⱦ��� ��)
flag: �̸� �Ǵ� ������ ����
*/
void insertsort(Movies *ary, int n, int x, int flag)	//���� ����
{
	Movies sel;		//�������Ұ�
	int i, j;			//�ݺ������

	for (i = 0; i < n - 1; i++)	//�� n-1�� �ݺ�
	{
		sel = ary[i+1];		//i ������Ҹ� ����
		j = i;					//j�� i�� ����
		while (j >= 0 && (compare(ary[j], sel, flag)==LEFT_BIG))	//i��Һ��� 0��°��ұ��� ��
		{
			ary[j + 1] = ary[j];		//sel�� ���� �ڸ��� �� �� ����� ���� ����
			j--;						//j�ϳ� ����
		}
		ary[j + 1] = sel;		//�ݺ����� �����ٸ� �� �����ڸ��� ������̵� �ű⿡ sel�� ����
	}
}

/*���� ���� (�迭, ������ ����, 0, NAME_SORT or YEAR_SORT)
ary: �迭
n: �������� ����
x: 0 (�Ⱦ��� ��)
flag: �̸� �Ǵ� ������ ����
*/
void bubblesort(Movies *ary, int n, int x, int flag)	//���� ����
{
	int i, j;			//�ݺ������

	for (i = 0; i < n - 1; i++)			//�ݺ�Ƚ�� ������ ���� �ݺ���(n-1�� �ݺ�)
	{
		for (j = 0; j < n - 1 - i; j++) //ó���� n-1��, �ι�° n-2��...������ 1�� �ݺ��ϱ����� n-1-i(i�� ���� n-2����)
		{
			if (compare(ary[j], ary[j+1], flag)==LEFT_BIG)	//j��Ұ� �״�����Һ��� ũ�ٸ� ������ġ��ȯ
				swapary(ary, j, j + 1);
		}
	}
}

/*�պ� ���� (�迭, 0, ������ ����-1, NAME_SORT or YEAR_SORT)
ary: ������ �迭
left: 0 (������ �迭�� ���� ���� �ε���)
right: ������ ���� - 1 (������ �迭�� ���� ������ �ε���)
flag: �̸� Ȥ�� �⵵�� ���� ����
*/
void mergesort(Movies *ary, int left, int right, int flag)	//�պ� ����
{
	int mid;	//mid��

	if(left<right)	//���� �����ε����� ���� �������ε������� �������� ����
	{
		mid = (left + right) / 2;		//�߰���ġ ���ϱ�
		mergesort(ary, left, mid, flag);		//���� ���ʺ��� �߰������� �� ������ ����
		mergesort(ary, mid + 1, right, flag);//�߰� �������� ���� �����ʱ����� �� �����γ���
		merge(ary, left, mid, right, flag);	//�� �������� ���� ���Ľ���
	}
}
/*�պ�
ary: �迭
left: ���� �ε���
mid: ��� �ε���
right: ������ �ε���
flag: �̸� Ȥ�� �⵵�� ���� ����
*/
void merge(Movies *ary, int left, int mid, int right, int flag)	//�պ�
{
	int i = left;		//i�� ���ʺκ� ù �ε���
	int j = mid + 1;	//j�� �����ʺκ� ù �ε���
	int index = left;	//�Ʒ� �ӽù迭 temp�� ���� �ε���
	Movies temp[ARRAY_SIZE];	//�պ��Ҷ� �� �ӽù迭

	while (i <= mid && j <= right)	//�����ε����� �������ε����� ������ �������� �ݺ�
	{
		if(compare(ary[i], ary[j], flag)!=LEFT_BIG)
			temp[index++] = ary[i++];
		else
			temp[index++] = ary[j++];
	}
	if (i > mid)					//�� �ݺ��� �����µ� �������� ���Ҵٸ�
		while (j <= right)			//���� ������ ���� ��� �ű�
			temp[index++] = ary[j++];
	else							//������ ���Ҵٸ�
		while (i <= mid)			//���� ���� ��� �ű�
			temp[index++] = ary[i++];

	for (i = left; i <= right; i++)	//���� i ��Ȱ��, �ӽù迭�� �������� �迭�� �ֱ�
		ary[i] = temp[i];
}

/*�� ���� (�迭, 0, ������ ����-1, NAME_SORT or YEAR_SORT)
ary: �迭
left: 0 (������ �迭�� ���� ���� �ε���)
right: ������ ���� -1 (������ �迭�� ���� ������ �ε���)
flag: �̸� Ȥ�� �⵵�� ���� ����

������ �ڵ�� ������ �ǻ��ڵ带 ����
-> ���� �������ִ� �ڵ庸�� �ڵ� ���̵� ª�� ȿ�������� ������ �ٲ�
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
/*�ǹ� ã��
ary: �迭
left: ���� ���� �ε���
right: ���� ������ �ε���
flag: ���� �Ǵ� �⵵�� �������� ����
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

/* Heap Sort (�迭, ������ ����, 0, NAME_SORT or YEAR_SORT)
arr: �迭
n: �������� ����
x: 0 (�Ⱦ��� ��)
flag: �̸� Ȥ�� �⵵�� ���� ����
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
arr: �迭
n: ���� ũ��(�������� ����)
i: max heap ���� ��ġ(���� Ʈ���� ��Ʈ)
flag: �̸� Ȥ�� �⵵�� ���� ����
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
 
/* ��� ���� (�迭, ������ ����, 0, NAME_SORT or YEAR_SORT)
arr: �迭
n: �������� ����
x: 0 (�Ⱦ��� ��)
flag: �⵵ �Ǵ� �������� �������� ����
*/
void radixsort(Movies arr[], int n, int x, int flag)
{
	// Find the maximum number to know number of digits 
	int exp;
	int count_n;

	//������Ŀ� ���̴� �迭
	Movies *output;
	int *count;

	if (flag == YEAR_SORT) count_n = 10;
	else count_n = CHAR_MAX;

	// �����Ҵ��� ���⼭ ����
	output = (Movies*)malloc(sizeof(Movies)*n);
	if (output == NULL)
		error_handling("malloc() - radixsort_year");

	count = (int*)malloc(sizeof(int) * count_n);
	if (count == NULL)
		error_handling("malloc() - radix_name, count");

	// flag�� ���� �̸� �Ǵ� �⵵�� ����
	if (flag == YEAR_SORT) radixsort_year(arr, output, count, n, flag);
	else radixsort_name(arr, output, count, n, flag);

	free(output);
	free(count);
}
/* Radix Sort_YEAR
arr: �迭
output: ���� ����� �ӽ� ������ �迭
count: ������Ŀ� �迭
n: �������� ����
flag: �̸� �Ǵ� �⵵�� �������� ����
*/
void radixsort_year(Movies arr[], Movies output[], int count[], int n, int flag)
{
	int exp;
	int m = 9000;

	for (exp = 1; m / exp > 0; exp *= 10)
		countSort(arr, output, count, n, 10, exp, flag);
}
/* Radix Sort_NAME
arr: �迭
output: ���� ����� �ӽ÷� ������ �迭
count: ��� ���Ŀ� �迭
n: �������� ����
flag: �̸� �Ǵ� �⵵�� �������� ����
*/
void radixsort_name(Movies arr[], Movies output[], int count[], int n, int flag)
{
	int exp;
	int m = getMax(arr, n);

	// ���ڿ�[m-1]���� ���ڿ�[0]���� �������
	for (exp = m - 1; exp >= 0; exp--)
		countSort(arr, output, count, n, CHAR_MAX, exp, flag);
}
/* Count Sort_NAME
arr: �迭
output: ������Ŀ� ���̴� �ӽ� �迭
count: ��� ���Ŀ� ���̴� count �迭
n: ������ ����
count_n: ����� ��(������ ��� 10)
exp: �ڸ���
flag: �̸� �Ǵ� �⵵�� ���� ����
*/
void countSort(Movies arr[], Movies output[], int count[], int n, int count_n, int exp, int flag)
{
	int i;
	
	// count�迭 �ʱ�ȭ
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
/*���� �� ������ ���� ��������
arr: �迭
n: �������� ����
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
/*count�迭�� �ε��� �� ���
obj: �ε��� �� ��� ���� ����
exp: �ڸ���
flag: �̸� �Ǵ� �⵵�� ����
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
//           ���� �Լ�                //
////////////////////////////////////////

int main()
{
	Movies movies[ARRAY_SIZE];
	int max_index = 0;
	int flag1, flag2;

	//���� �о���̱�
	max_index = readFile(movies);

	//���Ľð� ����, �������
	printTimeOfSorts(movies, max_index);


	//�ٷ� �Ʒ����ִ� /* �� ���ָ� �׽�Ʈ �ڵ� ��� ����, /*�� �߰��ϸ� �ٽ� �ּ����� �ٲ�
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