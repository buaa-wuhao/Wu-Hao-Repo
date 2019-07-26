
#include <string>
#include <iostream>
#include <algorithm>

class HasPtr{
	friend void swap(HasPtr& rhs,HasPtr& lhs);
	public:
		HasPtr(const std::string &s= std::string()):ps(new std::string(s)),i(0) {}
		HasPtr(const HasPtr& hasptr);
		HasPtr(HasPtr&& hasptr);
		HasPtr& operator=(const HasPtr& rhs);
		HasPtr& operator=(HasPtr&& rhs) noexcept;
		//HasPtr& operator=(HasPtr rhs);
		~HasPtr();
	private:
		std::string *ps;
		int           i;
};

HasPtr::~HasPtr()
{
	delete ps;
}

HasPtr::HasPtr(HasPtr&& hasptr)
{
	ps = hasptr.ps;
	i  = hasptr.i;
	hasptr.ps = nullptr;
	std::cout<<"HasPtr::HasPtr(HasPtr&& hasptr)"<<std::endl;
}

HasPtr::HasPtr(const HasPtr& hasptr)
{
	i  = hasptr.i;
	ps = hasptr.ps;
	std::cout<<"HasPtr::HasPtr(const HasPtr& hasptr)"<<std::endl;
}

HasPtr& HasPtr::operator=(HasPtr&& rhs) noexcept
{
	delete ps;
	ps = rhs.ps;
	i  = rhs.i;
	rhs.ps = nullptr;
	std::cout<<"HasPtr& HasPtr::operator=(HasPtr&& rhs) noexcept"<<std::endl;	
}

void swap(HasPtr& lhs,HasPtr& rhs)
{
	using std::swap;
	swap(lhs.i,rhs.i);
	swap(lhs.ps,rhs.ps);
}

// HasPtr& HasPtr::operator=(HasPtr rhs)
// {
// 	//using std::swap;
// 	swap(*this,rhs);
// 	std::cout<<"HasPtr& HasPtr::operator=(HasPtr rhs)"<<std::endl;
// 	return *this;
// }

HasPtr& HasPtr::operator=(const HasPtr& rhs) 
{		
	i  = rhs.i;
	ps = rhs.ps;
	std::cout<<"HasPtr& HasPtr::operator=(const HasPtr& rhs) "<<std::endl;
	return *this;
}

int main()
{
	HasPtr hp("wuhao"),hp1;
	//std::cout<< *(hp.ps) << std::endl;
	hp1 = hp;
	hp1 = std::move(hp);
	return 0;
}