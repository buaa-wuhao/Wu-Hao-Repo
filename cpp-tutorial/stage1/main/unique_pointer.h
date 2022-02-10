#ifndef _SHARED_POINTER_H_
#include "shared_pointer.h"
#endif
template<typename T,typename D>
class unique_pointer;

template<typename T,typename D=debugDelete>
void swap(unique_pointer<T,D>& lhs,unique_pointer<T,D>& rhs)
{
	using std::swap;
	swap(lhs.raw_ptr_,rhs.raw_ptr_);
	swap(lhs.deleter_,rhs.deleter_);	
	return;
}
template<typename T, typename D=debugDelete>
class unique_pointer {
	friend void swap<T,D>(unique_pointer& lhs, unique_pointer& rhs);
	public:
		unique_pointer():raw_ptr_(nullptr),deleter_(D()){}
		unique_pointer(const unique_pointer& other) = delete;
		unique_pointer& operator=(const unique_pointer& other) = delete;
		explicit unique_pointer(T* ptr,D del = D() ):raw_ptr_(ptr),deleter_(del){}
		unique_pointer(unique_pointer&& other) noexcept:raw_ptr_(other.raw_ptr_),deleter_(std::move(other.raw_ptr_)){
			other.raw_ptr_ = nullptr;	
		}
		unique_pointer& operator=(unique_pointer&& other) noexcept {
			raw_ptr_ = other.raw_ptr_;
			deleter_ = std::move(other.deleter_);
			other.raw_ptr_ = nullptr;
			return *this;	
		}
		void swap(unique_pointer& other)
		{
			swap(*this,other);
			return;	
		}
		~unique_pointer(){
			deleter_(raw_ptr_);	
		}
		T* release()
		{
			T* ptr = raw_ptr_;
			raw_ptr_ = nullptr;
			return ptr;		
		}
		void reset()
		{
			reset(nullptr);
		}
		void reset(T* q)
		{
			deleter_(raw_ptr_);
			raw_ptr_ = q;	
		}
		T&  operator*() const
		{
			return *raw_ptr_;	
		}
		T*  operator->() const
		{
			return &this->operator*();			    
		}
		operator bool() const
		{
			return (raw_ptr_==nullptr)? false:true;	
		}
		T* get() const
		{
			return raw_ptr_;	
		}
	private:
		T* raw_ptr_;
		D  deleter_;
};
