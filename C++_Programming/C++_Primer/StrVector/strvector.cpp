#include <string>
#include <memory>
#include "strvector.h"
using namespace std;

allocator<string> strvector::alloc;


void strvector::resize(size_t sz)
{
	if(sz==0)
	{
		free();
	}
	else if(sz > capacity())
	{
		reallocate();
	}
	else if(sz != size())
	{
		auto it = end();		

		for(int cnt = size() - sz;cnt>0;cnt--)
		{
			if(sz < size()) {alloc.destroy(--it);}
			else  
			{
				string s;
				alloc.construct(it++,s);
			}
		}
		first_free = it;
	}
}


strvector& strvector::operator=(const strvector& strv)
{
	auto data = alloc_n_copy(strv.begin(),strv.end());
	free();
	element   = data.first;
	first_free = cap = data.second;
	return *this;
}

strvector::~strvector()
{
	free();
}

strvector::strvector(const strvector& s)
{
	auto data = alloc_n_copy(s.begin(),s.end());
	element = data.first;
	first_free = cap = data.second;
}

pair<string*,string*> strvector::alloc_n_copy(const string* b, const string* e)
{
	auto data = alloc.allocate(e - b);
	return {data, uninitialized_copy(b,e,data)};
}

void strvector::reallocate()
{
	int sz = capacity();
	auto data = alloc.allocate(2*sz);
	uninitialized_copy(begin(),end(),data);
	free();
	element = data.first;
	first_free = data.second;
	cap = element + 2*sz;
}

void strvector::check_n_alloc()
{
	if(size()==capacity()) {reallocate();}
}

void strvector::push_back(const string& s)
{
	check_n_alloc();
	alloc.construct(first_free++,s);
}

void free()
{
	if(element)
	{	
		for(auto it=first_free;it!=element;)
		{
			alloc.destroy(--it);
		}
		alloc.deallocate(element,cap-element);
	}
}