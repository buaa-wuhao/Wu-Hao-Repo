#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;
class strRange{
	public:
	strRange(size_t l,size_t u):lower(l),upper(u) {}
	bool operator()(const string& str) const
	{
		return (str.size() >= lower) && (str.size() <= upper);
	}
	private:
		size_t lower;
		size_t upper;

};

int main(int argc,char* argv[])
{
	strRange strr(1,9),strr2(10,0-1);
	string file(argv[1]);
	ifstream in(file);
	
	if(!in.is_open())
	{
		cerr<<"cant open file"<<endl;
		return -1;
	}
	int count = 0,count2 = 0;
	for(string str; in >> str; )
	{
		if(strr(str))   count++;
		if(strr2(str))  count2++;
	}
	cout<< count << " " << count2 << endl;
	return 0;
}
