/*
2013150016 서민성
출석번호 : 4번
운영체제 팀과제 - 피드백 알고리즘
*/

#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <stdlib.h>
#include <string>

#define STANDARD 1
#define SIMULATE 2
#define NEW_PS 0
#define PS_EXIT 1
#define PS_CHANGE 2
#define PS_NO 3

using std::cout;
using std::cin;
using std::endl;
using std::setw;
using std::setfill;
using std::string;

class Process {	//프로세스
private:
	int id;						//id
	int arrive_time;			//도착시간
	int burst_time;				//필요한 실행시간
	int end_time;				//종료시간
	int return_time;			//반환시간
	double normalized_rtime;	//정규화된 반환시간

	int need_time;				//남은 실행시간
	int level;					//현재 위치한 준비큐 레벨
	/* 2중 연결 리스트
	   fp: 이전 프로세스 포인터(먼저들어온)
	   rp: 다음 프로세스 포인터(뒤에들어온)
	   Head에서 rp가 pop대상(가장 오래기다린 프로세스)*/
	Process *fp;
	Process *rp;
public:
	Process() {}
	Process(int id, int arrive_t, int burst_t)	//생성자
		:id(id), arrive_time(arrive_t), 
		burst_time(burst_t), need_time(burst_t)
	{
		level = 0;	//새로운 프로세스는 0레벨 큐 진입
	}

	/* Head(더미노드)에서 사용 */
	void setHead() {	//더미노드 초기화
		fp = this;
		rp = this;
	}
	void push(Process *nps) {	//프로세스 IN
		nps->rp = this;
		nps->fp = fp;
		fp->rp = nps;
		fp = nps;
	}
	Process* pop() {	//프로세스 OUT
		Process *tp = rp;
		rp->rp->fp = this;
		rp = rp->rp;
		tp->fp = NULL;
		tp->rp = NULL;
		return tp;
	}
	int getArTime() {	//다음 프로세스 진입 시간 반환
		if (rp == this)
			return -1;
		return rp->arrive_time;
	}
	/* Simulator를 위한 함수 */
	string getStringOfQ() {
		string str;
		Process *tp = fp;
		while (tp != this) {
			str += std::to_string(tp->id);
			tp = tp->fp;
		}
		return str;
	}
	void showForDisplay() {
		Process *p = rp;
		while (p != this) {
			cout << "  ";
			cout << p->id << "\t\t"
				 << p->arrive_time << "\t\t"
				 << p->burst_time << "\t\t"
				 << p->end_time << "\t\t"
				 << p->return_time << "\t\t"
				 << p->normalized_rtime << endl;
			p = p->rp;
		}
	}

	/* Process 객체에서 사용 */
	void del() {			//동적할당 메모리 반환
		delete(this);
	}
	void levelINC() {		//레벨 하나 증가
		/* 9단 피드백큐: 0~8단계 */
		if (level < 8) {
			level++;
		}
	}
	int getLevel() {		//현재 레벨값 반환
		return level; 
	}
	void need_time_Dec() {	//남은 실행시간 감소
		need_time--; 
	}
	int getNeed_t() {		//남은 실행시간 반환
		return need_time; 
	}
	int getId() {			//프로세스ID 반환
		return id;
	}
	Process* finish(int time) {
		//프로세스 종료시 값 계산

		//종료시간
		end_time = time;
		//반환시간=종료시간-도착시간
		return_time = time - arrive_time;
		//정규화된 반환시간=반환시간/버스트타임
		normalized_rtime = 
			(double)return_time / burst_time;

		return this;
	}
	void showinfo() {
		//프로세스 정보를 한줄에 출력
		cout << "  ";
		cout << id << "\t\t" 
			 << arrive_time << "\t\t" 
			 << burst_time << "\t\t" 
			 << end_time << "\t\t" 
			 << return_time << "\t\t" 
			 << normalized_rtime << endl;
	}
	
	void shortinfo() {	//파일 읽기 확인용
		printf("%d, %d, %d\n", 
			id, arrive_time, burst_time);
	}

	~Process() {}
};

