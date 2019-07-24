#include "exe13_39.hpp"


std::allocator<std::string> strVec::alloc;

void  strVec::resize(size_t n,const std::string& val)
{
	if(n==size()) return;
	
	if(n < size())
	{
		for(; firstFree!=element+n;)
		{
			alloc.destroy(--firstFree);
		}
		return;
	}
	
	if(n > capacity()) reserve(n);
	
	for(;firstFree!=element+n;)
	{	
		alloc.construct(firstFree++,val);
	}
	return;
}

void strVec::reserve(size_t n)
{
	if(n <= capacity()) return;
	std::string* new_element = alloc.allocate(n);
	std::string* new_firstFree = new_element;
	std::string* ptr = element;
	for(int i=0;i<size();i++)
	{
		alloc.construct(new_firstFree++,std::move(*ptr++));
	}
	free();
	element = new_element;
	firstFree = new_firstFree;
	cap = element + n;
	return;
}

std::pair<std::string*,std::string*> strVec::copy_n_chk(const strVec& strvec)
{
	size_t sz = size();
	element = alloc.allocate(sz);
	std::uninitialized_copy_n(strvec.element,sz,element);
	return std::make_pair(element,element+sz);
} 

strVec::strVec(std::initializer_list<std::string> list)
{
	size_t sz = list.size();
	firstFree = element = alloc.allocate(sz);
	for(auto it:list)
	{
		alloc.construct(firstFree++,it);
	}
	cap = firstFree;
}

strVec::strVec(const strVec& strvec)
{
	std::pair<std::string*,std::string*> pr = copy_n_chk(strvec);
	element = pr.first;
	cap = firstFree = pr.second;
}

strVec& strVec::operator=(const strVec& strvec)
{
	std::pair<std::string*,std::string*> pr = copy_n_chk(strvec);
	free();
	element = pr.first;
	cap = firstFree = pr.second;
	return (*this);
}

void strVec::free()
{
	if(!element) return;
	// for(std::string *ptr=firstFree; ptr!=element ;)
	// {
	// 	alloc.destroy(--ptr);
	// }
	std::for_each(element,firstFree,[](std::string& p){alloc.destroy(&p);});
	alloc.deallocate(element,cap-element);
}

void strVec::push_back(std::string str)
{
	if(isFull()) reallocate();
	alloc.construct(firstFree++,str);
	return;
}

void strVec::reallocate()
{
	size_t new_sz = (size()==0)?1:2*size();
	std::string* new_element = alloc.allocate(new_sz);
	std::string* new_firstFree = new_element;
	std::string* cur = element;
	for(int i=0;i<size();i++)
	{
		alloc.construct(new_firstFree++,std::move(*cur++));
	}

	free();
	element = new_element;
	firstFree = new_firstFree;
	cap       = new_element+new_sz;
}


// int main()
// {
// 	strVec s;
// 	return 0;
// }