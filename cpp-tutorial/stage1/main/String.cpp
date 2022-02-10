#include "String.h"
#include <string.h>
using namespace std;

allocator<char> String::alloc_;
pair<char*,char*> String::allocate_n_copy(const char* b,const char* e)
{
	auto data = alloc_.allocate(e-b);
	return {data,uninitialized_copy(b,e,data)};
}
String::String(const char* s)
{
	size_t sz = strlen(s);
	auto data = allocate_n_copy(s,s+sz);
	element = data.first;
	cap = first_free = data.second;
}
String::String(const String& rhs)
{
	auto data = allocate_n_copy(rhs.begin(),rhs.end());
	element = data.first;
	cap = first_free = data.second;
}
String& String::operator=(const String& rhs)
{
	auto data = allocate_n_copy(rhs.begin(),rhs.end());
	free();
	element = data.first;
	cap = first_free = data.second;
	return *this;
}
String& String::operator=(String&& rhs) noexcept
{
	if(this!=&rhs)	
	{
		free();// Will free() throw out exception?? 
		element = rhs.element;
		first_free = rhs.first_free;
		cap = rhs.cap;
		rhs.element = rhs.first_free = rhs.cap = nullptr;
	}
	return *this;
}
String::String(String&& rhs) noexcept
{
	element = rhs.element;
	first_free = rhs.first_free;
	cap = rhs.cap;
	rhs.element = rhs.first_free = rhs.cap = nullptr;
}
void String::push_back(char ch)
{
	check_n_copy();
	*first_free++ = ch;	
	return;
}
void String::check_n_copy()
{
	if(size()==capacity())
		reallocate();
	return;
}
void String::free()
{
	if(element)
	{
	//	for(char* ptr=first_free;ptr!=element;)	
			//alloc_.destroy(*(--ptr));	
		alloc_.deallocate(element,cap-element);
	}
	first_free = element = cap = nullptr;
}

void String::reallocate()
{
	size_t new_sz = ((size()==0)?1:2*size());
	char* new_element = alloc_.allocate(new_sz);
	char* new_ptr = new_element;
	for(char* ptr= element;ptr!=first_free;ptr++)
		alloc_.construct(new_ptr++,std::move(*ptr));	
	free();
	element = new_element;
	first_free = new_ptr;
	cap = element + new_sz;
	return;
}
