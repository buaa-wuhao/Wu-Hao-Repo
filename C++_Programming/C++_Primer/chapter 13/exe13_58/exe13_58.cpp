#include <vector>
#include <algorithm>
#include <iostream>
using namespace std;

class Foo{
public:
	Foo sorted() const &;
	Foo sorted() &&;
private:
	vector<int> data;
};

Foo Foo::sorted() &&
{
	sort(data.begin(),data.end());
	cout<<"Foo Foo::sorted() &&"<< endl;
	return Foo(*this);
}

// Foo Foo::sorted() const & 
// {
// 	Foo ret(*this);
// 	cout<<"Foo Foo::sorted() const & " << endl;
// 	return ret.sorted();
// }

Foo Foo::sorted() const & 
{
	Foo ret(*this);
	return Foo(*this).sorted();
}


int main()
{
	Foo foo;
	foo.sorted();
	return 0;
}
