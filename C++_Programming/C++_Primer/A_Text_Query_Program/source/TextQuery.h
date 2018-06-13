#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <iostream>
#include <memory>

using namespace std;

class QueryResult
{
	friend void print(ostream&,QueryResult&);	
	
	private:
              shared_ptr<vector<string>> lines;
	      shared_ptr<vector<int>>    rows;
	public:
	      QueryResult(shared_ptr<vector<string>> l,
                         shared_ptr<vector<int>>     r):lines(l),rows(r) {};		
};

class TextQuery
{
        private:
	      unordered_map<string,shared_ptr<vector<int>>> str2line;
              shared_ptr<vector<string>> lines;
	public:
	    TextQuery(ifstream& input);
	    QueryResult  Query(string word); 


};




