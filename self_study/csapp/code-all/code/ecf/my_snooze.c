#include "csapp.h"
void sigint_handler(int sig)
{
	printf("Ctr + c Cached!\n");
	return;
	//exit(0);	
}
int main(int argc,char* argv[])
{
	if(argc < 2)
		return -1;
	printf("%s, %s \n",argv[0],argv[1]);
	if(argv[1]==NULL)
	{
		printf("No time input!\n");	
		exit(-1);
	}
	if(signal(SIGINT,sigint_handler)==SIG_ERR)
		unix_error("signal error");
	char* ptr = argv[1];
	printf("length of digits: %ld\n",strlen(ptr));
	int sleep_time = 0;
	while(*ptr != '\0')
		sleep_time = sleep_time*10 + ((*ptr++)-'0');	
	int left_time = sleep(sleep_time);
	printf("Slept for %d of %d\n",sleep_time-left_time,sleep_time);
	return 0;

}
