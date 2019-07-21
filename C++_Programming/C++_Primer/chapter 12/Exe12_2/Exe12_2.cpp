#include <string>
#include <vector>
#include <memory>

using namespace std;

class StrBlob{

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



int main()
{
	StrBlob data;
	return 0;
}