#include <memory>
#include <utility>
class String{
	public:
		String(const String& other);
		String(const char* str="");
		String(String&& other) noexcept;
		String& operator=(const String& rhs);
		String& operator=(String&& rhs);

		size_t size() const{
			return first_free - element;	
		}
		size_t capacity() const{
			return cap - element;	
		}
		bool empty() const{
			return element==first_free;	
		}
		char* begin() const{
			return element;	
		}
		char* end() const{
			return first_free;	
		}
		void push_back(char c);
		void pop_back();
		//this function should not be called by empty string
		char& back() const{
			return *(first_free-1);		
		}
		//this function should not be called by empty string
		char& front() const{
			return *element;	
		}

		~String();
	private:
		static std::allocator<char> alloc_;
		char*  element; 
		char*  cap;
       		char*  first_free;		
		std::pair<char*,char*> allocate_n_copy(const char* b,const char* e);
		void check_n_copy();
		void reallocate();
		void free();

};
