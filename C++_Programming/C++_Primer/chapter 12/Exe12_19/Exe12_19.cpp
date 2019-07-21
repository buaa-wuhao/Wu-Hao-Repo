#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <iostream>

using namespace std;
class StrBlobPtr;

class StrBlob{

	friend class  StrBlobPtr;
	public:
		typedef vector<string>::size_type size_type;
		StrBlob():StrPtr(make_shared<vector<string>>()){}
		StrBlob(initializer_list<string> li):StrPtr(make_shared<vector<string>>(li)) {} 
		bool empty(){return StrPtr->empty();}
		size_type size(){return StrPtr->size();}
		void push_back(const string& str) {StrPtr->push_back(str);}
		void pop_back();

		string& front();
		string& back();
		string& front() const;
		string& back()  const;

		StrBlobPtr begin();
		StrBlobPtr end() ;


	private:
		shared_ptr<vector<string>> StrPtr;
		void check(size_type i,const string msg) const;
};

void StrBlob::check(size_type i,const string msg) const
{
	if(i >= StrPtr->size()) throw out_of_range(msg);
}

string& StrBlob::front()
{
	check(0,"front on empty StrBlob");
	return StrPtr->front();
}

string& StrBlob::back()
{
	check(0,"back on empty StrBlob");
	return StrPtr->back();
}

string& StrBlob::front() const
{
	check(0,"front on empty StrBlob");
	return StrPtr->front();
}

string& StrBlob::back() const
{
	check(0,"back on empty StrBlob");
	return StrPtr->back();
}

void StrBlob::pop_back()
{
	check(0,"empty StrBlob");
	StrPtr->pop_back();
	return;
}



class  StrBlobPtr{
	public:
		StrBlobPtr():curr(0) {}
		StrBlobPtr(StrBlob& a,size_t sz=0):wptr(a.StrPtr),curr(sz) {}
		shared_ptr<vector<string>> check(size_t i,const string& error) const;
		string&    deref() const;
		void incr();
	private:
		weak_ptr<vector<string>> wptr;
		size_t                 curr;
};


StrBlobPtr StrBlob::begin() {return StrBlobPtr(*this);}
StrBlobPtr StrBlob::end()   {return StrBlobPtr(*this,StrPtr->size());}

void StrBlobPtr::incr()
{
	check(curr,"Out of Range");
	curr++;
	//check(curr,"Out of Range");
	return;
}

shared_ptr<vector<string>> StrBlobPtr::check(size_t i,const string& error) const
{
	shared_ptr<vector<string>> p = wptr.lock();
	
	if(!p) throw runtime_error("unbound StrBlobPtr");
	if(i >= p->size()) throw out_of_range(error);
	return p;
}

string& StrBlobPtr::deref() const
{
	shared_ptr<vector<string>> p = check(curr,"Out of Range");
	return (*p)[curr];
}

int main(int argc,char* argv[])
{
	string filename(argv[1]);
	ifstream in(filename);
	StrBlob     data;
	StrBlobPtr  dataPtr(data);
	
	if(!in.is_open()) 
	{
		cerr<<"cant open file"<<endl;
		return -1;
	}
	
	for(string line;getline(in,line);data.push_back(line)) {}
	
	for(int i=0;i<data.size();i++)
	{
		cout<< dataPtr.deref()  <<endl;
		dataPtr.incr();
	}

	return 0;
}