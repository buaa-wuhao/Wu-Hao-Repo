#include <string>
#include <memory>
#include "strvector.h"
using namespace std;

allocator<string> strvector::alloc;

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