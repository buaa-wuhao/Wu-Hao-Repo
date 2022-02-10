#include "vec.h"
using namespace std;

template<typename T>
allocator<T> vec<T>::alloc_;

template<typename T>
vec<T>::vec(const initializer_list<T>& li)
{
	for(auto& it:li)	
		push_back(it);
}

template<typename T>
void vec<T>::reserve(size_t n)
{
	if(n <= capacity())
		return;
	T* new_b = alloc_.allocate(n);
	T* new_ptr = new_b;
	for(T* ptr= element_;ptr!=first_free_;ptr++)
		alloc_.construct(new_ptr++,std::move(*ptr));	
	free();
	element_ = new_b;
	first_free_ = new_ptr;
	cap_ = new_b + n;
	return;
}

template<typename T>
void vec<T>::pop_back()
{
	if(empty())
		return;
	alloc_.destroy(--first_free_);
}

template<typename T>
void vec<T>::resize(size_t n,const T& val)
{
	if(n > size())
	{
		int i = n-size();
		while(i>0)	
		{
			push_back(val);
			i--;	
		}
	}
	else if(n < size())
	{
		for(T* ptr=element_+n;ptr!=first_free_;ptr++)		
			alloc_.destroy(ptr);
		first_free_ = element_+n;
	}
	return;
	
}

template<typename T>
pair<T*,T*> vec<T>::allocate_n_copy(const T* b, const T* e)
{
	T* new_b = alloc_.allocate(e-b);
	T* new_e = uninitialized_copy(b,e,new_b);
	return make_pair(new_b,new_e);
}

template<typename T>
vec<T>::vec(const vec& rhs)
{
	auto data = allocate_n_copy(rhs.begin(),rhs.end());
	element_ = data.first;
	first_free_ = data.second;
	cap_ = data.second;	
}

template<typename T>
vec<T>& vec<T>::operator=(const vec& rhs)
{
	auto data = allocate_n_copy(rhs.begin(),rhs.end());
	free();
	element_ = data.first;
	first_free_ = data.second;
	cap_ = data.second;
	return *this;
}

template<typename T>
vec<T>& vec<T>::operator=(vec&& rhs) noexcept
{
	if(this != &rhs)
	{
		free();
		element_ = rhs.element_;
		first_free_ = rhs.first_free_;
		cap_ = rhs.cap_;
		rhs.element_ = nullptr;
		rhs.first_free_ = nullptr;
		rhs.cap_ = nullptr;	
	}
	return *this;
}

template<typename T>
vec<T>::vec(vec&& rhs) noexcept:element_(rhs.element_),first_free_(rhs.first_free_),cap_(rhs.cap_)
{
	rhs.element_ = rhs.first_free_ = rhs.cap_ = nullptr;
}

template<typename T>
void vec<T>::reallocate()
{
	size_t new_sz = (size()==0)?1:2*size();
	T* new_b = alloc_.allocate(new_sz);
	T* new_ptr = new_b;
	
	for(T* ptr = element_;ptr!=first_free_;ptr++)
		alloc_.construct(new_ptr++,std::move(*ptr));
	free();
	element_ = new_b;
	first_free_ = new_ptr;
	cap_ = new_b+new_sz;
	return;
  		
}
template<typename T>
void vec<T>::check_n_copy()
{
	if(size()==capacity())
		reallocate();
	return;
}

template<typename T>
void vec<T>::free()
{
	if(element_)
	{
//	for(T* ptr=first_free_;ptr!=element_;)
//	{
//		alloc_.destroy(--ptr);	
//	}
		for_each(element_,first_free_,[](string& str){alloc_.destroy(&str);});
		alloc_.deallocate(element_,cap_ - first_free_);
	}
	return;
}

template<typename T>
void vec<T>::push_back(const T& elem)
{
	check_n_copy();
	alloc_.construct(first_free_++,elem);
	return;
}


