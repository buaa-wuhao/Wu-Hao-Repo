#include <mutex>
#include <iostream>
#include <condition_variable>
#include <semaphore.h>
#include <vector>
#include <functional>
#include <thread>
#include <initializer_list>
#include <unordered_set>
#include <chrono>
#include <queue>
#include <exception>
using namespace std;

class myAllocator{
    private:
       size_t blk_sz; 
       size_t blk_num;
       char*  blk_head;
       size_t blk_used;
       size_t blk_sz_with_header;
       struct Block{
          Block* next; 
          size_t size;
       };
       Block* blk_list;
       void push(Block* ptr){
            if(ptr==nullptr) 
                return ;
            ptr->next = blk_list;
            blk_list = ptr;
            return;
       }
       Block* pop(){
            if(blk_list==nullptr) 
              return nullptr;
            Block* next = blk_list->next;
            Block* rtn = blk_list; 
            blk_list = next;
            return rtn;
       }
      
    public:
      myAllocator(size_t block_size,size_t block_num):blk_sz(block_size),blk_num(block_num){
            blk_sz_with_header = (block_size + sizeof(Block)-1)/sizeof(Block) + 1;
            blk_head = new char [blk_sz*block_num]; 
            if(blk_head==nullptr)
                throw runtime_error("No Memory!");
            blk_used = 0; 
            blk_list = nullptr;
      } 
      
      void* allocate(size_t sz){
            if(sz > blk_sz) 
              return nullptr;
            Block* rtn = pop();
            if(rtn)
              return (void*)(rtn+1);
            if(blk_used==blk_num)
              return nullptr;
           
            rtn = (Block*)(blk_head + blk_used*blk_sz_with_header);
            rtn->size = blk_sz;
            blk_used++;
            return (void*)(rtn+1);
      }
      
      void free(void* ptr)
      {
          Block* bp = (Block*) ptr;
          bp--;
          push(bp); 
          return; 
      }
      
      ~myAllocator(){
          delete [] blk_head; 
      }
};
// To execute C++, please define "int main()"
class Zemphore{
    private:
      int use_cnt;
      mutex mtx;
      condition_variable cv;
    public:
      Zemphore(int n):use_cnt(n){} 
      void wait(){
          unique_lock<mutex> lck(mtx); 
          use_cnt--;
          while(use_cnt<0)
            cv.wait(lck);
          return;
      }
      void post(){
          unique_lock<mutex> lck(mtx); 
          use_cnt++;
          if(use_cnt<=0)
            cv.notify_one();
          return;
      }
};

class Read_Write_Lck{
    private:
      int read_cnt; 
      sem_t    lck;
      sem_t    rd_lck;
    public:
      Read_Write_Lck():read_cnt(0){
          sem_init(&lck,0,1); 
          sem_init(&rd_lck,0,1);
      } 
  
      void read_lock(){
         sem_wait(&lck);
         read_cnt++; 
         if(read_cnt==1)  
            sem_wait(&rd_lck);
         sem_post(&lck);
      } 
    
      void read_release(){
         sem_wait(&lck);
         read_cnt--; 
         if(read_cnt==0) 
           sem_wait(&rd_lck);
         sem_post(&lck);  
      }
  
      void write_lock(){
         sem_wait(&rd_lck); 
      }
  
      void write_release(){
         sem_post(&rd_lck);
      } 
    
};


struct ListNode{
  int key;
  int val;
  ListNode* next;
  ListNode(int k,int v):key(k),val(v),next(nullptr){}
  ListNode():key(0),val(0),next(nullptr){}
};

class LinkedList{
    private:
      ListNode* head;
      mutex     mtx;
    public:
      //LinkedList(const LinkedList&) = delete;
  
