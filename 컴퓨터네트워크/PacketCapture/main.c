#include "superheader.h"	//이거 하나에 모든 전처리문 때려박음

int sock_raw;   //패킷 받을때 사용되는 소켓

//큰 크기의 메모리 공간 할당(패킷 가져오는 용도)
unsigned char *buffer;

//프로토콜 구분 - 각 프로토콜 별 출력 함수 호출(필터링)
int protocol_type;

//IP주소를 받아오는데는 동적 메모리 할당 사용함
//따라서 메인에서 IP주소가 필요할 때만 할당받기위해 사용
int current_mod;

//distinguish, printSimple용 전역변수들
char *source_ip;
char *dest_ip;
unsigned short source_port;
unsigned short dest_port;
unsigned int seq_no;
unsigned int ack_no;
unsigned short window_no;

/*
키보드로부터 문자하나 입력받는 함수
입력받은 하나의 문자를 제외한
뒤의 의미없는 문자 버퍼 제거
-> 길게 입력해도 맨 앞의 하나만 읽음
*/
char InputFromKeyboard() {
	char inp = fgetc(stdin);
	while(fgetc(stdin)!='\n');

	return inp;
}

/*
https://kldp.org/node/35884
인터넷에서 찾은 엔터 없이 키보드 입력 받기소스
사용 실패 - 터미널에서 printf 출력이 이상하게나옴
int kbhit1(int stdfd) {
	fd_set readfs;
	struct timeval Timeout;
	unsigned char rv;
	int res;
	int cnt;

	while(1) {
    	//printf(" loop working \r\n");
    	FD_SET(stdfd, &readfs);
    	Timeout.tv_sec = 3;
    	//printf( "select:%d\r\n", res = select(stdfd + 1, &readfs, NULL, NULL, &Timeout));
		res = select(stdfd + 1, &readfs, NULL, NULL, &Timeout);
    	if ( res < 0 ) { 
			perror(" Error on the select");
        	return -1;
		}
      	else if ( res > 0 ) { 
			if ( FD_ISSET(stdfd,&readfs) ) { 
				if ( read(stdfd,&rv,1) < 0 ) { 
					perror( "Error on read" );
            		return -1; 
				}
            return rv; 
		}
        else
            return 0;
      }
   }
}
void waitKeyPress() {
	char c=0;
	int stdfd;
	struct termios oldtio, newtio;
	stdfd=fileno(stdin);

	tcgetattr(stdfd, &oldtio);
	bzero(&newtio, sizeof(newtio));

	newtio.c_cflag=BAUDRATE | CRTSCTS | CS8 | CLOCAL | CREAD;
	//newtio.c_iflag=IGNPAR | ICRNL;
	newtio.c_iflag=IGNPAR;
	newtio.c_oflag=0;
	newtio.c_lflag=0;

	newtio.c_cc[VTIME]=0;
	newtio.c_cc[VMIN]=1;

	tcflush(stdfd, TCIFLUSH);
	tcsetattr(stdfd, TCSANOW, &newtio);

	while(1) {
		c=kbhit1(stdfd);
		if(c)
			break;
	}
	tcsetattr(stdfd, TCSANOW, &oldtio);
}
*/

/*
아무 키나 누르고 엔터를 누르면
패킷 캡처 프로세스가 종료됨
*/
void waitKeyPress() {
	InputFromKeyboard();
}

/*
프로세스 종료시키기
-> SIGINT 사용
*/
void terminateProc(pid_t pid) {
	int ck=kill(pid, SIGINT);
	if(ck==-1) {
		printf("kill() error");
		exit(1);
	}
}

/*
TCP 헤더에서 정보 뽑아내기
*/
void getTcpInfo() {
	char *type;

	seq_no=0;
	ack_no=0;
	window_no=0;

	//Sequence 번호 가져오기
	type=buffer+38;
	memcpy(&seq_no, type, 4);
	seq_no=ntohl(seq_no);

	//Ack 번호 가져오기
	type=buffer+42;
	memcpy(&ack_no, type, 4);
	ack_no=ntohl(ack_no);

	//Window 크기 가져오기
	type=buffer+48;
	memcpy(&window_no, type, 2);
	window_no=ntohs(window_no);
}

