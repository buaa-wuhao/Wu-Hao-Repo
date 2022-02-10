#include "strBlobPtr.h"
#include "strBlob.h"

strBlobPtr::strBlobPtr(const strBlob& a,size_type sz):wptr(a.data),cur(sz){}
strBlobPtr::strBlobPtr() {}
strBlobPtr strBlobPtr::incr()
{
	check(cur,"out of range");
	cur++;
	return (*this);
}
string& strBlobPtr::deref() const
{
	auto p = check(cur,"out of range");
	return (*p)[cur];
}
shared_ptr<vector<string>> strBlobPtr::check(size_type i, const string& msg) const
{
	auto p = wptr.lock();
	if(!p) 
		throw runtime_error("empty pointer");
	if(i>=p->size())
		throw out_of_range(msg);
	return p;
}

