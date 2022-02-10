#include "blob.h"

template<typename T>
void Blob<T>::check(size_type i, const string& msg) const
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
	data = make_shared(vector<T>(il));
}
