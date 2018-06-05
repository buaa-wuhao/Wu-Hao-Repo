#include <vector>
#include <fstream>
#include <string>
#include <unordered_map>

using namespace std;

class TextQuery
{
        private:
	      unordered_map<string,vector<int>> str2line;
              vector<string> lines;
	public:
	    TextQuery(ifstream input);
	    void  Query(string word); 


};