class ProcessSet {	//파일에서 읽어온 프로세스들의 집합
private:
	Process psHead;		//프로세스 리스트의 헤드
	int n;				//프로세스 갯수
	int total_bt;		//총 필요한 CPU시간
public:
	//생성자 : 파일에서 데이터 읽어오는곳
	ProcessSet() {
		n = 0;				//프로세스 갯수 초기화
		total_bt = 0;		//총 버스트 타임 초기화
		psHead.setHead();	//더미노드 초기화
		FILE *fp;			//파일포인터
		/* 파일 열기(input.txt 또는 input.txt.txt) */
		fp = fopen("input.txt", "r");
		if (fp == NULL) {
			fp = fopen("input.txt.txt", "r");
			if (fp == NULL) {
				printf("File open error!!\n");
				system("pause");
				exit(1);
			}
		}
		/* 필요한 변수들 */
		int id, at, bt;
		int i, j;
		char buff[50];
		/* 파일 읽기 
		   1. 한줄 읽기
		   2. 읽은값이 0이면 종료
		   3. 첫번째 ','까지 읽음
		   4. 첫 숫자 읽음
		   5. 두번째 ','까지 읽음
		   6. 두번째 숫자 읽음
		   7. 마지막 숫자 읽음
		   8. 프로세스 생성(push)
		   9. 1부터 반복*/
		while (1) {
			fscanf(fp, "%s", buff);
			if (atoi(buff) == 0) break;

			j = 0;
			while (buff[j++] != ',');
			id = atoi(buff);
			i = j;

			while (buff[j++] != ',');
			at = atoi(&(buff[i]));
			
			bt = atoi(&(buff[j]));
			total_bt += bt;

			psHead.push(new Process(id, at, bt));

			n++;
		}
				
		if (n == 0) {
			printf("No process!\n");
			system("pause");
			exit(1);
		}
		
		fclose(fp);
	}

	Process* pop() {	//프로세스 하나 내보내기
		return psHead.pop();
	}

	//다음 프로세스가 도착 할 시간 반환
	int getPushTime() { return psHead.getArTime(); }
	//총 필요한 CPU시간 반환
	int getTotal_bt() { return total_bt; }
	//총 프로세스 갯수 반환
	int getTotal_ps() { return n; }

	void showShort() {	//파일 제대로 읽어왔는지 확인용
		Process *tp;

		for (int i = 0; i < n; i++) {
			printf("Process %d: ", i + 1);
			tp = psHead.pop();
			tp->shortinfo();
			tp->del();
		}
	}
};

class Q {	//준비큐(단일)
private:
	Process psHead;	//프로세스 헤드
	int nops;		//프로세스 갯수
	int level;		//준비큐의 단계번호(0단계부터->)
	Q *fp;			//이전큐 포인터
	Q *rp;			//다음큐 포인터
public:
	Q(){	//기본생성자
		psHead.setHead();
	}
	Q(int lv)	//new연산자로 객채생성하는경우
		:level(lv), nops(0)
	{
		psHead.setHead();
	}

	/* Head(더미노드)에서 사용 */
	void setHead() {
		fp = this;
		rp = this;
	}
	void createQ(int lv) {	//새 레벨의 준비큐 생성
		Q* nq = new Q(lv);

		nq->rp = this;
		nq->fp = fp;
		fp->rp = nq;
		fp = nq;
	}
	Q* searchLv(int lv) {	//해당레벨 준비큐 찾아주기
		Q *tp = rp;
		for (int i = 0; i < lv; i++) {
			tp = tp->rp;
		}
		return tp;
	}
	void push(Process *ps) {	//프로세스 진입
		psHead.push(ps);
		nops++;
	}
	Process* pop() {	//프로세스 OUT(디스패치)
		nops--;
		return psHead.pop();
	}
	void allDel() {	//할당받은 메모리 반환
		Q *tq = rp;
		Q *tq2;
		while (tq != this) {
			tq2 = tq;
			tq = tq->rp;
			delete(tq2);
		}
	}
	/* Simulator를 위한 함수 */
	string getStringOfQ(int lv) {
		Q *tp = searchLv(lv);

		return tp->psHead.getStringOfQ();
	}

	/* Q객체에서 사용 */
	int getNops() { return nops; }	//프로세스 갯수반환
	int getLevel() { return level; }//레벨 반환
};

class ReadyQ {	//준비큐(다단계)
private:
	Q qHead;
	int level;	//level - 1 = 가장 하위레벨
public:
	ReadyQ() {	//기본생성자 0레벨 준비큐 생성
		level = 0;
		qHead.setHead();
		qHead.createQ(level++);
	}

