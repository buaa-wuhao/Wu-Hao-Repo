#include "csapp.h"

void Mmapcopy(int fd,int sz)
{
	char* buf;
	buf = Mmap(NULL,sz,PROT_READ | PROT_WRITE,MAP_SHARED,fd,0);
	Write(1,buf,sz);
	buf[0] = 'H';
	//Write(1,buf,sz);
	//Munmap(buf,sz);
	exit(0);
}


int main(int argc, char* argv[])
{
	struct stat stat;
	int fd;
	if(argc<2)
	{
		printf("usage %s <file name>\n",argv[0]);
		exit(0);	
	}

	fd = Open(argv[1],O_RDWR,0);
	fstat(fd,&stat);
	Mmapcopy(fd,stat.st_size);
	exit(0);
}
