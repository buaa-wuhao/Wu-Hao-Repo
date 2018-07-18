#ifndef _STR_VECTOR_H_
#define _STR_VECTOR_H_

#include<string>

class strvector{
	public:
		strvector():element(nullptr),first_free(nullptr),cap(nullptr){}
		strvector(const strvector& strvec);
		strvector& operator=(const strvector& strvec);
		size_t size() const {return first_free - element}
		size_t capacity const {return cap - element}

	private:
		allocator<std::string> alloc;
		std::string*           element;
		std::string*           first_free;
		std::string*           cap;

};







#endif /*_STR_VECTOR_H_*/