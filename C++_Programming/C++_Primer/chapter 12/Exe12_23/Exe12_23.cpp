#include<string.h>
//#include<stdio.h>
#include<iostream>
//using namespace std;
int main()
{
	const char* s1 = "hello ";
	const char* s2 = "world";
	char* s = new char[strlen(s1)+strlen(s2)+1];
	strcpy(s,s1);
	strcat(s,s2);
	std::cout<<s<<std::endl;
	return 0;
}