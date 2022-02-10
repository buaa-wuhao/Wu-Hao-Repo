#include "csapp.h"
sigjmp_buf sig_buf;
int mysystem(const char* command)
{
	int status;
	char* argv[] = {"","-c",command,NULL};	
	pid_t pid;

	if( (pid=Fork())==0 )
  	{	
		if(execve("/bin/sh",argv,environ) < 0)
		{
			unix_error("/bin/sh is not found!");
			exit(0);
		}
	}

	if(waitpid(pid,&status,0)>0)
	{
		if(WIFEXITED(status))	
			return WEXITSTATUS(status);	
	}

	return 0;
}
void sigHandler(int sig)
{
	siglongjmp(sig_buf,1);
}
char* tfgets(char* s,int n, FILE *stream)
{
	if(!sigsetjmp(sig_buf,1))	
	{
		if(signal(SIGALRM,sigHandler)==SIG_ERR)
			unix_error("signal alarm Error");
		alarm(5);
		return fgets(s,n,stream);	
	}
	return NULL;
}
int main(int argc, char* argv[], char* env[])
{
	char buf[100];
	char* input = NULL;
	//char* input = fgets(buf,100,stdin);

	printf("buf at %p\n",buf);
	//printf("%s",buf);
	//char* input = tfgets(buf,100,stdin);
	if(input==NULL)
	{
		printf("Empty String \n");	
	}
	else
	{
		printf("%s \n",input);	
	}

	//int rtn = mysystem("ls -l");
//if(execve("/bin/ls",argv,env)==-1)
//{
//	fprintf(stderr,"execve error %s\n",strerror(errno));
//	exit(1);
//}
	exit(0);
}
