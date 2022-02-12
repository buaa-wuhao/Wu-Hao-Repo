#include "lib/hello-time.h"
#include "main/hello-greet.h"
#include <iostream>
#include <string>
#define NDEBUG
#include <assert.h>
#include <new>
#include "SharedPtr.h"
#include <unordered_map>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "String.h"
#include "priority_queue.h"
#include "matrix.h"
#include "text_query.h"
#include "Query.h"
#include "Find.h"
#include "shared_ptr1.h"
#include "my_allocator.h"
#include "singleton.h"
#include "taskScheduler.h"
class myAllocator{
	using Integer = size_t;
	private:
		size_t num_of_blks;
		size_t blk_sz;
		size_t blks_used;
		char*  blk_head_ptr;
		//is this structure private or not,it is private!?
		struct Block{
			//For C , do we need "struct Block* next"?
			Block* next;	
		};
		Block* p_list;
		void Push(Block* ptr){
			if(ptr==nullptr) return;
			ptr->next = p_list;
			p_list = ptr;	
			return;
		}
		Block* Pop(){
			Block* rtn = p_list;
			if(p_list)	
				p_list = p_list->next;
			return rtn;
		}
	public:
		myAllocator(size_t numOfBlks,size_t blkSz):num_of_blks(numOfBlks),blk_sz(blkSz)
		{
			//what about malloc throw exception?
			blk_head_ptr = new char[num_of_blks*blk_sz];
			p_list = nullptr;
			blks_used = 0;
		}
		void* myAllocate(size_t sz)
		{
			assert(sz<=blk_sz);
			char* rtn_ptr = (char*) Pop();
			if(rtn_ptr==nullptr)
			{
				if(blks_used < num_of_blks)	
				{
					rtn_ptr = blk_head_ptr + blks_used*blk_sz;
					blks_used++;
				}
				else{
					std::new_handler handler = std::set_new_handler(0);
					std::set_new_handler(handler);
					if(handler)	
					{
						std::cout<< "lassert!o ha"<<std::endl;	
						(*handler)();
					}
					else{
						std::cout<< "assert!o ha"<<std::endl;	
						assert(0);
					}
				}
			}
			return (void*)rtn_ptr;
		}
		void myFree(void* ptr)
		{
		 	Push((Block*) ptr);	
			return;
		}
		~myAllocator()
		{
			delete [] blk_head_ptr;	
		}
};

class sparseMatrix{
	friend sparseMatrix operator+(const sparseMatrix& lhs, const sparseMatrix& rhs);
	public:
		sparseMatrix(int m,int n):rows(m),cols(n) {}
		sparseMatrix& operator+=(const sparseMatrix& rhs){
			if(rows!=rhs.rows || cols!=rhs.cols)
				return *this;
			for(auto& row:rhs.data)	
			{
				int r = row.first;
				for(auto& col:row.second)	
				{
					int c = col.first;
					data[r][c] += col.second;
					if(data[r][c]==0)
						data[r].erase(c);
				}
				if(data[r].empty())
					data.erase(r);
			}
			return *this;
		}
		bool set(int x,int y,int val){
			if(x<0||y<0||x>=rows||y>=cols)
				return false;
			data[x][y] = val;	
			return true;
		}
		bool get(int x,int y,int& val){
			if(x<0||y<0||x>=rows||y>=cols)
				return false;
			if(data.count(x)==0 || data[x].count(y)==0)
				return false;
			val = data[x][y];
			return true;
		}
	private:
		std::unordered_map<int,std::unordered_map<int,int>> data;
		int rows;
		int cols;


};
sparseMatrix operator+(sparseMatrix lhs, const sparseMatrix& rhs){
	lhs += rhs;
	return lhs;
}

int thread_i = 0;
std::mutex mtx;
std::condition_variable cv;
int nn = 30;
void thread_a(void){
	while(true){
		std::unique_lock<std::mutex> lck(mtx);
		while(thread_i<nn && thread_i%2)
			cv.wait(lck);
		if(thread_i>=nn) break;
		thread_i++;
		std::cout<<"a";
		cv.notify_one();
	}
}

void thread_b(void){

	while(true){
		std::unique_lock<std::mutex> lck(mtx);
		while(thread_i<nn && thread_i%2==0)	
			cv.wait(lck);
		if(thread_i>=nn) break;
		thread_i++;
		std::cout<<"b";
		cv.notify_one();
	}
}

#define N 16
typedef int fix_matrix[N][N];

