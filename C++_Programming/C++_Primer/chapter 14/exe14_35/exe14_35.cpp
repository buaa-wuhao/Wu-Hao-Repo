#include <string>
#include <iostream> 
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
	cout<<ps()<<endl;
	return 0;
}