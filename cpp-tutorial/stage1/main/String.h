#include <memory>
#include <utility>

class String{
	public:
		String():element(nullptr),first_free(nullptr),cap(nullptr){}
		String(const char* s="");
		String(const String& rhs);
		String(String&& rhs) noexcept;
		String& operator=(const String& rhs);
		String& operator=(String&& rhs) noexcept;
		~String(){free();}
		size_t size() const {return first_free-element;}
		size_t capacity() const {return cap-element;}
		bool empty() const {return element==first_free;}
		void push_back(char ch);
		char* begin() const { return element;}
		char* end() const {return first_free;}
	private:
		static std::allocator<char> alloc_;
		char* element;
		char* first_free;
		char* cap;
		void free();
		void check_n_copy();
		std::pair<char*,char*> allocate_n_copy(const char* b,const char* e);
		void reallocate();
};
