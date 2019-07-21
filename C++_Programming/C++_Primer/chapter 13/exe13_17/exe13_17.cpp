#include <iostream>
#include <string>
#include <cstdlib>
#include <time.h>
using namespace std;

class numbered
{
	public:
	numbered() {
		mysn = rand();}
	numbered(const numbered& num) {mysn = rand();}

	unsigned mysn;

};

void f(const numbered& s)
{
	cout << s.mysn << endl;
}

int main()
{
	srand(time(NULL));
	numbered a, b =a ,c=b,d,e;
	f(a); cout<< " a: " << a.mysn << endl;
	f(b); cout<< " b: " << b.mysn << endl;
	f(c); cout<< " c: " << c.mysn << endl;
	//f(d);f(e);

	return 0;
}