/*
포트번호(프로토콜 구분용)뽑아내기
1. IPv4 구분하기 - Ethernet Header의 type 부분
2. TCP/UDP 구분하기 - IP Header의 Protocol부분
3. 포트번호 구하기 - TCP/UDP Header의 source, dest
*/
void distinguishProtocol() {
	char *type;

	unsigned short ether_type=0;
	unsigned short h_ether_type=0;

	unsigned short ip_type=0;
	unsigned short h_ip_type=0;

	unsigned short port_no=0;

	struct in_addr temp_ip;

	char *temp;

	source_port=0;
	dest_port=0;

	//1. Ethernet에서 IPv4만 출력하도록 거르기
	type=buffer+12;

	memcpy(&ether_type, type, 2);
	h_ether_type=ntohs(ether_type);

	if(h_ether_type!=0x0800) {
		protocol_type=-1;
		return;
	}

	//2. TCP와 UDP만 통과, 나머지 무시
	type=buffer+22;

	memcpy(&ip_type, type, 2);
	h_ip_type=ntohs(ip_type);
	
	h_ip_type=h_ip_type&255;

	switch(h_ip_type) {
		case TCP:
			getTcpInfo();
		case UDP:
			break;
		default:
			protocol_type=-1;
			return;
	}

	//3. 출발지, 도착지 ip번호 뽑아내기
	if(current_mod==SIM_ALL) {
		type=buffer+26;

		memcpy(&(temp_ip.s_addr), type, 4);
		temp=inet_ntoa(temp_ip);

		source_ip=(char *)malloc(strlen(temp)+1);
		memset(source_ip, 0, strlen(temp)+1);
		memcpy(source_ip, temp, strlen(temp));

		type=buffer+30;

		memcpy(&(temp_ip.s_addr), type, 4);
		temp=inet_ntoa(temp_ip);

		dest_ip=(char *)malloc(strlen(temp)+1);
		memset(dest_ip, 0, strlen(temp)+1);
		memcpy(dest_ip, temp, strlen(temp));
	}

	//4. 포트번호로 프로토콜 구분
	type=buffer+34;

	//출발지 포트번호 가져오기
	port_no=0;
	memcpy(&port_no, type, 2);
	source_port=ntohs(port_no);

	//도착지 포트번호 가져오기
	port_no=0;
	memcpy(&port_no, type+2, 2);
	dest_port=ntohs(port_no);

	if((source_port==20) || (source_port==21) || (dest_port==20) || (dest_port==21))
		protocol_type=FTP;
	else if((source_port==80) || (dest_port==80))
		protocol_type=HTTP;
	else if((source_port==23) || (dest_port==23))
		protocol_type=TELNET;
	else if((source_port==53) || (dest_port==53))
		protocol_type=DNS;
	else
		protocol_type=-1;
}
/*
처음 보여지는 선택화면 출력
*/
void Menu(int err) {
	system("clear");
	printf("========================\n");
	printf("    Make Your Choice    \n");
	printf("========================\n");
	printf("1. Show All Packet\n");
	printf("\n");
	printf("2. Show Only HTTP\n");
	printf("3. Show Only FTP\n");
	printf("4. Show Only Telnet\n");
	printf("5. Show Only DNS\n");
	printf("\n");
	printf("6. All Simple\n");
	printf("\n");
	printf("0. Exit This Program\n");
	printf("========================\n");
	if(err) printf("Input: Number(0 or 1 or 2 or 3 or 4 or 5)\n");
	printf("Select : ");
}

/*
raw소켓을 열고 무작위모드로 설정
오류나면 프로그램 종료
*/
void CreateSocketAndPromisicMode() {

	struct ifreq ifr;

	//raw socket 생성
	//중요! root권한이어야지만 열림
	sock_raw = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
	if(sock_raw < 0){ //raw_socket 오류 처리
		printf("Raw Socket Error: %d\n", sock_raw);
		exit(1);
	}

	/*
	raw socket 보조자료와는 달리
	ifreq의 멤버로 바로 ifrn_name, ifrn_flags가 있지 않고
	ifr_ifrn이 걸쳐서 있음
	이유: 헤더파일로 linux/if_ether.h 사용함
	*/
    strncpy(ifr.ifr_ifrn.ifrn_name, "eth0", strlen("eth0")+1);
    if(ioctl(sock_raw, SIOCGIFFLAGS, &ifr) < 0) {
		printf("Promiscous Error: Name\n");
		close(sock_raw);
		exit(1);
	}

    ifr.ifr_ifru.ifru_flags |= IFF_PROMISC;
    if(ioctl(sock_raw, SIOCSIFFLAGS, &ifr) < 0) {
		printf("Promiscous Error: Flag\n");
		close(sock_raw);
		exit(1);
	}
}

