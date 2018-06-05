#include "TextQuery.h"

int main(int argc, char* argv[])
{
	TextQuery tq(argv[1]);
	string word(argv[2]);
	tq.Query();
	return 0;
}
