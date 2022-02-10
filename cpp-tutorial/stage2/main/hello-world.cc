#include "hello-greet.h"
#include <ctime>
#include <iostream>
#include <string>
#include <assert.h>
#include "Pub_sub.h"
#include "virtual_function.h"
#include <sys/time.h>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <functional>
#include <vector>
#include <unordered_map>
#include <map>
#include <memory>
#include <mutex>
#include <queue>
#include <algorithm>
using namespace std;

class FizzBuzz {
private:
    int n;
    int i;
    mutex mx;
    condition_variable cv;
public:
    FizzBuzz(int n) {
        this->n = n;
        i = 1;
    }

    // printFizz() outputs "fizz".
    void fizz(function<void()> printFizz) {
        unique_lock<mutex> lck(mx);
	while(true)
	{
		while(i<=n && !(i%3==0 && i%5!=0))     
		    cv.wait(lck);
		//cout<< i <<" after fizz" <<endl;
		if(i>n) break;
		printFizz();
		i++;
	 	cv.notify_all();	
	}
        cout<< "end of before fizz" <<endl;
    }

    // printBuzz() outputs "buzz".
    void buzz(function<void()> printBuzz) {
        unique_lock<mutex> lck(mx);
	while(true)
	{
		while(i<=n && !(i%3!=0 && i%5==0))     
		    cv.wait(lck);
		//cout<< i <<" after buzz" <<endl;
		if(i>n) break;
		printBuzz();
		i++;
		cv.notify_all();
	}
        cout<< "end of buzz" <<endl;
    }

    // printFizzBuzz() outputs "fizzbuzz".
	void fizzbuzz(function<void()> printFizzBuzz) {
        unique_lock<mutex> lck(mx);
     //   cout<< i <<" before fizzbuzz" <<endl;
	while(true)
	{
		while(i<=n && !(i%3==0 && i%5==0))     
		    cv.wait(lck);
	      //  cout<< i <<" after fizzbuzz" <<endl;
		if(i>n) break;
		printFizzBuzz();
		i++;
		cv.notify_all();
	}
        cout<< " end of fizzbuzz" <<endl;
    }

    // printNumber(x) outputs "x", where x is an integer.
    void number(function<void(int)> printNumber) {
        unique_lock<mutex> lck(mx);
	while(true)
	{
		while(i<=n && (i%3==0 || i%5==0))
		    cv.wait(lck);
		if(i>n) break;
		printNumber(i);
		i++;
		cv.notify_all();
	}
        cout<< "end of printNumber" << endl;
    }
};
FizzBuzz fizbuz(15);

void printFizz(){
	cout<<"Fizz" << " " << endl;
}
void printBuzz(){
	cout<<"Buzz" << " " << endl;
}
void printFizzBuzz(){
	cout<<"FizzBuzz"<<" " << endl;
}
void printNumber(int i){
	cout<<to_string(i)<<" " <<endl;
}
void thread_IDLE(){
	cout<<"idle!..  ";
}
void thread_A(){
	fizbuz.fizz(printFizz);
	/*
	while(true)
		cout<<"Thread_A"<<endl;
		*/
}

void thread_B(){
	fizbuz.buzz(printBuzz);
	/*
	while(true)
		cout<<"Thread_B"<<endl;
		*/
}

void thread_C(){
	fizbuz.fizzbuzz(printFizzBuzz);
	/*
	while(true)
		cout<<"Thread_C"<<endl;
		*/
}

void thread_D(){
	fizbuz.number(printNumber);
	/*
	while(true)
		cout<<"Thread_D"<<endl;
		*/
}

auto delFunc = [](FizzBuzz* ptr){
	cout<< "customer delete"<<endl;
	delete ptr;	
};

void delFiz(FizzBuzz* p)
{
	cout<<"customer delete" << endl;
	delete p;
}

template<typename... Ts>
std::unique_ptr<FizzBuzz,decltype(delFiz)*> 
makeFizzBuzza(Ts&&... params){
	std::unique_ptr<FizzBuzz,decltype(delFiz)*> uptr(nullptr,delFiz);	
	uptr.reset(new FizzBuzz(std::forward<Ts>(params)...));
	return uptr;
}


