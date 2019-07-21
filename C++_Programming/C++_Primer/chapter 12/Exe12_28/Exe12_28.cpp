#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <set>
#include <unordered_map>
#include <sstream>

using namespace std;
int main(int argc,char* argv[])
{
	ifstream input(argv[1]);
	
	vector<string> lines;
	unordered_map<string,set<int>> hash;
	string line;
	
	while(getline(input,line))
	{
		lines.push_back(line);
		istringstream iss(line);
		string word;
		int lineNumber = lines.size()-1;
		while(iss >> word)
		{
			hash[word].insert(lineNumber);
		}
	}

	while(true)
	{
		cout<<"enter word to look for , or q to quit: " << endl;
		string s;
		if(!(cin >> s) || s=="q")  break;
		if(hash.count(s))
		{
			for(auto it:hash[s])
			{
				cout << "(line " << it <<")" << lines[it] << endl;
			}
		}
		else
		{
			cout<< s << " does not exist!" << endl;
		} 

	}

	return 0;
}