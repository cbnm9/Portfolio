#include "superheader.h"

void PrintDns(unsigned char* buffer,int size) {
    PrintIpHeader(buffer, size);
    PrintUdpHeader(buffer, size);
    PrintSimpleDns(buffer, size);
    PrintPayload(buffer+14+20+8+12, size-14-20-8-12);
}

void PrintSimpleDns(unsigned char* buffer, int size) {
    char* type=buffer+14+20+8;

    unsigned short temp=0;
    unsigned short id=0;

    unsigned short qr=0;
    unsigned short opcode=0;
    unsigned short flags=0;
    unsigned short z=0;
    unsigned short rcode=0;

    unsigned short qd_c=0;
    unsigned short an_c=0;
    unsigned short ns_c=0;
    unsigned short ar_c=0;

    //DNS id 가져오기
    temp=0;
    memcpy(&temp, type, 2);
    id=ntohs(temp);

    //두번째 줄 가져오기
    temp=0;
    memcpy(&temp, type+2, 2);
    temp=ntohs(temp);

    qr=((temp&0x8000)>>15)&1;
    opcode=(temp&0x7800)>>11;
    flags=(temp&0x780)>>7;
    z=(temp&0x70)>>4;
    rcode=(temp&0xF);

    //QD Count
    temp=0;
    memcpy(&temp, type+4, 2);
    qd_c=ntohs(temp);

    //an Count
    temp=0;
    memcpy(&temp, type+6, 2);
    an_c=ntohs(temp);

    //ns Count
    temp=0;
    memcpy(&temp, type+8, 2);
    ns_c=ntohs(temp);

    //ar Count
    temp=0;
    memcpy(&temp, type+10, 2);
    ar_c=ntohs(temp);
    
    printf("=== DNS Header =======================\n");
    printf("  %-22s: 0x%x\n", "ID", id);
    printf("  %-22s: %s\n", "Query / Response", PrintQorR(qr));
    printf("  %-22s: %s\n", "Opcode", PrintOpcode(opcode));
    printf("  %-22s: ", "Flags"); PrintFlags(flags);
    printf("  %-22s: %d\n", "Z", z);
    printf("  %-22s: %d\n", "Reply Code", rcode);
    printf("  %-22s: %d\n", "N-O Questions", qd_c);
    printf("  %-22s: %d\n", "N-O Answer RRs", an_c);
    printf("  %-22s: %d\n", "N-O Questions", ns_c);
    printf("  %-22s: %d\n", "N-O Answer RRs", ar_c);
    printf("  (N-O: \" Number Of \") \n");
    printf("======================================\n");
}

char* PrintQorR(unsigned short code) {
    if(code==0)
        return "Query";
    else
        return "Response";
}
char* PrintOpcode(unsigned short code) {
    switch(code) {
        case 0:
            return "Standard Query";
        case 1:
            return "Inverse Query";
        case 2:
            return "Status";
        case 3:
            return "Unassigned";
        case 4:
            return "Notify";
        case 5:
            return "Update";
        default:
            return "Unassigned";
    }
}

/*
http://darksoulstory.tistory.com/62
*/
void PrintFlags(unsigned short code) {
    int ck=0;

    if((code>>3)&0x1) {
        printf("AA");
        ck=1;
    }

    if((code>>2)&0x1) {
        if(ck)
            printf(", ");
        printf("TC");
        ck=1;
    }

    if((code>>1)&0x1) {
        if(ck)
            printf(", ");
        printf("RD");
        ck=1;
    }

    if(code&0x1) {
        if(ck)
            printf(", ");
        printf("RA");
    }

    printf("\n");
}