template<typename... Ts>
std::unique_ptr<FizzBuzz,decltype(delFunc)> makeFizzBuzz(Ts&&... params){
	std::unique_ptr<FizzBuzz,decltype(delFunc)> u_ptr(nullptr,delFunc);
	u_ptr.reset(new FizzBuzz(std::forward<Ts>(params)...));
	return u_ptr;	
};

void print_localtime() {
  std::time_t result = std::time(nullptr);
  std::cout << std::asctime(std::localtime(&result));
}

template<typename T,size_t N>
size_t array_size(T (&)[N])
{
	return N;
}

class UF_String{
	private:
		unordered_map<string,string> id;
		unordered_map<string,int> weights;
	public:
		UF_String(const vector<pair<string,int>>& k2v){
				
			for(int i=0;i<k2v.size();i++)
			{
				weights[k2v[i].first] = k2v[i].second;
				id[k2v[i].first] = k2v[i].first;	
			}
		}
		string find_root(string child)
		{
			while(child!=id[child])	
				child = id[child];
			return child;
		}
		bool is_root(const string& key){
			return key==id[key];	
		}
		int get_weight(const string& key)
		{
			return weights[key];	
		}
		void join(const string& child1,const string& child2)
		{
			string parent1 = find_root(child1); 
			string parent2 = find_root(child2);
			if(parent1==parent2) return;	
			if(weights[parent1] >= weights[parent2])
			{
				weights[parent1] += weights[parent2];	
				id[parent2] = parent1;
			}
			else
			{
				weights[parent2] += weights[parent1];	
				id[parent1] = parent2;
			}
		}

};
void combinedKey(const vector<pair<string,int>>& k2v,
		 const vector<vector<string>>& synom)
{
	if(k2v.empty()) return;
	UF_String uf_str(k2v);	

	for(int i=0;i<synom.size();i++)
	{
		string parent = synom[i][0];
		for(int j=1;j<synom[i].size();j++)	
		{
			string child = synom[i][j];
		  	uf_str.join(parent,child);	
		}
	}
	for(int i=0;i<k2v.size();i++)
	{
		string key = k2v[i].first;
		if(uf_str.is_root(key)	)
		{
			cout<<	key << " " << uf_str.get_weight(key) << endl;
		}
	}
	return;
}

int f11(shared_ptr<UF_String> sp){
	return 1;
}

double f12(unique_ptr<UF_String> up){
	return 0.0;
}

bool f13(UF_String* rptr)
{
	return true;
}
mutex g11,g12,g13;

template<typename FuncType,
	 typename MuxType,
	 typename PtrType>
auto lockAndCall(FuncType func,MuxType& mux,PtrType ptr)->decltype(func(ptr)){
	using MutexGuard = lock_guard<MuxType>;
	MutexGuard g(mux);
	return func(ptr);
}    

int shortestPathBinaryMatrix(vector<vector<int>>& grid) {
        int n = grid.size(),m = grid[0].size();
        if(n==0) return 0;
        if(grid[0][0]==1) return -1;
        if(n==1) return 1;
       //int dir_x[] = {1,1, 1,0,0,-1,-1,-1};
       //int dir_y[] = {1,-1,0,1,-1,1,-1, 0};
        int dir_x[] = {0,0,1,-1};
        int dir_y[] = {1,-1,0,0};
        vector<int> parents(n*m,INT16_MAX);
        queue<pair<int,int>> q;
        q.push({0,0});
        bool found = false;
        int lvl = 1;
        while(!q.empty()&& !found)
        {
            auto sz = q.size();
            for(int i=0;i<sz&&!found;i++)
            {
                auto front = q.front();
                q.pop();
                //for(int j=0;j<8;j++)
                for(int j=0;j<4;j++)
                {
                    int new_x = front.first + dir_x[j];
                    int new_y = front.second+ dir_y[j];
                    if(new_x<0||new_y<0||new_x>=n||new_y>=m)
                        continue;
                    if(grid[new_x][new_y]==1) continue;
                    parents[new_x*m+new_y] = front.first*m + front.second;
                    //if(new_x==n-1 && new_y==n-1)
		    if(grid[new_x][new_y]==9)
                    {
                        found = true;
                        break; 
                    }
                    grid[new_x][new_y] = 1;
                    q.push({new_x,new_y});
                }
            }
            lvl++;
        }
        vector<pair<int,int>> paths;
        int i = 3*m-1;
	parents[0]=0;
        while(parents[i]!=i)
        {
	   cout<< i << " " << i/m << " " << i%m<<endl;
           paths.push_back({i/m,i%m});
           i = parents[i]; 
        }
        paths.push_back({0,0});
        reverse(paths.begin(),paths.end());
        for(int i=0;i<paths.size();i++)
        {
            cout<< paths[i].first << "  " << paths[i].second << endl;
        }
        return found?lvl:-1; 
    }
