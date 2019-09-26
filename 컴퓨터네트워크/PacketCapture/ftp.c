#include "superheader.h"

void PrintFtp(unsigned char* buffer, int size) {
	PrintIpHeader(buffer, size);
	PrintTcpHeader(buffer,size);
	//이더넷 14바이트+IP 20바이트+TCP 20바이트
	PrintPayload(buffer+14+20+20, size-14-20-20);
}