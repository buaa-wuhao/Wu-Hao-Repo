#include <functional>
#include <iostream>

struct debugDelete1{
	template<typename T>
	void operator()(T* ptr){
		std::cout<<"debug delete" <<std::endl;
		delete ptr;	
	}	
};
template<typename T>
class shared_ptr1;

template<typename F>
void swap(shared_ptr1<F>& spt1,shared_ptr1<F>& spt2)
{
	using std::swap;
	swap(spt1.raw_ptr,spt2.raw_ptr);
	swap(spt1.use_cnt,spt2.use_cnt);
	swap(spt1.del,spt2.del);
	return;
}

template<typename T>
class shared_ptr1{
	template<typename F>
	friend void swap(shared_ptr1<F>& spt1,shared_ptr1<F>& spt2);
	public:
		explicit shared_ptr1(T* ptr):raw_ptr(ptr),use_cnt(new size_t(1)),del(debugDelete1()){}
		shared_ptr1(T* ptr,std::function<void(T*)>& dele):raw_ptr(ptr),use_cnt(new size_t(1)),del(dele){}
		shared_ptr1(const shared_ptr1& other):raw_ptr(other.raw_ptr),use_cnt(other.use_cnt),del(other.del){
			(*use_cnt)++;	
		}
		shared_ptr1& operator=(const shared_ptr1& other){
			if(this!=&other)
			{
				free();
				raw_ptr = other.raw_ptr;
				use_cnt = other.use_cnt;	
				del = other.del;
				(*use_cnt)++;
			}
			return *this;
		}
		shared_ptr1(shared_ptr1&& other) noexcept:raw_ptr(other.raw_ptr),use_cnt(other.use_cnt),del(std::move(other.del)){
			other.raw_ptr = nullptr;
			other.use_cnt = nullptr;	
		}
		shared_ptr1& operator=(shared_ptr1&& other) noexcept{
			raw_ptr = other.raw_ptr;
			use_cnt = other.use_cnt;
			del = std::move(other.del);
			other.raw_ptr = nullptr;
			other.use_cnt = nullptr;	
			return *this;
		}

		void reset(T* new_ptr,const function<void(T*)>& d){
			if(new_ptr!=raw_ptr)	
			{
				free();
				raw_ptr = new_ptr;
				use_cnt = new size_t(1);	
			}
			del = d;
			return;
		}

		T& operator*() const{
			return *this;	
		}
		
		T* operator->() const{
			return &this->operator*();	
		}

		~shared_ptr1(){
			free();	
		}
	private:
		T* raw_ptr;
		size_t* use_cnt;
		std::function<void(T*)> del; 
		void free(){
			(*use_cnt)--;
			if((*use_cnt)==0)	
			{
				del(raw_ptr);
			}
			return;
		}
};