      LinkedList(){
        head = new ListNode(0,0); 
      }
      ListNode* listLookUp(int key){
          unique_lock<mutex> lck(mtx); 
          ListNode* p = head->next;
          while(p){
            if(p->key==key) 
                return p;
            p = p->next;
          }
          return nullptr;     
      }
      void addKeyValPair(int key,int val){
          unique_lock<mutex> lck(mtx);
          ListNode* node = listLookUp(key);
          if(node)
            node->val = val;
          else{
            node = new ListNode(key,val); 
            node->next = head->next;
            head->next = node;
          }
          return;
      } 
      void removeKey(int key){
         unique_lock<mutex> lck(mtx); 
         ListNode* node = listLookUp(key); 
         if(node==nullptr)
           return;
         ListNode* prv = head;
         ListNode* cur = head->next;
         while(cur){
            if(cur->key==key){
                prv->next = cur->next;
                delete cur;
                break;
            } 
            prv = cur;
            cur = cur->next;
         }
         return; 
      }
      bool getKey(int key,int& val){
         unique_lock<mutex> lck(mtx); 
         ListNode* node = listLookUp(key); 
         if(node==nullptr) 
           return false;
         val = node->val;
         return true;
      }
};

class HashTable{
    private:
        vector<LinkedList*> hash;
        size_t buckets_sz;
    public:
        HashTable(size_t n):hash(n,new LinkedList()),buckets_sz(n){}
        void set(int key,int val){
            hash[key%buckets_sz]->addKeyValPair(key,val); 
        }
        bool get(int key,int& val){
             return hash[key%buckets_sz]->getKey(key,val); 
        }
};

class debugDelete{
public:
  template<typename T>
  void operator()(T* ptr) const{
      delete ptr; 
  }
};
template<typename T>
class sharedPointer;

template<typename T>
void swap(sharedPointer<T>& lhs, sharedPointer<T>& rhs)
{
    using std::swap;
    swap(lhs.raw_ptr,rhs.raw_ptr);
    swap(lhs.use_cnt, rhs.use_cnt);
    swap(lhs.del,rhs.del);
    return;
}

template<typename T>
class sharedPointer{
    template<typename F>
    friend void swap(sharedPointer<F>& lhs,sharedPointer<F>& rhs);
    private:
        T*         raw_ptr;   
        size_t     *use_cnt;
        function<void(T*)> del;
        void free(){
            (*(use_cnt))--; 
            if(*use_cnt == 0)
            {
                del(raw_ptr);
                delete use_cnt; 
                raw_ptr = nullptr;
                use_cnt = nullptr;
            }
            return;
        }
    public:
        sharedPointer():raw_ptr(nullptr),use_cnt(nullptr),del(debugDelete()){} 
        explicit sharedPointer(T* p):raw_ptr(p),use_cnt(new size_t(1)),del(debugDelete()){}
        sharedPointer(T* p,const function<void(T*)>& d):raw_ptr(p),use_cnt(new size_t(1)),del(d){}
        sharedPointer(const sharedPointer& other):raw_ptr(other.raw_ptr),use_cnt(other.use_cnt){
            (*use_cnt)++;     
            del = other.del;
        }
        sharedPointer(sharedPointer&& other) noexcept:raw_ptr(other.raw_ptr),use_cnt(other.use_cnt),del(move(other.del)){
            other.raw_ptr = other.use_cnt = nullptr; 
        }
        sharedPointer& operator=(const sharedPointer& rhs){
            if(this != &rhs){
              free(); 
              raw_ptr = rhs.raw_ptr;
              use_cnt = rhs.use_cnt;
              del     = rhs.del;
              *use_cnt++;
            }    
            return *this;
        }
        sharedPointer& operator=(sharedPointer&& rhs) {
            if(this != &rhs){
              free(); 
              raw_ptr = rhs.raw_ptr;
              use_cnt = rhs.use_cnt;
              del     = move(del);
              rhs.raw_ptr = nullptr; 
              rhs.use_cnt = nullptr;
            } 
            return *this; 
        }
        ~sharedPointer(){
            free(); 
        }
        size_t get_use_cnt() const{
            return (*use_cnt); 
        }
  
        T& operator*() const{
          return *raw_ptr; 
        }
      
        T* operator->() const{
          return &(this->operator*()); 
        }
        
        void reset(T* p,const function<void(T*)>& d){
            if(p!=raw_ptr)
            {
              free(); 
              raw_ptr = p;
              use_cnt = new size_t(1);
              del = d;
            }
            return;
        }
};

