#include "TextQuery.h"

TextQuery::TextQuery(ifstream& input):lines(new vector<string>)
{
	string line;
	while(getline(input,line))
	{
//		cout<<line<<endl;
		lines->push_back(line);
		int idx = lines->size()-1;
		istringstream iss(line);
		string word;

                while(iss >> word)
		{
		    if(str2line.count(word)==0)
		    {
		       str2line[word].reset(new vector<int>);
		    }
		    str2line[word]->push_back(idx);
		}

	}
}


QueryResult TextQuery::Query(string word)
{

	shared_ptr<vector<int>> nodata(new vector<int>);

	if(str2line.count(word)!=0)
	{  return  QueryResult(lines,str2line[word]);}
	else
	{
	   return QueryResult(lines,nodata);
	}
}

void print(ostream& out , QueryResult& qr)
{

	    out<< "Total # of Occurences: " << qr.rows->size() << endl;
	    for(int i=0;i<qr.rows->size();i++)
	    {
		if(i>0 && *(qr.rows->begin()+i-1) == *(qr.rows->begin()+i)) continue;		    
		out<<"line "<< *(qr.rows->begin()+i) << ": " << *(qr.lines->begin()+ *(qr.rows->begin()+i)) << endl;
	    }
		
}
