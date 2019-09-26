#include "superheader.h"

//IP
void PrintIpHeader(unsigned char* buffer, int size) {
    
    char *temp;

    unsigned short temp_short=0;
    unsigned short ver=0;
    unsigned short iphl = 0;    // for header Length
    unsigned short tos = 0;     // for type Of Service
    unsigned short totalLength = 0; // for total Length 
    unsigned short id = 0;  // for identifier  
    unsigned short flags = 0;   // for Flag
    unsigned short offset = 0;  // for fragment Offset
    unsigned short ttl = 0; // for time to live
    unsigned short aprotocol = 0;   // for protocol
    unsigned short checkSum = 0;
    unsigned int temp_long=0;
    struct in_addr temp_ip;
    char* char_temp_ip;
    char* sourceIp;
    char* destIp;

    //버전, 헤더길이, TOS
    temp = buffer + 14;
    memcpy(&temp_short, temp, 2);
    temp_short = ntohs(temp_short);

    ver=(temp_short>>12)&0xF;
    iphl=(temp_short>>8)&0xF;
    tos = temp_short&0xFF;

    //총 길이
    temp = buffer + 16;
    memcpy(&totalLength, temp, 2);    // get total length
    totalLength = ntohs(totalLength);
    
    //식별자
    temp = buffer + 18;
    memcpy(&id, temp, 2); // get identifier
    id = ntohs(id);

    //플래그와 오프셋
    temp = buffer + 20;
    memcpy(&temp_short, temp, 2);  // get flag
    temp_short = ntohs(temp_short);

    flags=(temp_short>>13)&0x7;
    offset=temp_short&0x1FFF;

    //TTL과 프로토콜
    temp = buffer + 22;
    memcpy(&temp_short, temp, 2);  // get protocol
    temp_short=ntohs(temp_short);

    ttl=(temp_short>>8)&0xFF;
    aprotocol=temp_short&0xFF;

    //헤더 체크섬
    temp = buffer + 24;
    memcpy(&checkSum, temp, 2);
    checkSum = ntohs(checkSum);

    //출발지 IP
    temp=buffer+26;

	memcpy(&(temp_ip.s_addr), temp, 4);
	char_temp_ip=inet_ntoa(temp_ip);

	sourceIp=(char *)malloc(strlen(char_temp_ip)+1);
	memset(sourceIp, 0, strlen(char_temp_ip)+1);
	memcpy(sourceIp, char_temp_ip, strlen(char_temp_ip));

    //도착지 IP
	temp=buffer+30;

	memcpy(&(temp_ip.s_addr), temp, 4);
	char_temp_ip=inet_ntoa(temp_ip);

	destIp=(char *)malloc(strlen(char_temp_ip)+1);
	memset(destIp, 0, strlen(char_temp_ip)+1);
	memcpy(destIp, char_temp_ip, strlen(char_temp_ip));

    printf("=== IPv4 Header =====================\n");
    printf("  %-22s: %u\n", "Version", ver);
    printf("  %-22s: %u\n", "Header Length", iphl*4);
    printf("  %-22s: ", "Type Of Service(TOS)"); PrintIpTos(tos);
    printf("  %-22s: %u\n", "Total Packet Length", totalLength);
    printf("  %-22s: 0x%04x\n", "Identifier", id);
    printf("  %-22s: ", "Flags"); PrintIpFlags(flags);
    printf("  %-22s: %u\n", "Fragment Offset", offset);
    printf("  %-22s: %u\n", "Time To Live(TTL)", ttl);
    printf("  %-22s: ", "Protocol");
    switch(aprotocol)
    {
        case 6:
            printf("TCP\n");
            break;
        case 17:
            printf("UDP\n");
            break;
    }
    printf("  %-22s: 0x%04x\n", "Header Checksum", checkSum);
    printf("  %-22s: %s\n", "Source IP", sourceIp);
    printf("  %-22s: %s\n", "Destination IP", destIp);
    printf("======================================\n");

    free(sourceIp);
    free(destIp);
}