void printChristmasTree(int height)
{
	if(height<1) return;
	height--;
	int last_row_len = 2*height-1;
	string first_row_str = "#";
        for(int i=1;i<=height;i++)	
	{
		int cur_row_len = 2*i-1;
		int nums_space = (last_row_len - cur_row_len)/2;
		string lines(nums_space,' ');	
		if(i==1){
			first_row_str = lines+string(cur_row_len,'#'); 
			lines += string(cur_row_len,'*');
		}
		else if(i<height)
			lines += "#" + string(cur_row_len-2,' ') + "#";
		else
			lines +=string(cur_row_len,'#');
		cout<<lines<<endl;
	}
	cout<<first_row_str<<endl;
	return;
}

typedef void (*FP)(int);
using FPt = void(*)(int); 

template<typename T>
using MyList = list<T>;

template<typename T>
struct MyList2{
		typedef list<T> type;
};

template<typename T>
class Widget{
	MyList<T> l1;
	typename MyList2<T>::type l2;
};
enum color{
	white,
		red,
			yellow,
};
class SparseMatrix{
	friend SparseMatrix operator+(const SparseMatrix& lhs,const SparseMatrix& rhs);
	private:
		int rows;
		int cols;
		unordered_map<int,unordered_map<int,int>> m;
	public:
		SparseMatrix(int r,int c):rows(r),cols(c){}
		SparseMatrix& operator+=(const SparseMatrix& rhs){
			if(rows!=rhs.rows || cols!=rhs.cols)	
				return *this;
			for(auto& row:rhs.m)	
			{
				int r = row.first;
				for(auto& col:row.second)	
				{
					int c = col.first;
					int val = col.second;	
					m[r][c] += val;
					if(m[r][c]==0)
						m[r].erase(c);	
				}
				if(m[r].empty())
					m.erase(r);
			}
			return *this;
		}
		bool set(int x, int y,int val)
		{
			if(x<0||y<0||x>=rows||y>=cols)	
				return false;
			m[x][y] = val;
			return true;
		}
		bool get(int x, int y,int& val)
		{
			if(x<0||y<0||x>=rows||y>=cols)	
				return false;
			if(m.count(x)==0 || m[x].count(y)==0)	
				val = 0;
			else
				val = m[x][y];
			return true;
		}

};
/*
SparseMatrix operator+(const SparseMatrix& lhs,const SparseMatrix& rhs){
	SparseMatrix ans(lhs);
	if(lhs.rows!=rhs.rows || lhs.cols!=rhs.cols)	
		return ans;
	ans += rhs;
	return ans;

}
	*/
