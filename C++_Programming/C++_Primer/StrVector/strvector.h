#ifndef _STR_VECTOR_H_
#define _STR_VECTOR_H_

#include<string>

class strvector{
	public:
		strvector():element(nullptr),first_free(nullptr),cap(nullptr){}
		strvector(const strvector& );
		strvector(initializer_list<string>);
		strvector& operator=(const strvector&);
		void push_back(const string&);
		size_t size() const {return first_free - element;}
		size_t capacity const {return cap - element;}
		std::string* begin() const {return element;}
		std::string* end()  const  {return first_free;}
		void resize(size_t sz);
		void reserve(size_t sz);
		~strvector();
		

	private:
		static std::allocator<std::string> alloc;
		std::string*           element;
		std::string*           first_free;
		std::string*           cap;
		void free();
		void check_n_alloc();
		void reallocate();
		pair<string*,string*> alloc_n_copy(const string* b, const string* e);

};

 





#endif /*_STR_VECTOR_H_*/