void PrintIpTos(unsigned short code) {
    switch(code) {
        case 0:
            printf("Normal");
            break;
        case 1:
            printf("Minimize Cost");
            break;
        case 2:
            printf("Maximize Reliability");
            break;
        case 4:
            printf("Maximize Throughput");
            break;
        case 8:
            printf("Minimize Delay");
            break;
        case 15:
            printf("Maximize Security");
            break;
        default:
            printf("%d", code);
            break;
    }
    printf("\n");
}

void PrintIpFlags(unsigned short code) {
    int ck=0;

    if((code>>1)&0x1) {
        printf("Don't flagment");
        ck=1;
    }

    if(code&0x1) {
        if(ck)
            printf(", ");
        printf("More flagments");
        ck=1;
    }

    if(!ck) {
        printf("%d",code);
    }

    printf("\n");
}

//Tcp
void PrintTcpHeader(unsigned char* buffer, int size) {

    char *temp;

    unsigned short temp_short = 0;
    unsigned short sourcePort = 0;
    unsigned short destPort = 0;
    unsigned int seq = 0;
    unsigned int ack = 0;
    unsigned short headLength = 0;
    unsigned short flags=0;
    unsigned short window = 0;
    unsigned short checkSum = 0;

    //출발지 포트
    temp = buffer + 34;
	memcpy(&sourcePort, temp, 2);
	sourcePort=ntohs(sourcePort);

    //도착지 포트
    temp = buffer + 36;
	memcpy(&destPort, temp, 2);
	destPort=ntohs(destPort);

    //SEQ 번호
    temp = buffer + 38;
	memcpy(&seq, temp, 4);
	seq=ntohl(seq);

    //ACK 번호
    temp = buffer + 42;
	memcpy(&ack, temp, 4);
	ack=ntohl(ack);

    //헤더길이와 플래그
    temp = buffer + 46;
    memcpy(&temp_short, temp, 2);
    temp_short=ntohs(temp_short);

    headLength=(temp_short>>12)&0xF;
    flags=temp_short&0x3F;
    
    //윈도우
    temp = buffer + 48;
    memcpy(&window, temp, 2);
    window = ntohs(window);

    //체크섬
    temp = buffer + 50;
    memcpy(&checkSum, temp, 2);
    checkSum = ntohs(checkSum);

    printf("=== TCP Header========================\n");
    printf("  %-22s: %u\n", "Source Port", sourcePort);
    printf("  %-22s: %u\n", "Destination Port", destPort);
    printf("  %-22s: %u\n", "Sequence Number", seq);
    printf("  %-22s: %u\n", "ACK Number", ack);
    printf("  %-22s: %u Byte\n", "Header Length", headLength*4);
    printf("  %-22s: ", "Flags"); PrintTcpFlags(flags);
    printf("  %-22s: %u\n", "Window", window);
    printf("  %-22s: 0x%04x\n", "Checksum", checkSum);
    printf("=====================================\n");
}

void PrintTcpFlags(unsigned short code) {
    int ck=0;

    if((code>>5)&0x1) {
        printf("URG");
        ck=1;
    }

    if((code>>4)&0x1) {
        if(ck)
            printf(", ");
        printf("ACK");
        ck=1;
    }

    if((code>>3)&0x1) {
        if(ck)
            printf(", ");
        printf("Push");
        ck=1;
    }

    if((code>>2)&0x1) {
        if(ck)
            printf(", ");
        printf("Reset");
        ck=1;
    }

    if((code>>1)&0x1) {
        if(ck)
            printf(", ");
        printf("Syn");
        ck=1;
    }

    if(code&0x1) {
        if(ck)
            printf(", ");
        printf("Fin");
    }

    printf("\n");
}