void MinesSweeper(int rows,int cols, int num_mines)
{
	if(num_mines > rows*cols) return;
	vector<vector<char>> board(rows,vector<char>(cols,'E'));
	int cnt = 0;
	queue<pair<int,int>> q;
	srand(time(NULL));
	while(cnt < num_mines)
	{
		int rand_i = rand()%(rows*cols);
		int r_mine = rand_i/cols;
		int c_mine = rand_i%cols;	
		cout<<rand_i<< " " << r_mine << "  " << c_mine<<endl;
		if(board[r_mine][c_mine]=='E')
		{
			cnt++;
			board[r_mine][c_mine] = 'M';	
			q.push({r_mine,c_mine});
		}
	}
	int dir_x[] = {1, 1,1, 0,0,-1,-1,-1};
	int dir_y[] = {-1,0,1,-1,1,-1, 0, 1};
	while(!q.empty())
	{
		auto front = q.front();
		q.pop();
		int x = front.first,y = front.second;
		for(int i=0;i<8;i++)	
		{
			int new_x = x + dir_x[i];
			int new_y = y + dir_y[i];	
			if(new_x<0||new_y<0||new_x>=rows||new_y>=cols)
				continue;
			if(board[new_x][new_y]=='M')
				continue;
			if(board[new_x][new_y]=='E')
				board[new_x][new_y] = '1';
			else
				board[new_x][new_y] += 1;
		}
	}
	for(int i=0;i<rows;i++)
	{
		for(int j=0;j<cols;j++)
			cout<< board[i][j] << " ";	
		cout<<endl;
	}
	return;
}
class Widget2{
	using DataType = vector<int>;
	public:
		DataType&& getData() &&
		{
			return move(data);	
		}
		DataType& getData() &
		{
			return data;	
		}
	private:
		DataType data;

};
template<typename C>
auto cbegin(const C& container)->decltype(begin(container))
{
	return begin(container);
}
template<typename C>
auto cend(const C& container)->decltype(end(container))
{
	return end(container);
}
template<typename C,typename V>
void searchAndInsert(C& container,const V& targetValue,const V& insertValue)
{
	auto it = find(cbegin(container),cend(container),targetValue);	
	container.insert(it,insertValue);
	return;
}
int main(int argc, char** argv) {

  MinesSweeper(5,7,4);
  SparseMatrix sm(3,5),sm1(3,5);
  //sm.add(sm1);
  int dir_xx[] = {430,90,13,34,345};
  cout<<*cbegin(dir_xx)<<endl;
  vector<int> dir_xxy{0,90,13,34,345};
  searchAndInsert(dir_xxy,2,999);
  for(int i=0;i<dir_xxy.size();i++)
	  cout<<dir_xxy[i]<<" " ;
  MyList<int> int_list;
  int_list.push_back(0);
  MyList2<int>::type int_list2;
  int_list2.push_back(3);
  Widget<double> w_dbl;

  color c = color::white;	
  int i_c = c;
  struct timeval tv,tv1;
  struct timezone tz;
  std::string who = "world";
  if (argc > 1) {
    who = argv[1];
  }
  printChristmasTree(7);
  vector<vector<int>> grid {
	  {0,0,0,0,0,0,0,0,0}, 
	  {1,1,1,1,1,1,0,0,1}, 
	  {0,0,1,0,0,0,0,1,9}, 
	  {0,0,1,0,0,1,0,1,0}, 
	  {0,0,1,0,0,1,0,1,0}, 
	  {0,0,1,0,0,1,0,0,0}, 
  };

  shortestPathBinaryMatrix(grid); 

  using MutexGuard = lock_guard<mutex>;

  {
 	MutexGuard g1(g11);
        auto result = f11(0);	
  }
  //auto result1 = lockAndCall(f11,g11,0);
  {
 	MutexGuard g2(g12);
        auto result = f12(NULL);	
  
  }
  //auto result2 = lockAndCall(f12,g12,NULL);
  {
 	MutexGuard g3(g13);
        auto result = f13(nullptr);	
  
  }
  auto result3 = lockAndCall(f12,g12,nullptr);

  vector<pair<string,int>> k2v {{"a",1},{"b",2},{"c",2},{"d",1},{"e",3},{"f",2}};
  vector<vector<string>> synom {{"a","f","e"},{"b","c"},{"d"}};	
  combinedKey(k2v,synom);	


  string str_array[] = {"hi_a","bc_d","ddd"};
  cout<< array_size(str_array)<<endl;
  auto p_fizbuz = makeFizzBuzz(10); 
  auto q_fizbuz = makeFizzBuzza(10);
  //p_fizbuz->fizz(printFizz);
  cout<<"size of unique_ptr: " << sizeof(p_fizbuz) << endl;
  cout<<"size of unique_ptr: " << sizeof(q_fizbuz) << endl;
  cout<<"size of int ptr " << sizeof(int*) <<endl;
  int rtn = gettimeofday(&tv,&tz);

  thread A(thread_A);
  thread B(thread_B);
  thread C(thread_C);
  thread D(thread_D);
  //thread I(thread_IDLE);
  A.join();
  B.join();
  C.join();
  D.join();
  cout<< "All threads finished!" << endl;
  Publisher pub;
  square sq;
  sq.draw();
  sq.error("wuhao got an Error");
  shape *p_shape = new square();
  p_shape->draw();
  p_shape->error("wuhao got another Error!");
  p_shape->shape::draw();
  sq.shape::draw();
  //assert(0);
  std::cout << get_greet(who) << std::endl;
  print_localtime();
  return 0;
}
