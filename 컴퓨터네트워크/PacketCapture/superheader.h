/*
헤더파일들과 전처리지시자들의 모음
*/

#ifndef _SUPER_HEADER_  //이 헤더파일은 딱 한번만 포함된다
#define _SUPER_HEADER_

#define _REENTRANT

/* 기존 헤더파일들 포함
   이것저것 넣고 빼고하다보니 
   많아졌다 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <termios.h>
#include <fcntl.h>
#include <sys/select.h>
#include <sys/time.h>

#include <signal.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <stdarg.h>

//Packet Capture for DataLink layer ETH_ALL_P
#include <linux/if.h>
#include <linux/if_ether.h>

/* 직접 만든 헤더파일들 포함 */
#include "common_func.h"
#include "http.h"
#include "telnet.h"
#include "ftp.h"
#include "dns.h"

/* 여기는 Define 모음 */
#define MAX_BUFFER_SIZE 65536

#define ALL 0
#define HTTP 1
#define FTP 2
#define TELNET 3
#define DNS 4
#define SIM_ALL 5

#define TCP 6
#define UDP 17

#define THREAD_DESTROY 1

/*
https://kldp.org/node/35884
*/
#define BAUDRATE B38400

#endif