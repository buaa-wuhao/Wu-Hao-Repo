#include <string>
#include <iostream> 
#include <vector>
using namespace std;

class PrintString{
	public:
		PrintString(istream& in = cin):is(in) {}
		string operator()() const
		{
			string ss;
			getline(is,ss);
			return (is? ss:"");
		}
	private:
		istream& is;
};

int main()
{
	PrintString ps(cin);
	vector<string> vs;
	string str;
	while( (str = ps()) != "q")
	{
		vs.push_back(str);
	}
	//cout<<ps()<<endl;
	for(auto it:vs) cout<<it<<endl;
	return 0;
}