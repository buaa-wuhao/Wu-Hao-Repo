#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <set>
#include <unordered_map>
#include <sstream>
#include <memory>
#include "exe13_39.hpp"

using namespace std;


class QueryResult;
class TextQuery{
	public:
		TextQuery(ifstream& input);
		QueryResult query(string& s);
	private:
		//shared_ptr<vector<string>> 				               _lines;
		shared_ptr<strVec> 				               		   _lines;
		unordered_map<string,shared_ptr<set<int>>>             _hash;


};

class QueryResult{
	friend ostream& print(ostream& os, QueryResult qr);
	public:
		QueryResult(string s,shared_ptr<strVec> lines,shared_ptr<set<int>> hash):sought(s),_lines(lines),_hash(hash) {}
		shared_ptr<strVec> get_file() {return _lines;}
		set<int>::iterator begin() {return _hash->begin();}
		set<int>::iterator end()   {return _hash->end();} 
	private:
		string sought;
		shared_ptr<strVec> 		   _lines;
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

TextQuery::TextQuery(ifstream& input):_lines(new strVec)
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