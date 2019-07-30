#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

class isEqual{
public:
	isEqual(int i):val(i) {}
	bool operator()(int a)
	{
		return a==val;
	}
private:
	int val;
};

int main()
{
	vector<int> v = {3,2,1,4,3,4,8,6};
	replace_if(v.begin(),v.end(),isEqual(3),5);
	for(int i:v) cout<<i<<" ";
	cout<<endl;
	return 0;
}

