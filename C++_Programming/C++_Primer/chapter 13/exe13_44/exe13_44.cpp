#include <algorithm>
#include <memory>
#include <string.h>
#include <utility>
#include <vector>
#include <iostream>

class String{
	public:
		String():element(nullptr),firstFree(nullptr),cap(nullptr) {}
		String(const char*);
		String(const String&);
		String& operator=(const String&) ;
		~String() {free();}
		size_t size() const {return (firstFree-element);}
		char* begin() const {return element;}
		char* end()   const {return firstFree;}

	private:
		static std::allocator<char> alloc;
		char*  element;
		char*  firstFree;
		char*  cap;
		bool   isFull();
		std::pair<char*,char*>   copy_n(char* begin,char* end);
		void free();

};

std::allocator<char> String::alloc;

String::String(const String& str)
{
	std::cout << "Copy ...." << std::endl;
	std::pair<char*,char*> pr = copy_n(str.begin(),str.end());
	element = pr.first;
	cap = firstFree = pr.second;
	std::cout << "Copy" << std::endl;
}


String& String::operator=(const String& str)
{
	std::pair<char*,char*> pr = copy_n(str.begin(),str.end());
	char* new_element = pr.first;
	char* new_firstFree = pr.second;
	char* new_cap = firstFree;
	free();
	element = new_element;
	firstFree = new_firstFree;
	cap       = new_cap;
	std::cout<<"assign " << std::endl;
	return (*this);
}

String::String(const char* str)
{
	size_t sz = strlen(str);
	element = alloc.allocate(sz);
	firstFree = element;
	for(int i=0;i<sz;i++)
	{
		alloc.construct(firstFree++,*str++);
	}
	cap = firstFree;
}

void String::free()
{
	std::for_each(element,firstFree,[](char& ch){alloc.destroy(&ch);});
	alloc.deallocate(element,cap-element);
	return;
}

std::pair<char*,char*> String::copy_n(char* begin,char* end)
{
	size_t sz = end - begin;
	char*  new_element = alloc.allocate(sz);
	char*  new_firstFree   = new_element;
	char*  cur = begin;
	for(size_t i=0;i < sz; i++)
	{
		alloc.construct(new_firstFree++,*cur++);
	}
	return {new_element,new_firstFree};
}

int main()
{
	String s("wuhao");
	std::vector<String> vv;
	std::cout<<"test" << std::endl;
	for(int i=0;i<10;i++)
	{
		vv.push_back(s);
	}
	
	return 0;
}
