#include "TextQuery.h"

TextQuery::TextQuery(ifstream input)
{
	string line;
	while(getline(input,line))
	{
		lines.push_back(line);
		int idx = lines.size()-1;
		istringstream iss(line);
		string word;

                while(iss >> word)
		{
		    str2line[word].push_back(idx);
		}

	}
}


void Query(string word)
{
	if(str2line.count(word)==0) return;
	cout << "element occurs " << str2line[word].size() << "times" << endl;
	
	for(int i=0;i<str2line[word].size();i++)
	{
	    cout<<"line "<<str2line[word][i]<<" "<<lines[str2line[word][i]] << endl;
	}

	return;
}
