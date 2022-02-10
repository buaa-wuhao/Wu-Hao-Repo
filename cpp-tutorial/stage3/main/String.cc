#include "String.h"
#include <string.h>
using namespace std;

allocator<char> String::alloc_;
pair<char*,char*> String::allocate_n_copy(const char* b, const char* e){
	auto data = alloc_.allocate(e-b);	
	return {data,uninitialized_copy(b,e,data)};
}
String::String(const String& other)
{
	auto data = allocate_n_copy(other.element,other.first_free);	
	element = data.first;
	first_free = data.second;
	cap = first_free;
}

String::String(String&& other) noexcept{
	element = other.element;
	first_free = other.first_free;
	cap = other.cap;
	other.first_free = nullptr;
	other.cap = nullptr;
	other.element = nullptr;
}

String& String::operator=(String&& rhs){
	if(&rhs != this){
		free();
		element = rhs.element;
		first_free = rhs.first_free;
		cap = rhs.cap;
		rhs.element = rhs.first_free = rhs.cap = nullptr;
	}
	return *this;
}

String& String::operator=(const String& rhs){
	if(this!=&rhs){
		free();	
		auto data = allocate_n_copy(rhs.element,rhs.first_free);
		element = data.first;
		first_free = data.second;
		cap =  first_free;
	}	
	return *this;
}



String::String(const char* str){
	auto data = allocate_n_copy(str,str+strlen(str));
	element = data.first;
	cap = first_free = data.second;
}

void String::free(){
	if(element)
		alloc_.deallocate(element,cap-element);
	element = first_free = cap = nullptr;
	return;
}

void String::pop_back(){
	if(!empty()){
		first_free--;	
	}
	return;
}

void String::push_back(char ch){
	check_n_copy();
	*first_free++ = ch;
	return;
}

void String::reallocate(){
	size_t old_sz = size();
	size_t new_sz = (old_sz==0)?1:old_sz*2;
	char* new_element = alloc_.allocate(new_sz);
	char* p_new = new_element;
	//uninitialized_copy(element,first_free,new_element);
	for(char* ptr = element;ptr!=first_free;ptr++)
	{
		alloc_.construct(p_new++,move(*ptr));	
	}
	free();
	element = new_element;
	first_free = new_element + old_sz;
	cap = new_element + new_sz;
	return;
}


void String::check_n_copy(){
	if(size()==capacity())
		reallocate();
	return;
}

String::~String(){
	free();	
}
