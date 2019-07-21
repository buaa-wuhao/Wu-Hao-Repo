#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <set>
#include <unordered_map>
#include <sstream>
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
		string& at(size_t i) {return StrPtr->at(i);}

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

class QueryResult;
class TextQuery{
	public:
		TextQuery(ifstream& input);
		QueryResult query(string& s);
	private:
		shared_ptr<StrBlob> 				               _lines;
		unordered_map<string,shared_ptr<set<int>>>             _hash;


};

class QueryResult{
	friend ostream& print(ostream& os, QueryResult qr);
	public:
		QueryResult(string s,shared_ptr<StrBlob> lines,shared_ptr<set<int>> hash):sought(s),_lines(lines),_hash(hash) {}
		shared_ptr<StrBlob> get_file() {return _lines;}
		set<int>::iterator begin() {return _hash->begin();}
		set<int>::iterator end()   {return _hash->end();} 
	private:
		string sought;
		shared_ptr<StrBlob>        _lines;
		shared_ptr<set<int>>       _hash;
};

QueryResult TextQuery::query(string& s)
{
	static shared_ptr<set<int>> noData(new set<int>);
	if(_hash.count(s))
	{
		return QueryResult(s,_lines,_hash[s]);
	}
	else
	{
		return QueryResult(s,_lines,noData);
	}
}

TextQuery::TextQuery(ifstream& input):_lines(new StrBlob())
{
	string line;
	while(getline(input,line))
	{
		string word;
		_lines->push_back(line);
		size_t lineNo = _lines->size()-1;
		istringstream iss(line);
		while(iss>>word)
		{
			if(!_hash[word]) _hash[word].reset(new set<int>);
			_hash[word]->insert(lineNo);
		}
	}
}



ostream& print(ostream& os, QueryResult qr)
{
	os << qr.sought << " occurs " << ((qr._hash)==NULL ? 0 : (qr._hash)->size()) << " times" << endl;
	
	//if(qr._hash->empty()) return os;
	
	for(auto it: *(qr._hash))
	{
		os << "(line " << it << ") " << (qr._lines)->at(it) << endl;
	}
	return os;
}


int main(int argc,char* argv[])
{
 	ifstream input(argv[1]);
 	
 	TextQuery tq(input);
 	string word;
 	while(true)
 	{
 		if( !(cin>>word) || word=="q") break;
 		QueryResult qr = tq.query(word);
 		print(cout,qr);
 	}


 	//TextQuery tq(ifstream);
 	//QueryResult qr(&tq);

	return 0;
}