#ifndef TEXT_QUERY_H_
#define TEXT_QUERY_H_

#include<vector>
#include<iostream>
#include<memory>
#include<unordered_map>
#include<fstream>
#include<sstream>
#include<algorithm>
#include<string>

using namespace std;
class queryResult{
	friend ostream& operator<<(ostream& o, const queryResult& qr);
	public:
		queryResult(const string& word,shared_ptr<vector<int>> lines,
			    shared_ptr<vector<string>> file):sought(word),lines(lines),file(file){}
		vector<int>::iterator begin() const{
			return (*lines).begin();
		}
		vector<int>::iterator end() const{
			return (*lines).end();
		}
		shared_ptr<vector<string>> getFile() const{
			return file;
		}
	private:
		string sought;
		shared_ptr<vector<int>> lines;
		shared_ptr<vector<string>> file;
};

ostream& operator<<(ostream& os,const queryResult& qr){
	os<<"Searching word: " << qr.sought<<endl;
	for(int i=0;i<qr.lines->size();i++)	
	{
		os<<"(line: "<< (*qr.lines)[i]<<") "<<(*qr.file)[(*qr.lines)[i]]<<endl; 	
	}
	return os;
}

class textQuery{
	public:
		textQuery(ifstream& ifile);
		queryResult query(const string& word) const;
	private:
		shared_ptr<vector<string>> file;
		unordered_map<string,shared_ptr<vector<int>>> dict;
		
};

queryResult textQuery::query(const string& word) const{
	if(dict.count(word)==0)	
	{
		auto nolines = make_shared<vector<int>>();
		return queryResult(word,nolines,file);
	}
	else{
		return queryResult(word,dict.find(word)->second,file);	
	}
}

textQuery::textQuery(ifstream& ifile):file(new vector<string>()){
	string line_str;
	while(getline(ifile,line_str)){
		file->push_back(line_str);
		line_str.erase(remove_if(line_str.begin(),line_str.end(),::ispunct),line_str.end());
		//cout<<line_str<<endl;
		istringstream is(line_str);	
		string word;
		while(is>>word){
			if(dict.count(word)==0){
				dict[word] = make_shared<vector<int>>();	
			}
			dict[word]->push_back(file->size()-1);
		}
	}	
}


#endif