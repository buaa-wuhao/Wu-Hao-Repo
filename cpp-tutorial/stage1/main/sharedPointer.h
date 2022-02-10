#include <functional>
template<typename T> class sharedPointer;
template<typename T>
void swap(sharedPointer<T>& lhs,sharedPointer<T>& rhs)
{
	using std::swap;
	swap(lhs.pointer_,rhs.pointer_);
	swap(lhs.ref_count_,rhs.ref_count_);
	swap(lhs.deleter_,rhs.deleter_);
}

template<typename T>
class sharedPointer{
	friend void swap<T>(sharedPointer<T>& lhs,sharedPointer<T>& rhs); 
	public:
		sharedPointer():pointer_(nullptr),ref_count_(new size_t(0)),deleter_(debugDelete()) {}	
		explicit sharedPointer(T* ptr):pointer_(ptr),ref_count_(new size_t(1)),deleter_(debugDelete()) {}
		sharedPointer(T* ptr,const std::function<void(T*)>& d):pointer_(ptr),ref_count_(new size_t(1)),deleter_(d){}
		sharedPointer(const sharedPointer& rhs,const std::function<void(T*)>& d):pointer_(rhs.pointer_),ref_count_(rhs.ref_cout_),
								deleter_(d){
			(*ref_count_)++;
		}
		sharedPointer(const sharedPointer& rhs):pointer_(rhs.pointer_),ref_count_(rhs.ref_count_),deleter_(rhs.deleter_)
		{
			(*ref_count_)++;
		}
		sharedPointer(sharedPointer&& rhs) noexcept:pointer_(rhs.pointer_),
							    ref_count_(rhs.ref_count_),
			                          	    deleter_(std::move(rhs.deleter_)) 
	        {
			rhs.pointer_ = nullptr;
			rhs.ref_count_ = nullptr;	
		}
		sharedPointer& operator=(sharedPointer rhs)
		{
			swap(*this,rhs);	
			return *this;
		}
		void reset()
		{
			decrement_and_destroy();		
		}
		void reset(T* ptr)
		{
			std::function<void(T*)> d = debugDelete();
			reset(ptr,d);
		}
		void reset(T* ptr, const std::function<void(T*)>& d)
		{
			if(ptr!=pointer_)
			{
				decrement_and_destroy();
				pointer_ = ptr;
				*ref_count_ = 1;
			}
			deleter_ = d;
		}
		bool unique() const
		{
			return (*ref_count_)==1;
		}
		size_t use_count() const
		{
			return (*ref_count_);	
		}
		T& operator*() const
		{
			return (*pointer_);
		}
		
		T* operator->() const
		{
			return &this->operator*();	
		}
		
		operator bool() const 
		{
			return (pointer_==nullptr)?false:true;	
		}
		~sharedPointer() {
			decrement_and_destroy();
		}
		
	private:
		T* pointer_;
		size_t* ref_count_;
		std::function<void(T*)> deleter_;
		struct debugDelete{
			//public:
			void operator()(T* ptr)	
			{
				delete ptr;	
			}
		};

		void decrement_and_destroy()
		{
			(*ref_count_)--;
			if((*ref_count_)==0)			
			{
				deleter_(pointer_);	
				delete ref_count_;
				pointer_ = nullptr;
				ref_count_ = nullptr;
			}
			return;
		}
};
