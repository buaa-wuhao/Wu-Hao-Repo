#include <iostream>
#include <vector>
#include <memory>
using namespace std;

shared_ptr<vector<int>> rtn()
{
	//vector<int>* ptr = new vector<int>();
	return make_shared<vector<int>>();
}

void readin(shared_ptr<vector<int>> vPtr)
{
	int tmp;
	while(cin>>tmp)
	{
		vPtr->push_back(tmp);
	}
	return;
}

void printout(shared_ptr<vector<int>> vPtr)
{
	if(vPtr->empty()) return;
	for(int i=0;i<vPtr->size();i++)
	{
		cout<< vPtr->at(i) <<" ";
	}
	return;
}

int main()
{
	shared_ptr<vector<int>> vPtr;
	vPtr = rtn();
	readin(vPtr);
	printout(vPtr);
}

