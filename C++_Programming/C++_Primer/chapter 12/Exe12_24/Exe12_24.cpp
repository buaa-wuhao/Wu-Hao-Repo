
#include<iostream>
using namespace std;

int main()
{
	cout<<"Enter Number"<< endl;
	size_t len;
	cin >> len;
	cin.ignore();
	char* s = new char[len+1];
	cin.get(s,len+1);
	cout<<s<<endl;
	delete [] s;
	return 0;
}