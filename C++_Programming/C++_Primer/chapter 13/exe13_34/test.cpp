#include <set>
#include <iostream>
#include <string>
using namespace std;

struct pos{
	string* ptr;
	int x;
	int y;
};
int main()
{
	set<pos*> ints;
	pos array[5] = {{0,0,1},{0,1,2},{0,2,3},{0,3,4},{0,4,5}};
	
	for(int i=0;i<5;i++)
	{
		ints.insert(array+i);
	}
	for(auto it:ints)
	{
		cout<<it->x <<" "<< it->y<< endl;
	}
	return 0;
}

