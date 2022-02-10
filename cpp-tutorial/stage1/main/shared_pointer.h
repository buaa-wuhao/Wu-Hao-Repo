#ifndef _SHARED_POINTER_H_
#define _SHARED_POINTER_H_
#include <functional>
struct debugDelete{
	template<typename T> void operator()(T* p) const 
	{
		delete p;
	}
};
template<typename T>
class shared_pointer;

template<typename T>
void swap(shared_pointer<T>& lhs,shared_pointer<T>& rhs)
{
	using std::swap;
	swap(lhs.raw_ptr_,rhs.raw_ptr_);
	swap(lhs.use_cnt_,rhs.use_cnt_);
	swap(lhs.deleter_,rhs.deleter_);
}
template<typename T>
class shared_pointer{
	friend void swap<T>(shared_pointer& lhs,shared_pointer& rhs);
	public:
		shared_pointer():raw_ptr_(nullptr),use_cnt_(nullptr),deleter_{debugDelete()}{}
		explicit shared_pointer(T* ptr):raw_ptr_(ptr),use_cnt_(new size_t(1)),deleter_(debugDelete()) {}
		shared_pointer(T* ptr,const std::function<void()>& deleter):raw_ptr_(ptr),use_cnt_(new size_t(1)),deleter_{deleter}{}
		shared_pointer(const shared_pointer& rhs):raw_ptr_(rhs.raw_ptr_),use_cnt_(rhs.use_cnt_),deleter_{rhs.deleter_}
		{
			(*use_cnt_)++;
			cout<<"use count: " << (*use_cnt_) <<endl;
		}
		shared_pointer(shared_pointer&& rhs) noexcept:raw_ptr_(rhs.raw_ptr_),use_cnt_(rhs.use_cnt_),
			deleter_(std::move(rhs.deleter_))
		{
			rhs.raw_ptr_ = nullptr;
			rhs.use_cnt_ = nullptr;	
		}
		shared_pointer& operator=(shared_pointer&& rhs) noexcept
		{
			raw_ptr_ = rhs.raw_ptr_;
			use_cnt_ = rhs.use_cnt_;
			deleter_ = rhs.deleter_;
			rhs.raw_ptr_ = nullptr;
			rhs.use_cnt_ = nullptr;	
			return *this;
		}
		shared_pointer& operator=(const shared_pointer& rhs)
		{
			if(this!=&rhs)	
			{
				free();
				raw_ptr_ = rhs.raw_ptr_;
				use_cnt_ = rhs.use_cnt_;
				deleter_ = rhs.deleter_;
				(*use_cnt_)++;
			}
			return *this;
		}
		void swap(shared_pointer& q)
		{
			swap(*this,q);
			return;
		}
		bool unique() const
		{
			return (*use_cnt_)==1;	
		}
		size_t use_count() const
		{
			return *use_cnt_;	
		}
		void reset()
		{
			free();
		}
		void reset(T* q)
		{
			reset(q,debugDelete());
		}
		void reset(T* q,const std::function<void(T*)>& d)
		{
			if(q!=raw_ptr_)
			{
				free();
				raw_ptr_ = q;
				use_cnt_ = new size_t(1);
			}
			deleter_ = d;
		}
		T* get() const
		{
			return raw_ptr_;
		}
		T& operator*() const
		{
			return *raw_ptr_;		
		}

		T* operator->() const
		{
			return &this->operator*();
		}
		operator bool() const
		{
			return raw_ptr_!=nullptr;	
		}
		~shared_pointer(){
			free();	
		}
	private:
		T* raw_ptr_;
		size_t* use_cnt_;
		std::function<void(T*)> deleter_;

		void free()
		{
			(*use_cnt_)--;
			if(*use_cnt_ == 0)	
			{
				deleter_(raw_ptr_);
				delete use_cnt_;
				raw_ptr_ = nullptr;
				use_cnt_ = nullptr;	
			}
			return;
		}
};
#endif
