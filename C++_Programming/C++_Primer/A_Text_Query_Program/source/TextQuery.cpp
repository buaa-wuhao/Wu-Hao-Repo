#include "TextQuery.h"

TextQuery::TextQuery(ifstream& input)
{
	string line;
	while(getline(input,line))
	{
//		cout<<line<<endl;
		lines->push_back(line);
		int idx = lines.size()-1;
		istringstream iss(line);
		string word;

                while(iss >> word)
		{
		    str2line[word]->push_back(idx);
		}

	}
}


QueryResult TextQuery::Query(string word)
{

	QureyResult QR(lines,str2line[word]);
	return QR;
}
