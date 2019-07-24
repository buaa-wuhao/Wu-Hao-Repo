#include <string>
#include <memory>
#include <utility>
#include <initializer_list>
#include <algorithm>

class strVec
{

public:
	strVec():element(nullptr),firstFree(nullptr),cap(nullptr) {}
	strVec(const strVec&);
	strVec(std::initializer_list<std::string> list);
	strVec& operator=(const strVec&);
	std::string* begin() const {return element;}
	std::string* end()   const {return firstFree;}
	~strVec() {free();}
	void push_back(std::string str);
	size_t size() const {return (firstFree - element);}
	size_t capacity() const {return (cap - element);}
	void  reserve(size_t n);
	void  resize(size_t n,const std::string& val);
	std::string& at(size_t i) {return *(element+i);}

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