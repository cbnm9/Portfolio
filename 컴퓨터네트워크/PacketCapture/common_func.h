
void PrintIpHeader(unsigned char* , int);
void PrintIpFlags(unsigned short);
void PrintIpTos(unsigned short);

void PrintTcpHeader(unsigned char* , int);
void PrintTcpFlags(unsigned short);

void PrintUdpHeader(unsigned char*, int);
void PrintPayload(unsigned char* , int);
int CharacterFilter(char);