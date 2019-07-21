
#include <string>
#include <iostream>

class HasPtr{
	public:
		HasPtr(const std::string &s= std::string()):ps(new std::string(s)),i(0) {}
		HasPtr(const HasPtr& hasptr);
		HasPtr& operator=(const HasPtr& rhs);
		~HasPtr() {delete ps;}
	private:
		std::string *ps;
		int           i;
};

HasPtr::HasPtr(const HasPtr& hasptr)
{
	i = hasptr.i;
	ps = new std::string( *(hasptr.ps) );
}

HasPtr& HasPtr::operator=(const HasPtr& rhs)
{
		std::string *newps = new std::string(*(rhs.ps));
		delete ps;
		ps = newps;
		i = rhs.i;
	return *this;
}

int main()
{
	HasPtr hp("wuhao");
	std::cout<< *(hp.ps) << std::endl;
	return 0;
}