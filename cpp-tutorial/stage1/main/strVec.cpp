#include "strVec.h"

allocator<string> strVec::alloc_;

strVec::strVec(strVec&& rhs) noexcept:element(rhs.element),first_free(rhs.first_free),cap(rhs.cap)
{
	rhs.element = rhs.first_free = rhs.cap = nullptr;	
}
strVec::strVec(const strVec& rhs)
{
	pair<string*,string*> b_e = alloc_n_copy(rhs.element,rhs.first_free);
	element = b_e.first;
	cap = first_free = b_e.second;
}
strVec::strVec(const initializer_list<string>& il)
{
	reserve(il.size());
	for(auto& it:il)	
		alloc_.construct(first_free++,it);		
}
strVec& strVec::operator=(const strVec& rhs)
{
	auto data = alloc_n_copy(rhs.element,rhs.first_free);
	free();
	element = data.first;
	cap = first_free = data.second;
	return *this;
}
strVec& strVec::operator=(strVec&& rhs) noexcept
{
	if(this!=&rhs)
	{
		free();
		element = rhs.element;
		first_free = rhs.first_free;
		cap = rhs.cap;
		rhs.element = rhs.first_free = rhs.cap = nullptr;	
	}
	return *this;
}

pair<string*,string*> strVec::alloc_n_copy(const string* b,const string* e)
{
	string* this_b = alloc_.allocate(e - b);
	return {this_b,uninitialized_copy(b,e,this_b)};
}

void strVec::push_back(const string& str)
{
	check_n_copy();
	alloc_.construct(first_free++,str);	
	return;
}

void strVec::reallocate()
{
	size_t new_sz = (size()==0)?1:size()*2;
	string* new_b = alloc_.allocate(new_sz);
	string* new_p = new_b;
	for(string* p=element;p!=first_free;)
		alloc_.construct(new_p++,std::move(*p++));
	free();
	element = new_b;
	first_free = new_p;
	cap = new_b+new_sz;
}
void strVec::resize(size_t n,const string& s)
{
	if(n < size())
	{
		for(string* p=element+n;p!=first_free;)
			alloc_.destroy(p++);
		first_free = element + n;
		return;
	}
	reserve(n);

	for(string* p = first_free;p!=element+n;)	
		alloc_.construct(p++,s);

	return;
}
void strVec::reserve(size_t n)
{
	if(n<=capacity()) return;
	string* data = alloc_.allocate(n);
	string* ptr_new = data;
	for(string* ptr_old = element;ptr_old!=first_free;)
		alloc_.construct(ptr_new++,std::move(*ptr_old++));
	free();
	element = data;
	first_free = ptr_new;
	cap = element + n;
	return;
}
void strVec::free()
{
	if(element)
	{
	//for(auto p=first_free;p!=element;)	
	//{
	//	alloc_.destroy(--p);	
	//}
		for_each(element,first_free,[](string& p){alloc_.destroy(&p);});
		alloc_.deallocate(element,cap-element);
	}
}