	void push(Process *ps) {
		int n = ps->getLevel();
		/* 프로세스가 진입해야할 레벨의 준비큐가 없다면
		   하위레벨의 준비큐 생성*/
		if ( n > level - 1)
			qHead.createQ(level++);
		//해당 레벨의 준비큐 찾기
		Q *tq = qHead.searchLv(n);
		//준비큐로 진입(push)
		tq->push(ps);
	}
	Process* pop() {	//단기스케줄링

		//0레벨부터 탐색해 스케줄링 대상 프로세스 반환
		for (int i = 0; i < level; i++) {
			Q *tq = qHead.searchLv(i);
			if (tq->getNops() > 0) {
				return tq->pop();
			}
		}
		//준비큐가 비어있다면 NULL 반환
		return NULL;
	}
	void qDel() {	//큐에 사용된 메모리 반환
		qHead.allDel();
	}
	//가장 하위레벨정보 반환
	int getLv() { return level - 1; }
	/* Simulator를 위한 함수 */
	string getStringOfQ(int lv) {
		return qHead.getStringOfQ(lv);
	}
};

class CPU {	//프로세스를 수행할 CPU
private:
	Process *ps;
	int qTime;	//시간할당량
public:
	CPU() { 
		qTime = 0;
		ps = NULL;
	}
	void Dispatch(Process *p) {	//디스패치
		ps = p;
		setqTime();	//시간할당량 설정
	}
	void setqTime() {	//시간할당량 설정
		qTime = 1;
		/* 0레벨: 반복안함 = 1
		   1레벨: 1번반복 = 1*2 = 2
		   2레벨: 2번반복 = 1*2*2 = 4
		   ....
		*/
		for (int i = 0; i < ps->getLevel(); i++) {
			qTime = qTime * 2;
		}
	}
	void excute() { //프로세스 수행
		ps->need_time_Dec();	//남은 실행시간 감소
		/* 남은 실행시간이 0이라면 프로세스 완료된것
		   시간할당랑 0으로 설정*/
		if (ps->getNeed_t() == 0) {
			qTime = 0;
		}
		else {
			qTime--;	//시간 할당량 감소
		}
	}
	void setNull() {
		ps = NULL;
	}
	int getNeed_t() {	//남은 실행시간 반환
		return ps->getNeed_t();
	}
	int getqTime() { return qTime; }//시간할당량 리턴
	Process* getPs() { return ps; }	//프로세스 리턴
};

/* dis_std() 함수 제외한
   모든 변수와 함수는 Simulator를 위한 것
   : 400~605줄 대부분은 과제 요구사항 외 내용 */
class Display {	//Simulator를 위해 만든 클래스
private:
	Process pHead;
	int n_ps;
	string *pstr;
	string pstr2[9];
	int *pint;
	int ndx;

	bool NewPs;
	bool PsExit;
	bool PsChange;
	bool PsNo;
	int n_str;
	string str_Nps;
	string str_Eps;
	string str_Cps;
	string str_No;

