#include "shared_pointer.h"
template<typename T, typename D>
class uniquePointer;

template<typename T, typename D>
void swap(uniquePointer<T,D>& lhs, uniquePointer<T,D>& rhs)
{
	using std::swap;	
	swap(lhs.raw_ptr_,rhs.raw_ptr_);
	swap(lhs.deleter_,rhs.deleter_);
}


template<typename T, typename D=debugDelete<T> >
class uniquePointer{
	friend void swap<T,D>(uniquePointer& lhs, uniquePointer& rhs);
	public:
		//prevent copy and assignement
		uniquePointer(const uniquePointer& rhs) = delete;
		uniquePointer& operator=(const uniquePointer& rhs) = delete;
		uniquePointer() = default;
		explicit uniquePointer(T* ptr):raw_ptr_(ptr) {}
		uniquePointer(uniquePointer&& rhs) noexcept:raw_ptr_(rhs.raw_ptr_){
			rhs.raw_ptr_ = nullptr;
		}

		uniquePointer& operator=(uniquePointer&& rhs) noexcept
		{
			if(this!=&rhs)	
			{
				deleter_(raw_ptr_);
				raw_ptr_ = rhs.raw_ptr_;
				rhs.raw_ptr_ = nullptr;
			}
			return *this;
		}

		void reset()
		{
			deleter_(raw_ptr_);		
			raw_ptr_ = nullptr;
		}
		
		void reset(T* ptr)	
		{
			deleter_(raw_ptr_);	
			raw_ptr_ = ptr;
		}


		T& operator*() const
		{
			return (*raw_ptr_);	
		}

		T* operator->() const
		{
			return & this->operator*();	
		}

		T* get() const
		{
			return raw_ptr_;	
		}

		operator bool () const
		{
			return (raw_ptr_==nullptr)?false:true;	
		}
		T* release()
		{
			T* rtn_pointer = raw_ptr_;
			raw_ptr_ = nullptr;
			return rtn_pointer;
		}
		~uniquePointer() {
			deleter_(raw_ptr_);	
		}


	private:
		T* raw_ptr_ = nullptr;
		D  deleter_ = D();
};
