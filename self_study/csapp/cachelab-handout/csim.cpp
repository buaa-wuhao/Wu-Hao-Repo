//#include "cachelab.h"
#include <iostream>
#include <getopt.h>
#include <unordered_map>
#include <list>
#include <tuple>
#include <vector>
#include <sstream>
#include <fstream>
using namespace std;
extern "C" {void printSummary(int hits,  /* number of  hits */
				  int misses, /* number of misses */
				  int evictions); /* number of evictions */
}
class cacheSet {
    using Iterator=list<int>::iterator;
    //using Tuple3 = tuple<int,int,int>;
    public:
        cacheSet(int n):cap(n){}
        void load(int tag,int& hit,int& miss,int& evicts)
        {
            accessCache(tag,hit,miss,evicts);
        }
        void store(int tag,int& hit,int& miss,int& evicts)
        {
            accessCache(tag,hit,miss,evicts);
        }
        void modify(int tag,int& hit,int& miss,int& evicts)
        {
            // load command
            accessCache(tag,hit,miss,evicts);
            //store right after load
            hit++;
        }
    private:
        size_t cap;
        unordered_map<int,Iterator> tags2Iterator; 
        list<int> tags;
        void accessCache(int tag,int& hit,int& miss,int& evicts)
        {
            if(tags2Iterator.count(tag))
            {
                hit++;
                const auto& it = tags2Iterator[tag];
                tags.splice(tags.begin(),tags,it);
            }
            else{
                miss++;
                tags.emplace_front(tag);
                tags2Iterator[tag] = tags.begin();
                if(tags.size()>cap)
                {
                    evicts++;
                    int temp_tag = tags.back();
                    tags.pop_back();
                    tags2Iterator.erase(temp_tag);
                }
            }
            return;
        }
};
int main(int argc,char* argv[])
{
    char c;
    int numOfSets=0,numOfLines=0,numOfBlockbits=0;
    char* pathToTraceFile=NULL;
    while ((c = getopt(argc,argv,"s:E:b:t:")) != -1) {
        switch(c) {
        case 's':
            //cout<<"s"<<endl;
            numOfSets = atoi(optarg);
            break;
        case 'E':
            numOfLines = atoi(optarg);
            //cout<<"E"<<endl;
            break;
        case 'b':
            numOfBlockbits = atoi(optarg);
            break;
        case 't':
            pathToTraceFile = optarg;
            break;
        default:
            cout<<"default "<<endl;
            exit(1);
        }
    }
    //cout<<"numOfSets: " << numOfSets << " numOfLines: "<<numOfLines<<" numOfBlockBits: "<<
    //numOfBlockbits << " path to trace: " <<pathToTraceFile << endl;
    
    //size_t block_sz = 1 << numOfBlockbits;
    size_t sets_sz = 1 << numOfSets;
    vector<cacheSet> cache(sets_sz,cacheSet(numOfLines));
    ifstream trace_file(pathToTraceFile);
    string text;
    int hit = 0,miss = 0,envicts = 0;
    while(getline(trace_file,text))
    {
        //cout<<text<<endl;
        size_t pos = text.find_first_of(',');
        istringstream iss(text.substr(0,pos));
        string operation,str_addr_hex;
        iss>>operation;
        iss>>str_addr_hex;
        //cout<<operation<<" " << str_addr_hex << " " <<endl;
        if(operation=="I")
            continue;
        long addr = stol(str_addr_hex,nullptr,16);       
        int cacheSetIndex = (addr>>numOfBlockbits) & ( (1<<numOfSets) -1);
        int addr_tag = addr >> (numOfBlockbits + numOfSets);
        if(operation=="L")
            cache[cacheSetIndex].load(addr_tag,hit,miss,envicts);
        else if(operation=="S")
            cache[cacheSetIndex].store(addr_tag,hit,miss,envicts);
        else if(operation=="M"){
            cache[cacheSetIndex].modify(addr_tag,hit,miss,envicts);
        }
    }
    printSummary(hit, miss, envicts);
    return 0;
}