	int n_lv;
	int cpu_ps;
	int cpu_qTime;
	int time;
public:
	/* 생성자 : 맴버변수 초기화 */
	Display(ProcessSet *pSet) {
		pHead.setHead();

		n_ps = pSet->getTotal_ps();
		pstr = new string[n_ps];
		pint = new int[n_ps];
		ndx = 0;

		NewPs = false;
		PsExit = false;
		PsChange = false;
		PsNo = false;
		n_str = 0;
		str_Nps = "프로세스 진입";
		str_Eps = "프로세스 종료";
		str_Cps = "프로세스 시간만료";
		str_No = "준비큐에 프로세스 없음";
	}
	/* 수행이 끝난 프로세스 보관 */
	void push(Process *ps) {
		pHead.push(ps);
	}
	/* 각 이벤트 발생시 그 정보 저장 */
	void setEvent(int evt) {
		switch (evt) {
		case NEW_PS:
			if (!NewPs) {
				NewPs = true;
				n_str++;
			}
			break;
		case PS_EXIT:
			PsExit = true;
			n_str++;
			break;
		case PS_CHANGE:
			PsChange = true;
			n_str++;
			break;
		case PS_NO:
			PsNo = true;
			n_str++;
			break;
		}
	}
	/* 어떤 이벤트 발생했었는지 확인 */
	string ckEvent() {
		if (NewPs) {
			NewPs = false;
			n_str--;
			return str_Nps;
		}
		if (PsExit) {
			PsExit = false;
			n_str--;
			return str_Eps;
		}
		if (PsChange) {
			PsChange = false;
			n_str--;
			return str_Cps;
		}
		if (PsNo) {
			PsNo = false;
			n_str--;
			return str_No;
		}
	}
	/* 각 프로세스 수행 내용을 위한 문자열 설정 */
	void setStr(int id, int n) {
		pstr[ndx] += "  Ps";
		pstr[ndx] += std::to_string(id);
		pstr[ndx] += ":";
		for (int i = 0; i < n; i++) {
			pstr[ndx] += " ";
		}
		pint[ndx] = id;
		ndx++;
	}
	/* 결과를 보여주기위한 문자열 */
	void dis_std() {
		cout << "  ";
		cout << setw(97) << setfill('=') << "" << endl;
		cout << "  ";
		cout << "프로세스ID" << '\t' << "도착시간" << '\t'
			 << "서비스시간" << '\t' << "종료시간" << '\t'
			 << "반환시간" << '\t' << "정규화된 반환시간"
			 << endl;
		cout << "  ";
		cout << setw(97) << setfill('=') << "" << endl;
	}
	/* 현재의 상태 저장 */
	void dis_setInf(ReadyQ *rQ, CPU *Cpu, int tm) {
		time = tm;
		n_lv = rQ->getLv();
		for (int i = 0; i <= n_lv; i++) {
			pstr2[i] = "";
			pstr2[i] += rQ->getStringOfQ(i);
		}
		Process *tps = Cpu->getPs();
		if (tps == NULL) {
			cpu_ps = 0;
		}
		else {
			cpu_ps = tps->getId();
		}
		cpu_qTime = Cpu->getqTime();

		for (int j = 0; j < ndx; j++) {
			if (pint[j] == cpu_ps) {
				pstr[j] += "0";
			}
			else {
				pstr[j] += " ";
			}
		}
	}
	/* 위에서 저장한 상태와 발생한 이벤트 출력 */
	void dis_sim() {

		system("cls");
		cout << endl << endl;

		/* Line 1 */
		cout << "  ";

		cout << "SIMULATOR" << endl << endl;
		/* Line 2 */
		cout << "  ";

		cout << "전체 진행시간 : ";
		cout.setf(_IOSleft);
		cout << setw(7) << time;
		cout << "Event: ";
		while (n_str > 0) {
			cout << ckEvent() << "  ";
		}
		cout << endl << endl;
		cout.unsetf(_IOSleft);
		/* Line 3 */
		cout << "  ";

		cout.setf(_IOSleft);
		cout << setw(17) << "ReadyQ";

		cout << "CPU: " << cpu_ps << "  "
			 << "남은시간할당: " << cpu_qTime << "  ";
		cout << endl;
		cout.unsetf(_IOSleft);
		/* Line 4~ */
		for (int i = 0; i <= n_lv; i++) {
			cout << "  ";

			cout << "Lv" << i << ". ";
			cout << setw(9) << pstr2[i] << "->";
			cout << endl;
		}
		cout << endl << endl;
		/* Graph */
		for (int k = 0; k < ndx; k++) {
			cout << pstr[k] << endl;
			if (k < ndx - 1) {
				cout << "       ";
				for (int z = 0; z < time; z++)
					cout << "-";
				cout << endl;
			}
		}
		cout << endl << endl;
		system("pause");
	}
	/* Simulator모드에서 결과값 출력 */
	void lastDisplay() {
		dis_std();
		pHead.showForDisplay();
	}
	~Display() {
		delete[] pstr;
		delete[] pint;
	}
};

class Menu {
private:
	/* 메뉴 항목 출력 */
	void menuItem() {
		cout << endl;
		cout << "  ====================" << endl;
		cout << "    Make Your Choice" << endl;
		cout << "  ====================" << endl;
		cout << "  1. 결과값만 보기" << endl;
		cout << "  2. Simulator" << endl;
		cout << "  ====================" << endl;
	}
public:
	Menu() {}
	/* 둘중하나 선택 */
	int makeYourChoice() {
		int sel = 0;
		bool ck = true;

		while (1) {
			if (sel == STANDARD || sel == SIMULATE) {
				return sel;
			}
			else {
				system("cls");
				menuItem();
				if (!ck) {
					cout << "  Error : Input 1 or 2" << endl;
				}
				cout << "  Input 1 or 2 : "; cin >> sel;
				if(ck)
					ck = false;
			}
		}
	}
};