/*
패킷의 정보를 간단하게 한줄로 출력하기
Wire Shark와의 비교를 위해
출발/도착 IP, PORT
TCP의 경우 SEQ, ACK Window만 출력
-> 패킷이 잡히는 갯수만 비교해보기
*/
void printSimple(int count) {
	//count는 row소켓을 연 후 받은 패킷의 수
	//출력 결과 보면 이 숫자가 중간중간 뛰어넘는 것이 있는데
	//이것은 출력을 하지 않는 프로토콜이 온 경우이다
	//즉, HTTP, FTP, DNS, TELNET이 아닌경우 카운트는 증가하지만
	//출력은 하지 않는다
	printf("  %d> ", count);
	printf("IP: %s -> %s ", source_ip, dest_ip);
	printf("Port: %d -> %d ", source_port, dest_port);
	printf("Protocol: ");
	switch(protocol_type) {
		case HTTP:
			printf("HTTP ");
			break;
		case FTP:
			printf("FTP ");
			break;
		case DNS:
			printf("DNS ");
			break;
		case TELNET:
			printf("TELNET ");
			break;
	}

	switch(protocol_type) {
		case HTTP:
		case FTP:
		case TELNET:
			printf("Seq: %u ", seq_no);
			printf("ACK: %u ", ack_no);
			printf("Win: %d\n", window_no);
			break;
		default:
			printf("\n");
	}

	free(source_ip);
	free(dest_ip);
}

/*
패킷 캡쳐 출력 형식 별
Thread 쓰려다가 쓰레드만
강제종료 시키는 방법을 못찾음
recvfrom에서 블록된 상태에서도 강제종료 시켜야함
프로세스가 강제종료 시키기 더 좋음
따라서 멀티 프로세스로 노선 변경ㅠㅠ
*/
void* Trd_All(void *arg) {

	int data_size;
	struct sockaddr saddr;
	int saddr_size = sizeof saddr;
	
	while(1) {
		//패킷 받기
		data_size = recvfrom(sock_raw, buffer , MAX_BUFFER_SIZE , 0 , &saddr , &saddr_size);
		if(data_size < 0 ){
			printf("recvfrom() Error\n");
			exit(1);
		}

		distinguishProtocol();

		switch (protocol_type){
			case FTP:
				PrintFtp(buffer,data_size);
				break;
			case HTTP:
				PrintHttp(buffer,data_size);
				break;
			case TELNET:
				PrintTelnet(buffer,data_size);
				break;
			case DNS:
				PrintDns(buffer, data_size);
				break;
			default:
				break;
		}
	}
}

void* Trd_HTTP(void *arg) {
	
	int isExit, data_size;
	struct sockaddr saddr;
	int saddr_size = sizeof saddr;
	
	while(1) {

		//패킷 받기
		data_size = recvfrom(sock_raw, buffer , MAX_BUFFER_SIZE , 0 , &saddr , &saddr_size);
		if(data_size < 0 ){
			printf("recvfrom() Error\n");
			exit(1);
		}

		distinguishProtocol();

		switch (protocol_type){
			case HTTP:
				PrintHttp(buffer,data_size);
				break;
			default:
				break;
		}
	}
}

void* Trd_FTP(void *arg) {
	
	int isExit, data_size;
	struct sockaddr saddr;
	int saddr_size = sizeof saddr;
	
	while(1) {

		//패킷 받기
		data_size = recvfrom(sock_raw, buffer , MAX_BUFFER_SIZE , 0 , &saddr , &saddr_size);
		if(data_size < 0 ){
			printf("recvfrom() Error\n");
			exit(1);
		}

		distinguishProtocol();

		switch (protocol_type){
			case FTP:
				PrintFtp(buffer,data_size);
				break;
			default:
				break;
		}
	}
}

