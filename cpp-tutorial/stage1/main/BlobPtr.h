#include <string>
#include <memory>
#include <vector>

template<typename T>
class BlobPtr{
	typedef typename std::vector<T>::size_type size_type;
	//friend class Blob<T>;
	public:
		BlobPtr():curr(0){}
		BlobPtr(Blob<T>& a,size_type sz=0):wptr(a.data),curr(sz){}
		//T& operator*(BlobPtr& p) const?? why??
		T& operator*() const
		{
			auto p = check(curr,"out of range");
			return (*p)[curr];
		}
		BlobPtr& operator++()
		{
			curr++;
			return *this;
		}
		BlobPtr& operator--()
		{
			curr--;
			return *this;	
		}

	private:
		shared_ptr<std::vector<T>> check(size_type i, const std::string& msg);
		weak_ptr<std::vector<T>> wptr;
		size_type curr;
};

template<typename T>
shared_ptr<std::vector<T>> BlobPtr<T>::check(size_type i,const std::string& msg)
{
	auto p = wptr.lock();
	if(!p) throw runtime_error("empty pointer");
	if(i>=p->size())
		throw out_of_range(msg);
	return p;
}