int main() {
	Menu menu;		//선택메뉴

	ProcessSet ps;	//수행할 프로세스 집합
	ReadyQ readyQ;	//준비큐
	CPU Cpu;		//프로세서

	Display dis(&ps);//화면 출력

	int sel = menu.makeYourChoice();

	if (sel == STANDARD) {
		system("cls");
		cout << endl;
		dis.dis_std();
	}
	
	int n = ps.getTotal_bt();   //총 필요한 CPU 사용시간
	int n_ps = ps.getTotal_ps();//총 프로세스 갯수
	int count = 0;

	/*
	1. 신규 프로세스가 먼저 준비큐에 들어온 이후
	2. 시간초과 프로세스가 그 이후에 준비큐에 진입하고
	3. 스케줄링 알고리즘을 실행
	*/

	/* i는 시간진행, 총 필요한 CPU 사용시간만큼 진행 */
	for (int i = 0; i < n + 1; i++) {
		/* 프로세스 도착하면 진입 
		   같은시간에 여러개 도착시
		   모두 진입*/
		while (1) {
			if (i == ps.getPushTime()) {
				Process *t;
				t = ps.pop();
				readyQ.push(t);
				if (sel == SIMULATE) {
					dis.setStr(t->getId(), i);
					dis.setEvent(NEW_PS);
				}
			}
			else {
				break;
			}
		}

		if (sel == SIMULATE) {
			dis.dis_setInf(&readyQ, &Cpu, i);
		}

		/* 시간할당량 남았다면 프로세스 수행
		   아니라면 스케줄링후 프로세스 수행*/
		if (Cpu.getqTime() > 0) {	//시간할당량 남음
			Cpu.excute();
		}
		else {						//시간만료(선점)
			/* 문맥교환을 위해 수행중이던 프로세스와
			   다음에 수행할 프로세스 꺼내기 */
			Process *tp = Cpu.getPs();
			Process *tp2 = readyQ.pop();

			/* tp가 NULL이 아니면 시간만료 또는 수행완료
			   tp가 NULL이면 수행중이던 프로세스 없음*/
			if (tp != NULL) {

				if (tp->getNeed_t() == 0) {	//수행완료
					tp->finish(i);
					Cpu.setNull();
					count++;
					/* 1. 수행될 프로세스가 없을때
					   필요한 수행시간을 늘려줘야한다

					   2. 수행될 프로세스가 있을때
					   프로세스 디스패치 후 수행*/
					if (tp2 == NULL) {
						/* 수행될 프로세스가 없는게
						   모든 프로세스가 완료된경우는
						   필요한 수행시간을 늘리지 않는다*/
						if (count < n_ps) {
							n++;
						}
					}
					else {
						Cpu.Dispatch(tp2);
						Cpu.excute();
					}

					if (sel == STANDARD) {
						tp->showinfo();
						tp->del();
					}
					else {
						dis.push(tp);
						if (tp2 == NULL) {
							dis.setEvent(PS_NO);
						}
						dis.setEvent(PS_EXIT);
					}
				}
				else {						//시간만료
					if (sel == SIMULATE) {
						dis.setEvent(PS_CHANGE);
					}
					/* tp2가 NULL이면 준비큐에 프로세스가 없음
					   tp2가 NULL이 아니면 문맥교환*/
					if (tp2 == NULL) {
						if (sel == SIMULATE) {
							dis.setEvent(PS_NO);
						}
						Cpu.setqTime();	//현재레벨의 시간할당부여
						Cpu.excute();
					}
					else {
						tp->levelINC();	  //레벨증가
						readyQ.push(tp);  //준비큐진입
						Cpu.Dispatch(tp2);//디스패치
						Cpu.excute();     //프로세스 수행
					}
				}
			}
			else {
				/* 수행중인 프로세스가 없을때 :
				   
				   1. 다음 수행할 프로세스가 있다면
				   디스패치 후 실행
				   
				   2. 다음 수행할 프로세스가 없다면
				   필요한 CPU시간 늘림*/
				if (tp2 != NULL) {
					Cpu.Dispatch(tp2);
					Cpu.excute();
				}
				else {
					n++;
					if (sel == SIMULATE) {
						dis.setEvent(PS_NO);
					}
				}
			}
		}
		if (sel == SIMULATE) {
			dis.dis_sim();
		}
	}

	readyQ.qDel();	//큐 메모리 반환

	if (sel == SIMULATE) {
		dis.lastDisplay();
	}
	cout << endl;

	system("pause");

	return 0;
}