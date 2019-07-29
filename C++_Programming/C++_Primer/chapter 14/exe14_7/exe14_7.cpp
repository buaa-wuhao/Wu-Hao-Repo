#include <algorithm>
#include <memory>
#include <string.h>
#include <utility>
#include <vector>
#include <iostream>

class String{
	friend std::ostream& operator<<(std::ostream&,const String& rhs);
	public:
		String():element(nullptr),firstFree(nullptr),cap(nullptr) {}
		String(const char*);
		String(const String&);
		String(String&& str) noexcept;
		String& operator=(String&& rhs) noexcept;
		String& operator=(const String&) ;
		~String() {free();}
		size_t size() const {return (firstFree-element);}
		char* begin() const {return element;}
		char* end()   const {return firstFree;}
		const char* c_str() const {return element;}


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

std::ostream& operator<<(std::ostream& os, const String& rhs)
{
	os << rhs.c_str() <<std::endl;
	return os;
}

String::String(String&& str) noexcept
{
	element = str.element;
	firstFree = str.firstFree;
	cap       = cap;
	str.element = str.firstFree = cap = nullptr;
	std::cout<<"String::String(String&& str)"<<std::endl;
}

String& String::operator=(String&& rhs) noexcept
{
	if(this != &rhs)
	{
		free();
		element = rhs.element;
		firstFree = rhs.firstFree;
		cap   = rhs.cap;
		rhs.element = rhs.firstFree = rhs.cap = nullptr;
	}
	std::cout<<"String& String::operator=(String&& rhs)"<<std::endl;
	return *this;
}

String::String(const String& str)
{
	//std::cout << "Copy ...." << std::endl;
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
	std::cout<<"const char*"<<std::endl;
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

void foo(String x)
{
    std::cout << x.c_str() << std::endl;
}

void bar(const String& x)
{
    std::cout << x.c_str();
}

String baz()
{
    String ret("world");
    return ret;
}

int main()
{
char text[] = "world";

    String s0;
    std::cout<<"1111111111111111"<<std::endl;
    String s1("hello World!");
    std::cout<<s1<<std::endl;
//     std::cout<<"2222222222222222"<<std::endl;
//     String s2(s0);
//     std::cout<<"3333333333333333"<<std::endl;
//     String s3 = s1;
// 	std::cout<<"4444444444444444"<<std::endl;
//     String s4(text);
// 	std::cout<<"5555555555555555"<<std::endl;
//     s2 = s1;
//     std::cout<<"6666666666666666"<<std::endl;
//     foo(s1);
//     std::cout<<"7"<<std::endl;
//     bar(s1);
//     std::cout<<"8"<<std::endl;
//     foo("temporary");
//     std::cout<<"9"<<std::endl;    
//     bar("temporary");
//      std::cout<<"10"<<std::endl;
//     String s5 = baz();
//     std::cout<<"11"<<std::endl;
//     std::vector<String> svec;
//      std::cout<<"12"<<std::endl;
//     svec.reserve(8);
// std::cout<<"12"<<std::endl;
//     svec.push_back(s0);
// std::cout<<"13"<<std::endl;
//     svec.push_back(s1);
// std::cout<<"14"<<std::endl;
//     svec.push_back(s2);
// std::cout<<"15"<<std::endl;
//     svec.push_back(s3);
// std::cout<<"16"<<std::endl;
//     svec.push_back(s4);
// std::cout<<"17"<<std::endl;
//     svec.push_back(s5);
// std::cout<<"18"<<std::endl;
//     svec.push_back(baz());
// std::cout<<"19"<<std::endl;
//     svec.push_back("good job");
// std::cout<<"20"<<std::endl;

//     for (const auto &s : svec) {
//         std::cout << s.c_str() << std::endl;
//     }
}