class threadGuard{
    private:
      thread t;
    public:
      threadGuard(thread thrd):t(move(thrd)){} 
      threadGuard(const threadGuard&) = delete;
      threadGuard& operator=(const threadGuard&) = delete;
      ~threadGuard(){
          t.join();
      }
};
struct do_work{
    void operator()(int i){
      cout<<"do work ! "<< i<<endl; 
    }
};
//void do_work(int i){
//    cout <<"do work"<< i <<endl;
//}

template<typename T,typename D>
class uniquePointer;

template<typename U,typename F>
void swap(uniquePointer<U,F>& lhs,uniquePointer<U,F>& rhs){
    using std::swap;    
    swap(lhs.raw_ptr,rhs.raw_ptr);
    swap(lhs.del,    rhs.del);
    return;
}

template<typename T,typename D=debugDelete>
class uniquePointer{
  template<typename U,typename F>
  friend void swap(uniquePointer<U,F>& lhs,uniquePointer<U,F>& rhs);
  private:
    T*   raw_ptr; 
    D    del;
    void free(){
        del(raw_ptr); 
        raw_ptr = nullptr;
    }
  public:
    explicit uniquePointer(T* ptr):raw_ptr(ptr),del(D()){} 
    uniquePointer():raw_ptr(nullptr),del(D()){}
    uniquePointer(T* ptr,D d):raw_ptr(ptr),del(d){}
    uniquePointer(uniquePointer&& other) noexcept:raw_ptr(other.raw_ptr),del(move(other.del)){
        other.raw_ptr = nullptr; 
    }
    uniquePointer& operator=(uniquePointer&& other) noexcept{
      if(this!=&other)
      {
        free(); 
        raw_ptr = other.raw_ptr;
        del = move(other.del);
        other.raw_ptr = nullptr;
      }
      return *this;
    }
    T* release(){
      T* rtn = raw_ptr; 
      raw_ptr = nullptr;
      return rtn;
    } 
    
    void reset(T* ptr,D d=D()){
      if(ptr!=raw_ptr)
      {
        free(); 
        raw_ptr = ptr;
        del = d;
      }
      return; 
    }
    T& operator*() const{
      return *raw_ptr; 
    } 
    
    T* operator->() const{
      return &(this->operator*()); 
    }
    
    operator bool() const{
      return raw_ptr!=nullptr; 
    }
     
    uniquePointer(const uniquePointer&) = delete;
    uniquePointer& operator=(const uniquePointer&) = delete;
    ~uniquePointer(){
      free(); 
    } 
};

template<typename T>
class myVector{
    private:
      T*     start;
      T*     first_avail;
      T* cap;
      static allocator<T> alloc_; 
  
      pair<T*,T*> allocate_n_copy(T* b,T* e){
          size_t sz = e - b; 
          T* new_b = alloc_.allocate(sz);
          T* new_e = uninitialized_copy(b,e,new_b);
          return make_pair(new_b,new_e); 
      }
  
      void check_n_copy(){
          if(size()==capacity()) 
            reallocate();
      }
      void reallocate(){
          size_t new_sz = (size()==0)?1:size()*2; 
          T* new_b = alloc_.allocate(new_sz);
          T* ptr = new_b;
        
          for(T* p=start;p<first_avail;p++){
            alloc_.construct(ptr++,move(*p)); 
          }
          free();
          start = new_b;
          first_avail = ptr;
          cap= start + new_sz;
          return;
      }
  
      void free(){
          if(start){
            
            for(T* p=first_avail;p > start;)
            {
                alloc_.destroy(--p); 
            }
            alloc_.deallocate(start, cap-start); 
          }
          return;
      }
    public:
      myVector():start(nullptr),first_avail(nullptr), cap(nullptr){}
      
      myVector(const initializer_list<T>& l):myVector(){
          for(auto& it: l){
              push_back(it); 
          } 
          cout<< "myVector constructor" << endl;
      }
    
      myVector(const myVector& other){
        pair<T*,T*> p = allocate_n_copy(other.start,other.first_avail);          
        start = p.first;
         cap= first_avail = p.second;
      }
  
