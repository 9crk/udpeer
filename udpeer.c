#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

static int listenPort(int port)
{
    int ret,server_fd;
    struct sockaddr_in ser_addr; 
    server_fd = socket(AF_INET, SOCK_DGRAM, 0); //AF_INET:IPV4;SOCK_DGRAM:UDP
    if(server_fd < 0){
        printf("create socket fail!\n");return -1;
    }
    memset(&ser_addr, 0, sizeof(ser_addr));
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_addr.s_addr = htonl(INADDR_ANY); //IP地址，需要进行网络序转换，INADDR_ANY：本地地址
    ser_addr.sin_port = htons(port);
    ret = bind(server_fd, (struct sockaddr*)&ser_addr, sizeof(ser_addr));
    if(ret < 0)
    {
        printf("socket bind fail!\n");
        return -1;
    }else{
	printf("listen %d ok!\n",port);	
    }
    return server_fd;
}
#define BUFF_LEN	1024
int main(int argc, char* argv[])
{
    int ret,f1,f2;
    int f1Inited = 0;
    int f2Inited = 0;
    char buf[BUFF_LEN];
    f1=listenPort(atoi(argv[1]));
    f2=listenPort(atoi(argv[2]));
    
    socklen_t len;
    struct sockaddr_in clent_addr1,clent_addr2;
    fd_set fds;
    struct timeval timeout;
    int maxfd;
    len = sizeof(clent_addr1);
    maxfd = (f2>f1?f2:f1)+1;
    while(1){
        timeout.tv_usec = 0;
        timeout.tv_sec = 1;
        FD_ZERO(&fds);
        FD_SET(f1, &fds);
        FD_SET(f2, &fds);
	ret = select(maxfd, &fds, NULL, NULL, &timeout);
	if(ret < 0){printf("error\n");break;}
	if(ret == 0){continue;}
    	memset(buf,0,BUFF_LEN);
	if(FD_ISSET(f1, &fds)){
		ret = recvfrom(f1, buf, BUFF_LEN, 0, (struct sockaddr*)&clent_addr1, &len);  
		//printf("recv1-%d:%s\n",ret,buf);
		if(ret > 0)f1Inited = 1;
		if(ret > 0 && f2Inited){
			sendto(f2, buf, BUFF_LEN, 0, (struct sockaddr*)&clent_addr2, len);
		}
	}else if(FD_ISSET(f2, &fds)){
    		ret = recvfrom(f2, buf, BUFF_LEN, 0, (struct sockaddr*)&clent_addr2, &len);  
		//printf("recv2-%d:%s\n",ret,buf);
		if(ret > 0)f2Inited = 1;
		if(ret > 0 && f1Inited){
			sendto(f1, buf, BUFF_LEN, 0, (struct sockaddr*)&clent_addr1, len);
		}		
	}
    }
    close(f1);
    close(f2);
    return 0;
}
