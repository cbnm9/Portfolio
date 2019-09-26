/*
2013150016 ���μ�
�⼮��ȣ : 4��
�ü�� ������ - �ǵ�� �˰���
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

class Process {	//���μ���
private:
	int id;						//id
	int arrive_time;			//�����ð�
	int burst_time;				//�ʿ��� ����ð�
	int end_time;				//����ð�
	int return_time;			//��ȯ�ð�
	double normalized_rtime;	//����ȭ�� ��ȯ�ð�

	int need_time;				//���� ����ð�
	int level;					//���� ��ġ�� �غ�ť ����
	/* 2�� ���� ����Ʈ
	   fp: ���� ���μ��� ������(��������)
	   rp: ���� ���μ��� ������(�ڿ�����)
	   Head���� rp�� pop���(���� ������ٸ� ���μ���)*/
	Process *fp;
	Process *rp;
public:
	Process() {}
	Process(int id, int arrive_t, int burst_t)	//������
		:id(id), arrive_time(arrive_t), 
		burst_time(burst_t), need_time(burst_t)
	{
		level = 0;	//���ο� ���μ����� 0���� ť ����
	}

	/* Head(���̳��)���� ��� */
	void setHead() {	//���̳�� �ʱ�ȭ
		fp = this;
		rp = this;
	}
	void push(Process *nps) {	//���μ��� IN
		nps->rp = this;
		nps->fp = fp;
		fp->rp = nps;
		fp = nps;
	}
	Process* pop() {	//���μ��� OUT
		Process *tp = rp;
		rp->rp->fp = this;
		rp = rp->rp;
		tp->fp = NULL;
		tp->rp = NULL;
		return tp;
	}
	int getArTime() {	//���� ���μ��� ���� �ð� ��ȯ
		if (rp == this)
			return -1;
		return rp->arrive_time;
	}
	/* Simulator�� ���� �Լ� */
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

	/* Process ��ü���� ��� */
	void del() {			//�����Ҵ� �޸� ��ȯ
		delete(this);
	}
	void levelINC() {		//���� �ϳ� ����
		/* 9�� �ǵ��ť: 0~8�ܰ� */
		if (level < 8) {
			level++;
		}
	}
	int getLevel() {		//���� ������ ��ȯ
		return level; 
	}
	void need_time_Dec() {	//���� ����ð� ����
		need_time--; 
	}
	int getNeed_t() {		//���� ����ð� ��ȯ
		return need_time; 
	}
	int getId() {			//���μ���ID ��ȯ
		return id;
	}
	Process* finish(int time) {
		//���μ��� ����� �� ���

		//����ð�
		end_time = time;
		//��ȯ�ð�=����ð�-�����ð�
		return_time = time - arrive_time;
		//����ȭ�� ��ȯ�ð�=��ȯ�ð�/����ƮŸ��
		normalized_rtime = 
			(double)return_time / burst_time;

		return this;
	}
	void showinfo() {
		//���μ��� ������ ���ٿ� ���
		cout << "  ";
		cout << id << "\t\t" 
			 << arrive_time << "\t\t" 
			 << burst_time << "\t\t" 
			 << end_time << "\t\t" 
			 << return_time << "\t\t" 
			 << normalized_rtime << endl;
	}
	
	void shortinfo() {	//���� �б� Ȯ�ο�
		printf("%d, %d, %d\n", 
			id, arrive_time, burst_time);
	}

	~Process() {}
};