      myVector(myVector&& other) noexcept:start(other.start),first_avail(other.first_avail),
           cap(other.cap) {
          other.start = other.first_avail = other.cap= nullptr; 
      }
  
      myVector& operator=(const myVector& other){
          if(this != &other) 
          {
              free(); 
              auto p = allocate_n_copy(other.start,other.first_avail);
              start = p.first;
               cap= first_avail = p.second;
          }
          return *this; 
      }
      
      myVector& operator=(myVector&& other) noexcept{
              if(this != &other){
                  free();        
                  start = other.start;
                   cap= first_avail = other.first_avail;
                  other.start = nullptr;
                  other.first_avail = nullptr;
              } 
              return *this; 
      }
      
      void reserve(size_t sz){
          if(sz < capacity())    
            return;
          T* new_b = alloc_.allocate(sz);
          T* p = new_b;
          for(T* ptr=start;ptr!=first_avail;ptr++){
              alloc_.construct(p++,move(*ptr)); 
          }
          free();
          start = new_b;
          first_avail = p;
          cap = new_b + sz; 
          return; 
      }
   
      void resize(size_t sz,const T& elem=T()){
          if(sz<size()) 
          {
              for(T* ptr=start+sz;ptr!=first_avail;ptr++){
                  alloc_.destroy(ptr); 
              } 
              first_avail = start + sz;
              return;
          }
          if(sz < capacity()){
              for(T* ptr=start+size();ptr!=sz+start;ptr++){
                  alloc_.construct(ptr,elem); 
              } 
              first_avail = sz+start;
              return; 
          }
          T* new_b = alloc_.allocate(sz);
          T* p = new_b;
          for(T* ptr=start;ptr!=first_avail || p!=new_b+sz;)
          {
              if(ptr < first_avail)
                alloc_.construct(p++,move(*ptr++)); 
              else
                alloc_.construct(p++,elem);
          }
          free();
          start = new_b; 
          first_avail = p;
          cap = start + sz;
          return;
      
      } 
      void push_back(const T& elem){
          check_n_copy(); 
          alloc_.construct(first_avail++,elem);
          return;
      } 
      
      void pop_back(){
          if(empty())
            return;
          alloc_.destroy(--first_avail);
          return;
      } 
  
      T* begin() const{
        return start; 
      } 
  
      T* end() const{
        return first_avail; 
      }
  
      T& back() const{
        return *(first_avail-1); 
      }
      T& front() const{
        return *start; 
      }
      size_t size() const{
          return first_avail - start; 
      } 
      bool empty() const{
          return start==first_avail; 
      }
      size_t capacity() const{
          return  cap- start; 
      }
      ~myVector(){
         free();
      }

};

template<typename T>
allocator<T> myVector<T>::alloc_;



template<typename T,typename U=less<T>>
class myPQ{
    private:
      vector<T> data; 
      U         comp;
      int parent(int i){
        if(i==0) 
          return -1;
        return (i-1)/2;
      } 
      int y_child(int i){
        return 2*i + 1; 
      }
      
      void bubble_up(int i){
        int parent_i  = parent(i); 
        if(parent_i < 0)
          return;
        if(comp(data[parent_i],data[i]))
        {
            swap(data[parent_i],data[i]); 
            bubble_up(parent_i); 
        }
        return;
      }
      
      void bubble_down(int i){
        int bigger = i; 
        int l_child = y_child(i),r_child = l_child+1;
        int sz = data.size();
        if(l_child<sz && comp(data[bigger],data[l_child]))
        {
            bigger = l_child; 
        }
        if(r_child<sz && comp(data[bigger],data[r_child])){
            bigger = r_child; 
        }
        if(bigger!=i)
        {
          swap(data[i],data[bigger]); 
          bubble_down(bigger);
        }
        return;
      }
      void heapify(){
        int sz = data.size();
        for(int i=(sz-1);i>=0;i--){
          bubble_down(i); 
        } 
        return;
      }
    public:
      myPQ():comp(U()){} 
      myPQ(const initializer_list<T>& il):comp(U()){
        for(auto& it:il){
          data.push_back(it);
        } 
        heapify();
      }
      void push(const T& elem){
          data.push_back(elem);
          bubble_up(data.size()-1);
      }
      size_t size() const{
          return data.size(); 
      }
      T  top() const{
        if(empty())
          throw out_of_range("empty Queue!");
        return data.front(); 
      }
      bool empty() const{
        return data.empty(); 
      } 
      void pop() {
          if(empty()) 
              return;
          data[0] = data.back();
          data.pop_back();
          bubble_down(0);
          return;
      }
      
};

