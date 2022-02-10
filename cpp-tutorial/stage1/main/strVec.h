#include <string>
#include <initializer_list>
#include <memory>
#include <algorithm>
using namespace std;

class strVec {
	public:
		strVec():element(nullptr),first_free(nullptr),cap(nullptr){}
		strVec(const strVec& rhs);
		strVec(const initializer_list<string>& il);
		strVec(strVec&& rhs) noexcept;
		strVec& operator=(strVec&& rhs) noexcept;
		strVec& operator=(const strVec& rhs);
		size_t size() const { return first_free-element;}
		bool   empty() const { return element == first_free;}
		void push_back(const string& str);
		size_t capacity() const {return cap - element;}
		string* begin() const {  return element;}
		string* end() const {return first_free;}
		void resize(size_t n,const string& s="");
		void reserve(size_t n);
		~strVec() { free();}
	private:
		static allocator<string> alloc_;
		string *element,*first_free,*cap;
		void check_n_copy() {
			if(size()==capacity())
				reallocate();
		}
		void free();
		pair<string*,string*> alloc_n_copy(const string* b,const string* e);// what kind of input parameters need
		void reallocate();
};