//UDP
void PrintUdpHeader(unsigned char* buffer, int size) {
    char * udp_start;
    
    unsigned short temp;
    unsigned short s_port=0;
    unsigned short d_port=0;
    unsigned short len=0;
    unsigned short check_sum=0;

    udp_start=buffer+34;

	//출발지 포트번호 가져오기
	temp=0;
	memcpy(&temp, udp_start, 2);
	s_port=ntohs(temp);

	//도착지 포트번호 가져오기
	temp=0;
	memcpy(&temp, udp_start+2, 2);
	d_port=ntohs(temp);

    //헤더 길이 가져오기
    temp=0;
    memcpy(&temp, udp_start+4, 2);
    len=ntohs(temp);

    //체크섬 가져오기
    temp=0;
    memcpy(&temp, udp_start+6, 2);
    check_sum=ntohs(temp);

    printf("=== UDP Header =======================\n");
    printf("  %-22s: %u\n", "Source Port", s_port);
    printf("  %-22s: %u\n", "Destination Port", d_port);
    printf("  %-22s: %u Byte\n", "Length", len);
    printf("  %-22s: 0x%04x\n", "Checksum", check_sum);
    printf("======================================\n");
}

//문자 필터링 - payload에 출력할 수 있는 문자 구분
int CharacterFilter(char ch) {
    //출력 가능한 아스키코드: 32~126 두 숫자 포함
    if(ch>31 && ch<127) return 1;
    else return 0;
}

//Payload
void PrintPayload(unsigned char* data , int size) {
    //data: 페이로드 시작지점, size: 페이로드 길이
    int i, j;
    int last_p;
    
    if(size<=0) {
        printf("\n\n");
        return;
    }

    printf("=== Data =============================\n");

    for(i=0 ; i < size ; i++)
    {
        //i가 0인 처음은 건너띄고
        //매번 16바이트를 16진수로 출력할때마다
        //해당 데이터를 문자로도 출력해준다
        if(i%16==0 && i!=0)
        {
            //16진수 출력물로부터 3칸 띄기
            printf("   ");
            //출력한 16진수의 처음으로 돌아가서 다시 반복
            for(j=i-16 ; j<i ; j++)
            {
                //출력할 문자 필터링(문자 또는 점으로 출력)
                if(CharacterFilter(data[j]))
                {
                    printf("%c",(unsigned char)data[j]);
				} 
                else 
                {
					printf(".");
				}
            }
            //모두 출력했으면 한 칸 띄기
            printf("\n");
        } 

        //매 새로운 줄마다 2칸씩 띈다
        //i가 0인 처음도 2칸 띄기 위해 위 if문과 따로 구성
        if(i%16==0) printf("  ");

        //한 바이트씩 16진수로 출력
        printf("%02x ",(unsigned int)data[i]);

        //16진수 출력 시 8바이트마다 구분해준다
        if((i+1)%8 == 0) printf("  ");

        //마지막 바이트를 출력한 경우 위에서 처리 불가
        if(i==size-1)
        {
            //i 하나 증가 했을 때
            i++;
            //i%16이 0이 아니라는 것은 16바이트를 출력한게 아닌 것
            if(i%16 != 0)
            {
                //이때 i%16은 출력한 바이트 수
                //그러므로 남은 바이트수인 16-출력한 바이트수만큼
                //공백을 출력하여 문자열의 길이를 맟춰준다
                for(j=0; j<(16-i%16); j++) printf("   ");
                if(i%16 < 8) printf("  ");
                last_p=i-i%16;
            }
            else
            {
                last_p=i-16;
            }

            //5칸 띄기
            printf("     ");

            //이제 위처럼 문자로 출력하는 루틴 수행
            for(j=last_p ; j<i ; j++)
            {
                //출력할 문자 필터링(문자 또는 점)
                if(CharacterFilter(data[j]))
                {
                    printf("%c",(unsigned char)data[j]);
				} 
                else 
                {
					printf(".");
				}
            }
            //모두 출력했으면 한 칸 띄기
            printf("\n");
        }
    }

    printf("======================================\n\n\n");
}