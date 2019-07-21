#include <string>
using namespace std;

class Employee{
public:
	Employee()
	{
		eid = num++;
	}
	Employee(string& na)
	{
		name = na;
		eid  = num++;
	}
private:
	string name;
	unsigned eid;
	static unsigned num;
};
unsigned Employee::num = 0;
int main()
{

	return 0;
}