class ProcessSet {	//���Ͽ��� �о�� ���μ������� ����
private:
	Process psHead;		//���μ��� ����Ʈ�� ���
	int n;				//���μ��� ����
	int total_bt;		//�� �ʿ��� CPU�ð�
public:
	//������ : ���Ͽ��� ������ �о���°�
	ProcessSet() {
		n = 0;				//���μ��� ���� �ʱ�ȭ
		total_bt = 0;		//�� ����Ʈ Ÿ�� �ʱ�ȭ
		psHead.setHead();	//���̳�� �ʱ�ȭ
		FILE *fp;			//����������
		/* ���� ����(input.txt �Ǵ� input.txt.txt) */
		fp = fopen("input.txt", "r");
		if (fp == NULL) {
			fp = fopen("input.txt.txt", "r");
			if (fp == NULL) {
				printf("File open error!!\n");
				system("pause");
				exit(1);
			}
		}
		/* �ʿ��� ������ */
		int id, at, bt;
		int i, j;
		char buff[50];
		/* ���� �б� 
		   1. ���� �б�
		   2. �������� 0�̸� ����
		   3. ù��° ','���� ����
		   4. ù ���� ����
		   5. �ι�° ','���� ����
		   6. �ι�° ���� ����
		   7. ������ ���� ����
		   8. ���μ��� ����(push)
		   9. 1���� �ݺ�*/
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

	Process* pop() {	//���μ��� �ϳ� ��������
		return psHead.pop();
	}

	//���� ���μ����� ���� �� �ð� ��ȯ
	int getPushTime() { return psHead.getArTime(); }
	//�� �ʿ��� CPU�ð� ��ȯ
	int getTotal_bt() { return total_bt; }
	//�� ���μ��� ���� ��ȯ
	int getTotal_ps() { return n; }

	void showShort() {	//���� ����� �о�Դ��� Ȯ�ο�
		Process *tp;

		for (int i = 0; i < n; i++) {
			printf("Process %d: ", i + 1);
			tp = psHead.pop();
			tp->shortinfo();
			tp->del();
		}
	}
};

class Q {	//�غ�ť(����)
private:
	Process psHead;	//���μ��� ���
	int nops;		//���μ��� ����
	int level;		//�غ�ť�� �ܰ��ȣ(0�ܰ����->)
	Q *fp;			//����ť ������
	Q *rp;			//����ť ������
public:
	Q(){	//�⺻������
		psHead.setHead();
	}
	Q(int lv)	//new�����ڷ� ��ä�����ϴ°��
		:level(lv), nops(0)
	{
		psHead.setHead();
	}

	/* Head(���̳��)���� ��� */
	void setHead() {
		fp = this;
		rp = this;
	}
	void createQ(int lv) {	//�� ������ �غ�ť ����
		Q* nq = new Q(lv);

		nq->rp = this;
		nq->fp = fp;
		fp->rp = nq;
		fp = nq;
	}
	Q* searchLv(int lv) {	//�ش緹�� �غ�ť ã���ֱ�
		Q *tp = rp;
		for (int i = 0; i < lv; i++) {
			tp = tp->rp;
		}
		return tp;
	}
	void push(Process *ps) {	//���μ��� ����
		psHead.push(ps);
		nops++;
	}
	Process* pop() {	//���μ��� OUT(����ġ)
		nops--;
		return psHead.pop();
	}
	void allDel() {	//�Ҵ���� �޸� ��ȯ
		Q *tq = rp;
		Q *tq2;
		while (tq != this) {
			tq2 = tq;
			tq = tq->rp;
			delete(tq2);
		}
	}
	/* Simulator�� ���� �Լ� */
	string getStringOfQ(int lv) {
		Q *tp = searchLv(lv);

		return tp->psHead.getStringOfQ();
	}

	/* Q��ü���� ��� */
	int getNops() { return nops; }	//���μ��� ������ȯ
	int getLevel() { return level; }//���� ��ȯ
};

class ReadyQ {	//�غ�ť(�ٴܰ�)
private:
	Q qHead;
	int level;	//level - 1 = ���� ��������
public:
	ReadyQ() {	//�⺻������ 0���� �غ�ť ����
		level = 0;
		qHead.setHead();
		qHead.createQ(level++);
	}

