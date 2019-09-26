#include "superheader.h"

void PrintHttp(unsigned char* buffer, int size) {

    //HTTP 메시지 시작위치 계산
    unsigned char *start=buffer+14+20+20;
    unsigned int count=0;
    unsigned char tp;
    char temp[4];
    int flag=0;
    int i;

    //먼저 IP, TCP 출력
    PrintIpHeader(buffer, size);
    PrintTcpHeader(buffer, size);

    memset(temp, 0, 4);

    //만약 HTTP메시지가 00부터 시작한다면 
    //헤더도 없는 메시지
    //즉, TCP 응답 메시지이다
    //이 경우 와이어샤크에서는 TCP로 분류하지만
    //이 프로그램에서는 HTTP로 분류해 처리한다
    if(start[0]==0x00) {
        PrintPayload(start, size-14-20-20);
        return;
    }

    //첫 세글자를 가지고 HTTP로 시작하는지
    //GET으로 시작하는지 알아낸다
    memcpy(temp, start, 3);

    //시작하는 글자에 맞게 출력한다
    switch (CompareString(temp)) {
        case 1: //HTTP~~
            PrintHttHead(start, size, 0);
            break;
        case 2: //GET~~
            PrintGetHead(start, size);
            break;
        default:    //ELSE
            PrintPayload(start, size-14-20-20);
            return;
    }
}

void PrintHttHead(unsigned char* buf, int size, int flag) {
    int cnt=0;
    int b_cnt=0;
    char *temp;

    //flag가 0이 아니라는 것은 아래의 PrintGetHead에서
    //이 함수를 호출했다는 것이다
    if(!flag) {
        printf("=== HTTP Header =====================\n");
    }

    //반복문으로 모든 헤더의 라인을 출력한다
    while(1) {
        //한 줄에서 캐리지리턴을 찾는다
        while(1) {
            if(buf[cnt]=='\r')
                break;
            cnt++;
        }
        cnt++;

        //찾은 캐리지 리턴까지 한 줄을 출력한다
        temp=(char *)malloc(cnt-b_cnt);
        memset(temp, 0, cnt-b_cnt);
        memcpy(temp, buf+b_cnt, cnt-b_cnt-1);
        printf("  %s\n",temp);
        free(temp);

        //만약 캐리지리턴이 또 있다면
        //헤더라인이 끝난것이다 이때 반복문 나간다
        if(buf[cnt+1]=='\r')
            break;
    
        cnt++;
        b_cnt=cnt;
    }

    //cnt+3하므로써 buf+cnt는 페이로드의 처음을 가리키게된다(있다면)
    cnt+=3;

    printf("=====================================\n");

    //HTTP 헤더는 다 출력했으니 페이로드를 출력한다
    PrintPayload(buf+cnt, size-54-cnt-flag);
}

void PrintGetHead(unsigned char* buf, int size) {
    char *temp;
    int l;
    int cnt=0;
    int ct2=2;

    printf("=== HTTP Header =====================\n");
    
    //만약 GET으로 시작한다면 그 줄은 따로 출력처리한다
    //안그러면 GET 마지막에 HTTP가 붙어 나와서
    //출력 결과물이 좋게 보이지 않는다
    //사실 GET줄이 매우 길어서 이렇게해도 썩 좋아보이진 않는다
    //GET줄은 공백문자가 반드시 두개가 있고,
    //마지막 공백 문자는 GET줄의 끝을 의미한다
    while(1) {
        if(buf[cnt]==' ')
            ct2--;
        if(!ct2)
            break;
        cnt++;
    }
    cnt++;

    //GET 줄을 출력한다
    temp=(char *)malloc(cnt);
    memset(temp, 0, cnt);
    memcpy(temp, buf, cnt-1);
    printf("  %s\n",temp);
    free(temp);

    //HTTP부터 있는 헤더부분을 출력한다
    PrintHttHead(buf+cnt, size, cnt);
}

int CompareString(char *text) {
    //헤더라인이 HTT로 시작하는지 또는
    //GET으로 시작하는지에 따라
    //1 또는 2를 리턴해주는 함수
    char *htt="HTT";
    char *get="GET";

    if(!strcmp(text, htt))
        return 1;
    
    if(!strcmp(text, get))
        return 2;
    
    return 0;
}