struct Header{
  Header* next;
  size_t  size;
};


class Allocator{
    private:
      const size_t page_size = 1024;
      static Header* free_list;
      static Header  base;
      unordered_set<char*> pool;
      Header* morecore(size_t sz){
        if(sz < page_size) 
            sz = page_size; 
        char* new_p = new char [sz*sizeof(Header)];
        pool.insert(new_p);
        if(new_p == nullptr)
            return nullptr; 
        Header* bp = (Header*) new_p; 
        bp->size = sz;
        free((void*)(bp+1));
        return free_list; 
      } 
    public:
      void* allocate(size_t nbytes){
        size_t sz = (nbytes + sizeof(Header)-1)/sizeof(Header) + 1;
        Header *prv,*cur;
        if((prv=free_list)==nullptr){
            base.next = prv = free_list = &base; 
            base.size = 0;
        }
        for(cur = prv->next;;prv = cur,cur = cur->next)
        {
            if(cur->size >= sz){
                if(cur->size == sz) 
                {
                    prv->next = cur->next; 
                }
                else{
                    cur->size -= sz;
                    cur += cur->size;
                    cur->size = sz;
                }
                free_list = prv;
                return (void*)(cur+1);
            } 
            if(cur==free_list){
              cur = morecore(sz); 
              if(cur==nullptr)
                return nullptr;
            }
        }
        return nullptr;
      }
  
      void free(void* ptr){
         if(ptr==nullptr)         
           return;
         Header* bp = (Header*) ptr;
         bp--;
         Header* p; 
         for(p = free_list;!(bp > p && bp < p->next);p = p->next){
              if(p->next <= p && (bp >p || bp < p->next))
                break;
         } 
         if(bp + bp->size == p->next){
            bp->size += p->next->size; 
            bp->next = p->next->next;
         }
         else{
            bp->next = p->next; 
         }
         if(p + p->size == bp){
            p->size += bp->size; 
            p->next = bp->next;
         } 
         else{
            p->next = bp; 
         } 
         free_list = p; 
         return;
      }
  
      ~Allocator(){
          for(auto it:pool){
            delete [] it; 
          } 
      }
};

Header* Allocator::free_list = nullptr;
Header  Allocator::base;



struct Task{
    size_t             task_id;
    function<void()>   cmd;
    Task(size_t id, const function<void()>& command):task_id(id),cmd(command){}
};

using myTimePoint = chrono::system_clock::time_point;

struct timeComp{
    bool operator()(const pair<myTimePoint,Task>& lhs,
                    const pair<myTimePoint,Task>& rhs) const{
            return  lhs.first > rhs.first;
    }
};

class taskScheduler {
     private:
  
           bool    is_alive;
           thread  bckgrd_thrd; 
           mutex   mtx; 
           condition_variable cv;
           int    task_id;  
           vector<thread> pools; 
  
           priority_queue<pair<myTimePoint,Task>,vector<pair<myTimePoint,Task>>,
                        timeComp> pq;
  