	void push(Process *ps) {
		int n = ps->getLevel();
		/* ���μ����� �����ؾ��� ������ �غ�ť�� ���ٸ�
		   ���������� �غ�ť ����*/
		if ( n > level - 1)
			qHead.createQ(level++);
		//�ش� ������ �غ�ť ã��
		Q *tq = qHead.searchLv(n);
		//�غ�ť�� ����(push)
		tq->push(ps);
	}
	Process* pop() {	//�ܱ⽺���ٸ�

		//0�������� Ž���� �����ٸ� ��� ���μ��� ��ȯ
		for (int i = 0; i < level; i++) {
			Q *tq = qHead.searchLv(i);
			if (tq->getNops() > 0) {
				return tq->pop();
			}
		}
		//�غ�ť�� ����ִٸ� NULL ��ȯ
		return NULL;
	}
	void qDel() {	//ť�� ���� �޸� ��ȯ
		qHead.allDel();
	}
	//���� ������������ ��ȯ
	int getLv() { return level - 1; }
	/* Simulator�� ���� �Լ� */
	string getStringOfQ(int lv) {
		return qHead.getStringOfQ(lv);
	}
};

class CPU {	//���μ����� ������ CPU
private:
	Process *ps;
	int qTime;	//�ð��Ҵ緮
public:
	CPU() { 
		qTime = 0;
		ps = NULL;
	}
	void Dispatch(Process *p) {	//����ġ
		ps = p;
		setqTime();	//�ð��Ҵ緮 ����
	}
	void setqTime() {	//�ð��Ҵ緮 ����
		qTime = 1;
		/* 0����: �ݺ����� = 1
		   1����: 1���ݺ� = 1*2 = 2
		   2����: 2���ݺ� = 1*2*2 = 4
		   ....
		*/
		for (int i = 0; i < ps->getLevel(); i++) {
			qTime = qTime * 2;
		}
	}
	void excute() { //���μ��� ����
		ps->need_time_Dec();	//���� ����ð� ����
		/* ���� ����ð��� 0�̶�� ���μ��� �Ϸ�Ȱ�
		   �ð��Ҵ�� 0���� ����*/
		if (ps->getNeed_t() == 0) {
			qTime = 0;
		}
		else {
			qTime--;	//�ð� �Ҵ緮 ����
		}
	}
	void setNull() {
		ps = NULL;
	}
	int getNeed_t() {	//���� ����ð� ��ȯ
		return ps->getNeed_t();
	}
	int getqTime() { return qTime; }//�ð��Ҵ緮 ����
	Process* getPs() { return ps; }	//���μ��� ����
};

/* dis_std() �Լ� ������
   ��� ������ �Լ��� Simulator�� ���� ��
   : 400~605�� ��κ��� ���� �䱸���� �� ���� */
class Display {	//Simulator�� ���� ���� Ŭ����
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
	/* ������ : �ɹ����� �ʱ�ȭ */
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
		str_Nps = "���μ��� ����";
		str_Eps = "���μ��� ����";
		str_Cps = "���μ��� �ð�����";
		str_No = "�غ�ť�� ���μ��� ����";
	}
	/* ������ ���� ���μ��� ���� */
	void push(Process *ps) {
		pHead.push(ps);
	}
	/* �� �̺�Ʈ �߻��� �� ���� ���� */
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
	/* � �̺�Ʈ �߻��߾����� Ȯ�� */
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
	/* �� ���μ��� ���� ������ ���� ���ڿ� ���� */
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
	/* ����� �����ֱ����� ���ڿ� */
	void dis_std() {
		cout << "  ";
		cout << setw(97) << setfill('=') << "" << endl;
		cout << "  ";
		cout << "���μ���ID" << '\t' << "�����ð�" << '\t'
			 << "���񽺽ð�" << '\t' << "����ð�" << '\t'
			 << "��ȯ�ð�" << '\t' << "����ȭ�� ��ȯ�ð�"
			 << endl;
		cout << "  ";
		cout << setw(97) << setfill('=') << "" << endl;
	}
	/* ������ ���� ���� */
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
	/* ������ ������ ���¿� �߻��� �̺�Ʈ ��� */
	void dis_sim() {

		system("cls");
		cout << endl << endl;

		/* Line 1 */
		cout << "  ";

		cout << "SIMULATOR" << endl << endl;
		/* Line 2 */
		cout << "  ";

		cout << "��ü ����ð� : ";
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
			 << "�����ð��Ҵ�: " << cpu_qTime << "  ";
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
	/* Simulator��忡�� ����� ��� */
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
	/* �޴� �׸� ��� */
	void menuItem() {
		cout << endl;
		cout << "  ====================" << endl;
		cout << "    Make Your Choice" << endl;
		cout << "  ====================" << endl;
		cout << "  1. ������� ����" << endl;
		cout << "  2. Simulator" << endl;
		cout << "  ====================" << endl;
	}
public:
	Menu() {}
	/* �����ϳ� ���� */
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
	Menu menu;		//���ø޴�

	ProcessSet ps;	//������ ���μ��� ����
	ReadyQ readyQ;	//�غ�ť
	CPU Cpu;		//���μ���

	Display dis(&ps);//ȭ�� ���

	int sel = menu.makeYourChoice();

	if (sel == STANDARD) {
		system("cls");
		cout << endl;
		dis.dis_std();
	}
	
	int n = ps.getTotal_bt();   //�� �ʿ��� CPU ���ð�
	int n_ps = ps.getTotal_ps();//�� ���μ��� ����
	int count = 0;

	/*
	1. �ű� ���μ����� ���� �غ�ť�� ���� ����
	2. �ð��ʰ� ���μ����� �� ���Ŀ� �غ�ť�� �����ϰ�
	3. �����ٸ� �˰����� ����
	*/

	/* i�� �ð�����, �� �ʿ��� CPU ���ð���ŭ ���� */
	for (int i = 0; i < n + 1; i++) {
		/* ���μ��� �����ϸ� ���� 
		   �����ð��� ������ ������
		   ��� ����*/
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

		/* �ð��Ҵ緮 ���Ҵٸ� ���μ��� ����
		   �ƴ϶�� �����ٸ��� ���μ��� ����*/
		if (Cpu.getqTime() > 0) {	//�ð��Ҵ緮 ����
			Cpu.excute();
		}
		else {						//�ð�����(����)
			/* ���Ʊ�ȯ�� ���� �������̴� ���μ�����
			   ������ ������ ���μ��� ������ */
			Process *tp = Cpu.getPs();
			Process *tp2 = readyQ.pop();

			/* tp�� NULL�� �ƴϸ� �ð����� �Ǵ� ����Ϸ�
			   tp�� NULL�̸� �������̴� ���μ��� ����*/
			if (tp != NULL) {

				if (tp->getNeed_t() == 0) {	//����Ϸ�
					tp->finish(i);
					Cpu.setNull();
					count++;
					/* 1. ����� ���μ����� ������
					   �ʿ��� ����ð��� �÷�����Ѵ�

					   2. ����� ���μ����� ������
					   ���μ��� ����ġ �� ����*/
					if (tp2 == NULL) {
						/* ����� ���μ����� ���°�
						   ��� ���μ����� �Ϸ�Ȱ���
						   �ʿ��� ����ð��� �ø��� �ʴ´�*/
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
				else {						//�ð�����
					if (sel == SIMULATE) {
						dis.setEvent(PS_CHANGE);
					}
					/* tp2�� NULL�̸� �غ�ť�� ���μ����� ����
					   tp2�� NULL�� �ƴϸ� ���Ʊ�ȯ*/
					if (tp2 == NULL) {
						if (sel == SIMULATE) {
							dis.setEvent(PS_NO);
						}
						Cpu.setqTime();	//���緹���� �ð��Ҵ�ο�
						Cpu.excute();
					}
					else {
						tp->levelINC();	  //��������
						readyQ.push(tp);  //�غ�ť����
						Cpu.Dispatch(tp2);//����ġ
						Cpu.excute();     //���μ��� ����
					}
				}
			}
			else {
				/* �������� ���μ����� ������ :
				   
				   1. ���� ������ ���μ����� �ִٸ�
				   ����ġ �� ����
				   
				   2. ���� ������ ���μ����� ���ٸ�
				   �ʿ��� CPU�ð� �ø�*/
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

	readyQ.qDel();	//ť �޸� ��ȯ

	if (sel == SIMULATE) {
		dis.lastDisplay();
	}
	cout << endl;

	system("pause");

	return 0;
}