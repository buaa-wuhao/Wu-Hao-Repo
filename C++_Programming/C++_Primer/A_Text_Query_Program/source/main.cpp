#include "TextQuery.h"

int main(int argc, char* argv[])
{
	ifstream input(argv[1]);
	TextQuery tq(input);
	string word;

	while((cin>>word) || word!="q")
	{
            QueryResult qr = tq.Query(word);
	    for(int i=0;i<qr.rows->size();i++)
	    {
		cout<< *(qr.lines->begin()+ *(qr.rows->begin()+i)) << endl;
	    }
	}	
	
	return 0;
}
