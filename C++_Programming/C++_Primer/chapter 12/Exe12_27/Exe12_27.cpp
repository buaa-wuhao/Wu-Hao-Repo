#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <set>
#include <unordered_map>
#include <sstream>
#include <memory>

using namespace std;



class TextQuery{
	public:
		TextQuery(ifstream& input,vector<string> lines,unordered_map<string,set<int>> hash);
		TextQuery(vector<string> lines,unordered_map<string,set<int>> hash);
		//TextQuery(ifstream& input);
		shared_ptr<vector<string>> getLines() {return _lines;}
		shared_ptr<unordered_map<string,set<int>>>  getHash() {return _hash;}
	private:
		shared_ptr<vector<string>> 				    _lines;
		shared_ptr<unordered_map<string,set<int>>>  _hash;


};
TextQuery::TextQuery(vector<string> lines,unordered_map<string,set<int>> hash)
{

}

TextQuery::TextQuery(ifstream& input,vector<string> lines,unordered_map<string,set<int>> hash)
//TextQuery::TextQuery(ifstream& input)//,vector<string> lines,unordered_map<string,set<int>> hash)
{
	// _lines = make_shared<vector<string>>(lines);
	// _hash  = make_shared<unordered_map<string,set<int>>>(hash);
	// string _line;
	// while(getline(input,_line))
	// {
	// 	_lines->push_back(_line);
	// 	istringstream _iss(_line);
	// 	string word;
	// 	int lineNumber = _lines->size()-1;
	// 	while(_iss >> word)
	// 	{
	// 		_hash->at(word).insert(lineNumber);
	// 	}
	// }

}

class QueryResult{
	public:
		QueryResult(TextQuery* tq);
		void query(string s);
	private:
		shared_ptr<vector<string>> 				    _lines;
		shared_ptr<unordered_map<string,set<int>>>  _hash;
};

QueryResult::QueryResult(TextQuery* tq)
{
	_lines = tq->getLines();
	_hash  = tq->getHash();
}

void QueryResult::query(string s)
{
	if( _hash->count(s)==0)
	{
		cout<<s <<" does not exist!" << endl;
		return;
	}

	for(auto it:(_hash)->at(s))
	{
		cout << _lines->at(it) << endl;
	}
	return;
}


int main(int argc,char* argv[])
{
 	ifstream input(argv[1]);
 	
 	vector<string> lines;
 	unordered_map<string,set<int>> hash;
 	
 	//TextQuery tq(lines,hash);
 	TextQuery tq(input,lines,hash);
 	//TextQuery tq(ifstream);
 	//QueryResult qr(&tq);

 	while(true)
	{
		cout<<"enter word to look for , or q to quit: " << endl;
		string s;
		if(!(cin >> s) || s=="q")  break;
		//qr.query(s);
	}

	return 0;
}
// int main(int argc,char* argv[])
// {
// 	ifstream input(argv[1]);
	
// 	vector<string> lines;
// 	unordered_map<string,set<int>> hash;
// 	string line;
	
// 	while(getline(input,line))
// 	{
// 		lines.push_back(line);
// 		istringstream iss(line);
// 		string word;
// 		int lineNumber = lines.size()-1;
// 		while(iss >> word)
// 		{
// 			hash[word].insert(lineNumber);
// 		}
// 	}

// 	while(true)
// 	{
// 		cout<<"enter word to look for , or q to quit: " << endl;
// 		string s;
// 		if(!(cin >> s) || s=="q")  break;
// 		if(hash.count(s))
// 		{
// 			for(auto it:hash[s])
// 			{
// 				cout << "(line " << it <<")" << lines[it] << endl;
// 			}
// 		}
// 		else
// 		{
// 			cout<< s << " does not exist!" << endl;
// 		} 

// 	}

// 	return 0;
// }