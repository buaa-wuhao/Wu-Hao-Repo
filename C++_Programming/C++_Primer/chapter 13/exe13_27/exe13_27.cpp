
#include <string>
#include <iostream>

class HasPtr{
	public:
		HasPtr(const std::string &s= std::string()):ps(new std::string(s)),i(0),use(new size_t(0)) {}
		HasPtr(const HasPtr& hasptr);
		HasPtr& operator=(const HasPtr& rhs);
		~HasPtr();
	private:
		std::string *ps;
		int           i;
		size_t        *use;
};

HasPtr::~HasPtr()
{
	if(--*use == 0)
	{
		delete ps;
		delete use;
	}
}

HasPtr::HasPtr(const HasPtr& hasptr)
{
	i  = hasptr.i;
	ps = hasptr.ps;
	use = hasptr.use;
	(*use)++;
}

HasPtr& HasPtr::operator=(const HasPtr& rhs)
{
		(*rhs.use)++;
		if(--(*use)==0)
		{
			delete use;
			delete ps;
		}
		
		i  = rhs.i;
		ps = rhs.ps;
		use = rhs.use;

	return *this;
}

int main()
{
	//HasPtr hp("wuhao");
	//std::cout<< *(hp.ps) << std::endl;
	return 0;
}