
#include <string>
#include <iostream>

class HasPtr{
	public:
		HasPtr(const std::string &s= std::string()):ps(new std::string(s)),i(0) {}
		HasPtr(const HasPtr& hasptr);
	private:
		std::string *ps;
		int           i;
};

HasPtr::HasPtr(const HasPtr& hasptr)
{
	this->i = hasptr.i;
	this->ps = new std::string( *(hasptr.ps) );
}

int main()
{
	HasPtr hp("wuhao");
	std::cout<< *(hp.ps) << std::endl;
	return 0;
}