           void backgroundTask(){
                while(true){
                  
                    unique_lock<mutex> lck(mtx);
                    while(is_alive && pq.empty()){
                      cv.wait(lck);
                    } 
                    if(is_alive==false)
                      return;
                    auto time_now = chrono::system_clock::now();
                    auto top = pq.top();
                    if(top.first <= time_now){
                        pq.pop(); 
                        //thread t1(top.second.cmd);
                        //t1.detach();
                        pools.emplace_back(top.second.cmd);
                        //thread1.detach();
                    }
                    else{
                        cv.wait_until(lck,top.first); 
                    }
                }
                
           }
     public:
        taskScheduler():is_alive(true),bckgrd_thrd(&taskScheduler::backgroundTask,this){
              task_id = 0;                            
        }
        void endTaskScheduler(){
            unique_lock<mutex> lck(mtx);
            is_alive = false; 
            cv.notify_all();
            return;
        } 
        void addTask(const function<void()>& cmd, chrono::seconds sec){
             unique_lock<mutex> lck(mtx); 
             auto time_to_exec = chrono::system_clock::now() + sec;
             pq.push({time_to_exec,Task(task_id++,cmd)});  
             cv.notify_all(); 
             return; 
        } 
	bool empty() const{
		return pq.empty();	
	}
        ~taskScheduler(){
            endTaskScheduler();
            bckgrd_thrd.join();
            for(auto& it: pools){
              it.join(); 
            }
        }
};




int main() {
  cout<<thread::hardware_concurrency()<<endl;
  auto func = [](int i){cout<<"lamda do work "<< i << endl;};
  //thread thrd(func,585); 
  threadGuard thrd_gd(thread(func,656));
  vector<thread> thrds; 
  for(int i=0;i<10;i++)
  {
      thrds.emplace_back(func,i);
  }
  for(auto& it:thrds){
    it.join(); 
  }
  
  myAllocator my_alloc(64,100);
  void* ptr = my_alloc.allocate(10);
  if(ptr==nullptr) cout<<"empty pointer"<<endl;
  my_alloc.free(ptr); 
  Zemphore z(10);
  Read_Write_Lck rw_lck;
  sharedPointer<myAllocator> shared_ptr_alloc(new myAllocator(64,100));
  uniquePointer<myAllocator> unique_ptr_alloc(new myAllocator(64,100));
  Allocator alloc_;
  for(int i=0;i<20;i++){
    ptr = alloc_.allocate(rand()%1024);
    alloc_.free(ptr); 
  }
  myVector<string> nums { "Hello, ", "World!!", "\n" };
  //myVector<int> nums {1,2,3,4};
  nums.resize(1,"wuhao");
  cout<<nums.size() << endl;
  for (auto& word : nums) {
    cout << word;
  }
  nums.resize(6,"hw");
  cout<<nums.size()<<endl;
  
  for (auto& word : nums) {
    cout << word;
  }
  nums.pop_back();
  cout<<nums.size()<<endl;
  
  for (auto& word : nums) {
    cout << word;
  }
  
  myPQ<int,less<int>> pq {2,34,5,6,88,15};
  while(!pq.empty()){
    cout << pq.top() << " "; 
    pq.pop();
  }
  
  for(int i=0;i<20;i++)
    pq.push(rand()%100); 
  cout<<"size "<< pq.size()<< endl;
  while(!pq.empty()){
    cout << pq.top() << " "; 
    pq.pop();
  }
  
   taskScheduler task_scheduler;
   auto task1 = [](){
       time_t tt = chrono::system_clock::to_time_t(chrono::system_clock::now()) ; 
       cout<<"task1 "<< ctime(&tt) << endl; 
       //cout<<"task1 woo hoo" << endl; 
   };
   auto task2 = [](){
      time_t tt = chrono::system_clock::to_time_t(chrono::system_clock::now()) ; 
      cout<<"task2 "<< ctime(&tt)<<endl;
      //cout<<"task2 "<< endl;
     int i = 100;
     i++;
   };
   cout<< endl;
   time_t tt = chrono::system_clock::to_time_t(chrono::system_clock::now()) ; 
   cout<<"cur time: "<< ctime(&tt) << endl;
 // try{
   task_scheduler.addTask(task1,chrono::seconds(5));
//  }
  //  catch(exception& e)
  //  {
  //    cout<< e.what() << endl; 
  //  }
   task_scheduler.addTask(task2,chrono::seconds(10)); 
  cout<<"ending !" <<endl;

  while(!task_scheduler.empty()){
 	this_thread::sleep_for(chrono::seconds(1)); 
  }
  return 0;
}

