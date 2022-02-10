#include "strBlob.h"
#include "strBlobPtr.h"

strBlobPtr strBlob::begin() const {return strBlobPtr(*this,0);}
strBlobPtr strBlob::end() const {return strBlobPtr(*this,data->size());}

strBlob::strBlob(const strBlob& str)
{
	data = make_shared<vector<string>>(*str.data);
}
strBlob& strBlob::operator=(const strBlob& rhs)
{
	data = make_shared<vector<string>>(*rhs.data);
	return *this;
}
string& strBlob::front() const
{
	check(0,"empty !");
	return data->front();
}
string& strBlob::back() const
{
	check(0,"empty!");
	return data->back();
}
void strBlob::check(size_type i, const string& msg) const
{
	if(i>=data->size())
		throw out_of_range(msg);
	return;
}

void strBlob::pop_back()
{
	check(0,"empty!");	
	data->pop_back();
	return;
}