void fix_set_diag(fix_matrix A,int val){
	for(int i=0;i<N;i++)
	{
		A[i][i] = val;	
	}
	return;
}
typedef struct{
	int x;
	int y;
}strA;
//strA 
void process(strA A){
	A.x = 99;
	A.y = 88;	
	std::cout<<A.x<<" "<<A.y<<std::endl;
}

class Test
{
int i ;
 
public:
Test(int ii):i(ii) { } //constructor
 
int get() const { return i; }
 
~Test( ) { }
};
 
string reverse_sentence(const string& s){
	auto sz = s.size();
	vector<string> words;
	string word;
	for(int i=0;i<=sz;i++)
	{
		if(i<sz && isalpha(s[i]))
		{
			word += s[i];
			continue;
		}
		if(!word.empty())
		{
			words.push_back(word);
			word = "";
		}
		if(i<sz && ispunct(s[i]))
		{
			words.push_back(string(1,s[i]));
			continue;
		}
	}
	int l = 0,r = words.size()-1;
	//for(auto& it:words)
	//		cout<<it<<" ";
	while(l<r)
	{
		while(l<r && ispunct(words[l][0]))
			l++;
		while(l<r && ispunct(words[r][0]))
			r--;
		swap(words[l++],words[r--]);
	}
	word = "";
	for(int i=0;i<words.size();i++)
	{
		if(!word.empty() && !ispunct(words[i][0]))
		{
			word += " ";
		}
		word += words[i];
	}
	return word;
}
int main(int argc, char** argv) {

  taskScheduler task_scheduler;
  std::string who = "hi, I am Hao!";
  if (argc > 1) {
    who = argv[1];
  }
  //my_allocator* p_alloc = singleton<my_allocator>::instance();
  my_allocator alloc(10,30);
  //cout<<"my__allocator?"<<endl;
  char* ptr = (char*)alloc.allocate(25);
  cout<<"my__allocator?"<<endl;
  char* ptr1 = (char*)alloc.allocate(15);
  cout<<"my__allocator?"<<endl;
  *ptr1 = 'h';
  *ptr = 'w';
  cout<<"my__allocator?"<<endl;
  std::cout << get_greet(who) <<  *ptr << std::endl;
  cout<<"test "<<endl;
  cout<<"running? "<<reverse_sentence(who)<<endl;
  shared_ptr1<Test> ptr_test(new Test(5));
  std::vector<Test> vec={ 21 , 45 };

vec.push_back( Test(34) ) ; //Appending Test object by passing Test object
 
vec.push_back( 901 ) ; //Appending Test object but int data is passed,work fine
 
vec.emplace_back( Test(7889) ); //work fine
 
vec.emplace_back( 4156 ) ; //work fine
 
for( auto elem:vec )
{
	std::cout<< elem.get() << " " ;
}
  ifstream ifs("test.txt",ifstream::in);
  textQuery tq(ifs);

  query qry = query("hair") | query("Alice");
  query qry1 = ~query("Daddy");
  cout<< qry.eval(tq) <<endl;
  cout<<qry1.eval(tq)<<endl;
// while(true){
//  cout<<"Enter the word,you would like to search: "<<endl;
//  string s;
//  if(!(cin>>s) || s=="q") break;
//  cout<< tq.query(s) << endl;
// }


  strA A{1,2};
  process(A);
  std::cout<<A.x<<" "<<A.y<<std::endl;

  int array[N]={1,2,3};
  std::cout<< &array<<std::endl;
  std::cout<< (int*)(&array+1)-(int*)&array << std::endl;
  fix_matrix B;
  fix_set_diag(B,555);
  for(int i=0;i<N;i++)
  {
 	for(int j=0;j<N;j++){
//		std::cout<< B[i][j]<<" ";
	} 
//	std::cout<<std::endl;
  }
  matrix<int> matx(5,6);
  //myAllocator::Integer i = 0;
  //myAllocator alloc(1,20);
  //char* ptr = (char*)alloc.myAllocate(25);
  //char* ptr1 = (char*)alloc.myAllocate(15);
  //*ptr1 = 'h';
  //*ptr = 'w';
  //std::cout << get_greet(who) <<  *ptr << std::endl;
  print_localtime();
  String str("wuhao");
  //std::cout<<str<<std::endl;	
  std::thread th1(thread_a),th2(thread_b);
  th1.join();
  th2.join();
  priorityQueue<int> pq{5,7,8,10,23,99};
  while(!pq.empty()){
	  std::cout<<pq.top()<<" ";
        pq.pop();	
  }
  std::cout<<std::endl;
  return 0;
}
