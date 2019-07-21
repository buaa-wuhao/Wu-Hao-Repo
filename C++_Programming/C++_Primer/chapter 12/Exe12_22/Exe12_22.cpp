#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <iostream>

using namespace std;
class ConstStrBlobPtr;

class StrBlob{

	friend class  ConstStrBlobPtr;
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

		ConstStrBlobPtr begin() const;
		ConstStrBlobPtr end() const;


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



class  ConstStrBlobPtr{
	public:
		ConstStrBlobPtr():curr(0) {}
		ConstStrBlobPtr(const StrBlob& a,size_t sz=0):wptr(a.StrPtr),curr(sz) {}
		shared_ptr<vector<string>> check(size_t i,const string& error) const;
		const string&    deref() const;
		void incr();
	private:
		weak_ptr<vector<string>> wptr;
		size_t                 curr;
};


ConstStrBlobPtr StrBlob::begin() const {return ConstStrBlobPtr(*this);}
ConstStrBlobPtr StrBlob::end()   const {return ConstStrBlobPtr(*this,StrPtr->size());}

void ConstStrBlobPtr::incr()
{
	check(curr,"Out of Range");
	curr++;
	//check(curr,"Out of Range");
	return;
}

shared_ptr<vector<string>> ConstStrBlobPtr::check(size_t i,const string& error) const
{
	shared_ptr<vector<string>> p = wptr.lock();
	
	if(!p) throw runtime_error("unbound StrBlobPtr");
	if(i >= p->size()) throw out_of_range(error);
	return p;
}

const string& ConstStrBlobPtr::deref() const
{
	shared_ptr<vector<string>> p = check(curr,"Out of Range");
	return (*p)[curr];
}

int main(int argc,char* argv[])
{
	/*
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
*/
	return 0;
}