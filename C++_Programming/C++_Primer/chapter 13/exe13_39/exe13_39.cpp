#include <string>
#include <memory>
#include <utility>

class strVec
{

public:
	strVec():element(nullptr),firstFree(nullptr),cap(nullptr) {}
	strVec(const strVec&);
	strVec& operator=(const strVec&);
	std::string* begin() const {return element;}
	std::string* end()   const {return firstFree;}
	~strVec() {free();}
	void push_back(std::string str);
	size_t size() const {return (firstFree - element);}

private:
	static std::allocator<std::string> alloc;
	std::string* element;
	std::string* firstFree;
	std::string* cap;
	bool isFull() {return (firstFree==cap);}
	void free();
	void reallocate();
	std::pair<std::string*,std::string*> copy_n_chk(const strVec& strvec); 
};


std::allocator<std::string> strVec::alloc;


std::pair<std::string*,std::string*> strVec::copy_n_chk(const strVec& strvec)
{
	size_t sz = size();
	element = alloc.allocate(sz);
	std::uninitialized_copy_n(strvec.element,sz,element);
	return std::make_pair(element,element+sz);
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
	for(std::string *ptr=firstFree; ptr!=element ;)
	{
		alloc.destroy(--ptr);
	}
	alloc.deallocate(element,firstFree-element);
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


int main()
{
	strVec s;
	return 0;
}