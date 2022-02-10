
#include<initializer_list>
#include<memory>
#include<string>
#include<vector>
template<typename> class BlobPtr;
//1.forward decaration??2. why there's no T after typename above?/
template<typename T> 
class Blob{
	typedef T valuetype;
	typedef typename vector<T>::size_type size_type;
	friend class BlobPtr<T>;
	public:
		Blob():data(make_shared<std::vector<T>>()){}
		Blob(initializer_list<T> il);
		bool empty() const{return data->empty();}
		size_type size() const{return data->size();}
		void push_back(T& elem) {data->push_back(elem);}
		void push_back(T&& elem){data->push_back(move(elem));}
		T& operator[](size_type i) {return (*data)[i];}
		T& back();
		void pop_back();
	private:
		shared_ptr<std::vector<T>> data;
		void check(size_type i,const std::string& msg) const;
};


template<typename T>
void Blob<T>::check(size_type i, const std::string& msg) const
{
	if(i>=data->size())
		throw out_of_range(msg);
	return;
}

template<typename T>
T& Blob<T>::back()
{
	check(0,"Empty Blob!");
	return data->back();
}

template<typename T>
void Blob<T>::pop_back()
{
	check(0,"Empty Blob!");
	data->pop_back();
	return;
}

template<typename T>
Blob<T>::Blob(initializer_list<T> il)
{
	data = make_shared<std::vector<T>>(il);
}
