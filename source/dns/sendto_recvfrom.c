#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <pthread.h>

int socketfd1 = -1;
int socketfd2 = -1;

void *socket1Thread(void *arg)
{

    struct sockaddr_in dest = {0};

	dest.sin_family = AF_INET;
	dest.sin_port = htons(5061);
	dest.sin_addr.s_addr = inet_addr("192.168.0.109");

	struct sockaddr_in addrLocal;
	addrLocal.sin_family = AF_INET; //设置tcp协议族
	addrLocal.sin_port = htons(5068); //设置端口号
	addrLocal.sin_addr.s_addr = inet_addr("192.168.0.109"); //设置ip地址
	int ret = bind(socketfd1, (struct sockaddr*)&addrLocal, sizeof(addrLocal));

/*
    //先调用connect()函数，为套接字指定目的地址/端口
    struct sockaddr_in svr_addr;
    svr_addr.sin_family = AF_INET;
    svr_addr.sin_port = htons(5061);
    svr_addr.sin_addr.s_addr = inet_addr("192.168.0.109");
    connect(socketfd1, (struct sockaddr* )&svr_addr, sizeof(svr_addr));
    send(socketfd1, "socketfd1 sendto 192.168.0.109:5062", sizeof("socketfd1 sendto 192.168.0.109:5062"), 0);
*/
    sendto(socketfd1, "socketfd1 sendto 192.168.0.109:5062", sizeof("socketfd1 sendto 192.168.0.109:5062"), 0, (struct sockaddr*)&dest , sizeof(dest));
    printf("\r socketfd1 sendto: socketfd1 sendto 192.168.0.109:5062. \n");

    unsigned char buf[512];
	struct sockaddr_in addr = {0};
    unsigned long addr_len = sizeof(struct sockaddr_in);
    while (1) {
        recvfrom(socketfd1, buf, sizeof(buf) , 0, (struct sockaddr*)&addr , &addr_len);
        printf("\r socketfd1 recvfrom: %s  %s:%d. \n", buf, inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
    }
}

void *socket2Thread(void *arg)
{
    struct sockaddr_in dest = {0};

	dest.sin_family = AF_INET;
	dest.sin_port = htons(5068);
	dest.sin_addr.s_addr = inet_addr("192.168.0.109");

    struct sockaddr_in addrLocal;
    addrLocal.sin_family = AF_INET; //设置tcp协议族
    addrLocal.sin_port = htons(5062); //设置端口号
    addrLocal.sin_addr.s_addr = inet_addr("192.168.0.109"); //设置ip地址
    int ret = bind(socketfd2, (struct sockaddr*)&addrLocal, sizeof(addrLocal));

    while (1) {
        sendto(socketfd2, "socketfd2 sendto 192.168.0.109:5068", sizeof("socketfd2 sendto 192.168.0.109:5068"), 0, (struct sockaddr*)&dest , sizeof(dest));
        printf("\r socketfd2 sendto: socketfd2 sendto 192.168.0.109:5068. \n");
        sleep(2);
    }
}

int main(int argc , char *argv[])
{
	socketfd1 = socket(AF_INET, SOCK_DGRAM, 0);
	if(socketfd1 < 0){
		printf("\rcreate socketfd1 failed.\n");
		return 0;
	}

	socketfd2 = socket(AF_INET, SOCK_DGRAM, 0);
	if (socketfd2 < 0){
		printf("\rcreate socketfd2 failed.\n");
		return 0;
	}

    pthread_t threadid1;   //线程的标识符
    if (pthread_create(&threadid1, NULL, socket1Thread, NULL) != 0) {
        printf("\rcreate pthread1 failed.\n");  //打印错误信息
        return 0;
    }

    pthread_t threadid2;   //线程的标识符
    if (pthread_create(&threadid2, NULL, socket2Thread, NULL) != 0) {
        printf("\rcreate pthread2 failed.\n");  //打印错误信息
        return 0;
    }

    while(1) {
        sleep(10);
    }

	return 0;
}
