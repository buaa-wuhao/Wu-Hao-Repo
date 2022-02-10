#include <functional>

struct debugDelete{
	template<typename T>
	void operator()(T* ptr) const{
		delete ptr;	
	}
};

template<typename T>
class SharedPtr;

template<typename T>
inline void swap(SharedPtr<T>& lhs, SharedPtr<T>& rhs)
{
	using std::swap;
	swap(lhs.raw_ptr,rhs.raw_ptr);
	swap(lhs.cnt,rhs.cnt);	
	swap(lhs.del,rhs.del);
	return;
}
template<typename T>
class SharedPtr{
	//friend void swap(SharedPtr<T>& lhs,SharedPtr<T>& rhs);
	template<typename F>
	friend void swap(SharedPtr<F>& lhs,SharedPtr<F>& rhs);
	public:
		//should we have const T* p???
		explicit SharedPtr(T* p):raw_ptr(p),cnt(new size_t(1)),del(debugDelete()){}
		SharedPtr(T* p,std::function<void(T*)>& d):raw_ptr(p),cnt(new size_t(1)),del(d){}
	  	//return SharedPtr& or SharedPtr?,copy constructor no return !!! do we have no except?
		SharedPtr(const SharedPtr& other):raw_ptr(other.raw_ptr),cnt(other.cnt),del(other.del){
			(*cnt)++;	
		}	
		//
		SharedPtr(SharedPtr&& other) noexcept:raw_ptr(other.raw_ptr),cnt(other.cnt),del(std::move(other.del)) {
			other.raw_ptr = nullptr;
			other.cnt = nullptr;	
		}
		SharedPtr& operator=(SharedPtr&& other) noexcept{
			raw_ptr = other.raw_ptr;
			cnt = other.cnt;
			del = other.del;
			raw_ptr = nullptr;
			cnt = nullptr;		
		}

		SharedPtr& operator=(const SharedPtr& other){
			if(this!=&other){
				free();
				raw_ptr = other.raw_ptr;
				cnt = other.cnt;
				del = other.del;	
			}
			return *this;
		}
		void reset(){
			free();	
		}
		void reset(T* p){
			reset(p,debugDelete());	
		}
		void reset(T* p, const std::function<void(T*)>& d)
		{
			if(p!=raw_ptr)	
			{
				free();
				raw_ptr = p;
				cnt = new size_t(1);
			}
			del =  d;
			return;
		}
		T& operator*() const{
			return *raw_ptr;
		}
		T* operator->() const{
			return &this->operator*();	
		}

		operator bool() const{
			return raw_ptr!=nullptr;	
		}
		size_t use_cnt() const{
			return (*cnt);	
		}

		~SharedPtr(){
			free();	
		}


	private:
		T*       raw_ptr;
		size_t*  cnt;
		std::function<void(T*)> del;
		void free(){
			if(--(*cnt) == 0)
			{
				del(raw_ptr);	
				raw_ptr = nullptr;
				delete cnt;
				cnt = nullptr;
			}
		}

};
