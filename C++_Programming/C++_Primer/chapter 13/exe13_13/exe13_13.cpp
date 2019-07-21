#include <iostream>
#include <vector>
using namespace std;

struct X
{
	X() {cout<<"X()"<<endl;}
	X(const X&) {cout<<"X(const X& )"<<endl;}
	X& operator=(const X& ) {cout<<"X& operator=(const X& )"<<endl;}
	~X() {cout<<"~X()"<<endl;}
};

void passNonRef(X x)
{
	cout<<"void passNonRef(X x)"<<endl;
}

void passRef(X& x)
{
	cout<<"void passRef(X& x)"<<endl;
}

int main()
{
	X x;
	cout<<"1 ****************"<<endl;
	vector<X> array_x(10);
	cout<<"2 ****************"<<endl;
	passNonRef(x);
	cout<<"3 ****************"<<endl;
	passRef(x);
	cout<<"4 ****************"<<endl;
	X* p_x = new X;
	cout<<"5 ****************"<<endl;
	X xx = x;
	cout<<"6 ****************"<<endl;
	*p_x = x;
	cout<<"7 ****************"<<endl;
	array_x.push_back(x);
	cout<<"7.5 ****************"<<endl;
	array_x.push_back(x);
    cout<<"8 ****************"<<endl;
    delete p_x;
    cout<<"9 ****************"<<endl;
    cout<<"return 0" <<endl;
	return 0;
}