
#include <iostream>
#include <functional>
#include <string>
#include <map>

int add(int a,int b) {return (a+b);}

auto mod = [](int a,int b) {return a % b;};

struct divide{
	int operator()(int a,int b) const
	{
		return a / b;
	}
};


auto binop = std::map<std::string, std::function<int(int,int)> >
{
	{"+",add},
	{"-",std::minus<int>()},
	{"*",std::multiplies<int>()},
	{"/",divide()},
	{"%",mod}
};

int main()
{
	while(std::cout<<"num operator num",true)
	{
		int lhs,rhs;
		std::string opt;
		std::cin>>lhs>>opt>>rhs;
		std::cout <<  binop[opt](lhs,rhs)  << std::endl;
	}
	return 0;
}