void* Trd_TELNET(void *arg) {
	
	int isExit, data_size;
	struct sockaddr saddr;
	int saddr_size = sizeof saddr;
	

	while(1) {

		//패킷 받기
		data_size = recvfrom(sock_raw, buffer , MAX_BUFFER_SIZE , 0 , &saddr , &saddr_size);
		if(data_size < 0 ){
			printf("recvfrom() Error\n");
			exit(1);
		}

		distinguishProtocol();

		switch (protocol_type){
			case TELNET:
				PrintTelnet(buffer,data_size);
				break;
			default:
				break;
		}
	}
}

void* Trd_DNS(void *arg) {
	
	int isExit, data_size;
	struct sockaddr saddr;
	int saddr_size = sizeof saddr;
	
	while(1) {

		//패킷 받기
		data_size = recvfrom(sock_raw, buffer , MAX_BUFFER_SIZE , 0 , &saddr , &saddr_size);
		if(data_size < 0 ){
			printf("recvfrom() Error\n");
			exit(1);
		}

		distinguishProtocol();

		switch (protocol_type){
			case DNS:
				PrintDns(buffer, data_size);
				break;
			default:
				break;
		}
	}
}

void* Trd_ASIM(void *arg) {

	int data_size;
	struct sockaddr saddr;
	int saddr_size = sizeof saddr;
	int count=1;
	
	while(1) {
		//패킷 받기
		data_size = recvfrom(sock_raw, buffer , MAX_BUFFER_SIZE , 0 , &saddr , &saddr_size);
		if(data_size < 0 ){
			printf("recvfrom() Error\n");
			exit(1);
		}

		distinguishProtocol();

		switch (protocol_type){
			case FTP:
			case HTTP:
			case TELNET:
			case DNS:
				printSimple(count);
			default:
				break;
		}

		count++;
	}
}
/*
선택한 메뉴 항목 별 패킷받는 함수
*/
void Capture(int flag) {

	//멀티 프로세스
	pid_t pid;

	//함수 포인터 선언
	void* (*fp) (void*);

	current_mod=flag;

	switch(flag) {
		case ALL:
			fp=Trd_All;
			break;
		case HTTP:
			fp=Trd_HTTP;
			break;
		case FTP:
			fp=Trd_FTP;
			break;
		case TELNET:
			fp=Trd_TELNET;
			break;
		case DNS:
			fp=Trd_DNS;
			break;
		case SIM_ALL:
			fp=Trd_ASIM;
			break;
		default:
			printf("Impossible\n");
			exit(1);
	}

	system("clear");

	pid=fork();
	//raw 소켓 열고 무작위모드 설정하기
	CreateSocketAndPromisicMode();
	switch(pid) {
		case -1:	//Error
			printf("fork() Error\n");
			break;
		case 0:		//Child
			fp(NULL);
			break;
		default:	//Parent
			waitKeyPress();
			terminateProc(pid);
			waitpid(pid, NULL, 0);
			break;
	}
	close(sock_raw);
}

//////////////////
/*  Main 함수   */
/////////////////

int main(void) {

	/*------------ 변수 영역  ---------------*/

	//메뉴 선택용 변수들 선언
	char select;	//메뉴항목 선택용 변수
	int err=0;		//잘못된 입력 처리용 변수

	//큰 크기의 메모리 공간 할당(패킷 가져오는 용도)
	buffer = (unsigned char *)malloc(MAX_BUFFER_SIZE);


	/*--------------  프로그램 동작 영역  --------------*/

    while(1)
    {
		Menu(err); //선택메뉴 출력
		err=0;

		select = InputFromKeyboard(); //키보드로부터 입력 받기

		switch(select){
		
			case '1': //전부 조회
				Capture(ALL);
				break;

			case '2': //http 조회
				Capture(HTTP);
				break;

			case '3': //ftp 조회
				Capture(FTP);
				break;

			case '4': //telent 
				Capture(TELNET);
				break;

			case '5': //dns
				Capture(DNS);
				break;
			
			case '6': //전부 조회, 간단히
				Capture(SIM_ALL);
				break;

			case '0': //종료
				return 0;

			default : //다른 키 눌렀을 시 오류 처리
				err=1;
				break;
		}		
    }
	
	return 0; 
}