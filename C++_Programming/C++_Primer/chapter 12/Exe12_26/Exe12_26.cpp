#include <iostream>
#include <memory>
using namespace std;
int main()
{
	allocator<string> alloc;
	int n = 20;
	string * const p = alloc.allocate(n);
	string *q = p;
	for(int i=0;i<n;i++)
	{
		string s;
		cin >> s;
		alloc.construct(q++,s);
	}

	while(q != p)
	{
		alloc.destroy(--q);
	}

	alloc.deallocate(p,n);
	return 0;
}