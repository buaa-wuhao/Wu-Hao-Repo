#include "TextQuery.h"

int main(int argc, char* argv[])
{
	ifstream input(argv[1]);
	TextQuery tq(input);
	string word;

	while((cin>>word) && word!="q")
	{
	    QueryResult qr = tq.Query(word);
	    print(cout,qr);
        }	
	
	return 0;
}
