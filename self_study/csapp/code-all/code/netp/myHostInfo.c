#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#define BUFFER_SIZE 254
int my_open_listenfd(char const* port){
	struct addrinfo hints,*ip_list=NULL,*ptr=NULL;
	int err_code,listenfd=-1,optval=1;
	memset(&hints,0,sizeof(struct addrinfo));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_ADDRCONFIG | AI_NUMERICSERV;

	if((err_code = getaddrinfo(NULL,port,&hints,&ip_list))){
		fprintf(stderr,"%s \n",gai_strerror(err_code));
	}
	for(ptr=ip_list;ptr;ptr=ptr->ai_next){
		listenfd = socket(ptr->ai_family,ptr->ai_socktype,ptr->ai_protocol);
		if(listenfd < 0)
			continue;
		setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,(const void*)&optval,sizeof(int));
		if(bind(listenfd,ptr->ai_addr,ptr->ai_addrlen)<0)
		{
			if(close(listenfd)<0)
			{
				fprintf(stderr,"close listenfd failed: %s \n",strerror(errno));
				return -1;
			}
			continue;
		}
		break;
	}
	freeaddrinfo(ip_list);
	if(ptr==NULL) 
		return -1;
	if(listen(listenfd,1024)<0)
	{
		close(listenfd);
		return -1;
	}
	return listenfd;
}
int my_open_clientfd(const char* host,const char* port){
	struct addrinfo hints,*ip_list=NULL,*ptr=NULL;
	int err_code,clientfd=-1;
	memset(&hints,0,sizeof(struct addrinfo));
	
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_ADDRCONFIG | AI_NUMERICSERV;

	if((err_code = getaddrinfo(host,port,&hints,&ip_list)) != 0){
		fprintf(stderr,"%s\n",gai_strerror(err_code));	
		return clientfd;
	}
	
	for( ptr=ip_list; ptr; ptr=ptr->ai_next){
		clientfd = socket(ptr->ai_family,ptr->ai_socktype,ptr->ai_protocol);	
		if(clientfd==-1)
			continue;
		err_code = connect(clientfd,ptr->ai_addr,ptr->ai_addrlen);
		if(err_code==0)
			break;
		if(close(clientfd)<0){
			fprintf(stderr,"Close socket failed! %s \n",strerror(errno));
		}
	}
	freeaddrinfo(ip_list);
	if(!ptr)	
		return -1;
	return clientfd;	
}

int main(int argc,char* arg[]){
	if(argc < 2){
		fprintf(stderr,"Not enough input parameters!\n");	
		exit(0);
	}		
	struct addrinfo hints,*ip_list,*ptr;
	char ip_host[BUFFER_SIZE],ip_service[BUFFER_SIZE];	
	char* serv = "http";
	memset(&hints,0,sizeof(struct addrinfo));
	hints.ai_family = AF_INET; // IPv4 address only
	hints.ai_socktype = SOCK_STREAM; 
	hints.ai_flags = (AI_NUMERICSERV | AI_ADDRCONFIG);
	int err_code;
	if((err_code = getaddrinfo(NULL,arg[1],&hints,&ip_list)) != 0){
		fprintf(stderr,"%s\n",gai_strerror(err_code));	
		exit(1);
	}
	for(ptr=ip_list;ptr;ptr=ptr->ai_next){
		err_code = getnameinfo(ptr->ai_addr,ptr->ai_addrlen,ip_host,
				BUFFER_SIZE,ip_service,BUFFER_SIZE,NI_NUMERICHOST|NI_NUMERICSERV);	
		if(err_code){
			fprintf(stderr,"%s\n",gai_strerror(err_code));
		}
		printf("%s : %s\n",ip_host,ip_service);
	}
	freeaddrinfo(ip_list);
	return 0;
}
