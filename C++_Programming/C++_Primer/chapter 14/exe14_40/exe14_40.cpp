#include <iostream>
class bigger{
public:
	bool operator()(const string& a,const string& b) const
	{
		return a.size() < b.size();
	}
};

class gequal{
public:
	gequal(size_t sz):size(sz) {}
	bool operator()(const string& str) const
	{
		return str.size() >= sz;
	}
private:
	size;
};

class printout
{
public:
	void operator()(const string& str) const
	{
		std::cout<<str << std::endl;
	}
};

