#include <iostream>
#include <cstdio>
#include <vector>
#include <functional>
#include <string.h>
#include <semaphore.h>
#include <mutex>
#include <condition_variable>
using namespace std;
class myAlloc{
    public:
        myAlloc(size_t blkNum, size_t blkSZ):blk_num(blkNum),blk_sz(blkSZ){
            blk_head = new char[blk_num*blk_sz]; 
            if(blk_head==nullptr)
                throw runtime_error("No memory");
            blk_used = 0;
            p_list = nullptr;
            
        }
        void* allocate(size_t blkSZ){
           if(blkSZ > blk_sz) 
           {
               //cout<<"Block Size is over the limit"<<endl;
               throw runtime_error("Block size over the limit");
               return nullptr;
           }
           Block *rtn = pop();
           if(rtn)
                return (void*) rtn;
            if(blk_used==blk_num)
            {
                throw runtime_error("out of memory");
            }
            rtn = (Block*)(blk_head + blk_used*blk_sz);
            blk_used++;
            return (void*)rtn;
        }
        void deallocate(void* blk){
           push(blk); 
           return;
        }
        ~myAlloc(){
            delete [] blk_head;
        }
   private: 
        char* blk_head;
        size_t blk_num;
        size_t blk_sz;
        size_t blk_used;
        struct Block{
            Block* next;
        };
        Block* p_list;
        void push(void* p_blk){
            Block* p = (Block*)p_blk;
            p->next = p_list;
            p_list = p;
            return;
        }
        Block* pop(){
            if(p_list==nullptr)
                return nullptr;
            Block* nxt = p_list->next;
            Block* rtn = p_list;
            p_list = nxt;
            return rtn;
        }
};
void genChristmasTree(int k)
{
    int n = k-1;
    int len=1,space_cnt = n-1;
    for(int i=n;i>=1;i--)
    {
        char ch = (i==n)?'*':'#';
        string row ;
        if(i>1)
            row = string(space_cnt,' ')+string(1,ch) + (ch!='*'?(string(len-2,' ')+string(1,ch)):"");
        else
            row = string(space_cnt,' ')+string(len,ch); 
        cout<<row<<endl;
        space_cnt--;
        len += 2;
    }
    cout<<string(n-1,' ')+"#"<<endl;
}

template<typename T,typename F=less<T>>
class priorityQueue{
        private:
                vector<T> data;
                F         comp;
                int parent(int i){
                    if(i==0) return -1;
                    return (i-1)/2;
                } 
                int younger_child(int i){
                    return 2*i+1;
                }
                void bubble_up(int idx){
                   while(idx>0 && comp(data[parent(idx)],data[idx]))
                   {
                        swap(data[parent(idx)],data[idx]);
                        idx = parent(idx); 
                   }
                   return;
                }
                void bubble_down(int idx){
                    while(idx<data.size()){
                        int tgt_idx = idx;
                        int child1 = younger_child(idx);
                        int child2 = child1+1;
                        if(child1<data.size() && comp(data[tgt_idx],data[child1]))
                            tgt_idx = child1;
                        if(child2<data.size() && comp(data[tgt_idx],data[child2]))
                            tgt_idx = child2;
                        if(idx!=tgt_idx)
                        {
                            swap(data[idx],data[tgt_idx]);
                            idx = tgt_idx;
                            continue;
                        }
                        break;
                    }
                    return;
                }
                void heapify(){
                    auto sz = data.size();
                    for(int i=parent(sz-1);i>=0;i--)
                       bubble_down(i); 
                    return;
                }
        public:
                priorityQueue(F cmp=F()):comp(cmp){} 
                
                void push(const T& elem){
                    data.push_back(elem);  
                    bubble_up(data.size()-1);
                } 
                void pop(){
                    if(empty())
                        throw out_of_range("empty queue!");
                    data[0] = data.back(); 
                    data.pop_back();
                    bubble_down(0);
                }
                T top() const{
                   if(empty()) 
                        throw out_of_range("empty queue!");
                   return data[0]; 
                }
                bool empty() const{
                    return data.empty();
                }
                size_t size() const{
                    return data.size();
                }
};
class Allocator{
    private:
        const size_t page_size = 1024;
        static Header* free_list; 
        static Header base; 
        Header* morecore(size_t sz){
            if(sz<page_size) 
              sz = page_size;
            char* new_p = new char [sz*sizeof(Header)];
            if(new_p==nullptr) return nullptr;
            Header* up = (Header*) new_p;
            up->size = sz;
            free((void*)(up+1));
            return free_list;
        }
    public:
      void* allocate(size_t nbytes){
          size_t sz = (nbytes + sizeof(Header) - 1)/sizeof(Header) + 1; 
          Header *prv,*cur;
          if((prv=free_list)==nullptr)
          {
              base.next = prv = free_list = &base; 
              base.size = 0;
          }
          
          for(cur=prv->next;;prv=cur,cur=cur->next)
          {
              if(cur->size >= sz){
                if(cur->size==sz){
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
      void  free(void* ptr){
            Header* bp = (Header*) ptr; 
            bp--;
            Header* p; 
            for(p=free_list;!(bp>p && bp<p->next);p=p->next)
            {
                if(p>p->next && (bp>p || bp<p->next)){
                    break; 
                } 
            }
            if(bp+bp->size == p->next){
                bp->size += p->next->size;
                bp->next = p->next->next;
            }
            else{
                bp->next = p->next; 
            }
            
            if(p+p->size == bp){
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
         
      }

};
Header* Allocator::free_list = nullptr;
Header  Allocator::base;
class debugDelete{
    public:
    template<typename T>
    void operator()(T* ptr) const{
        delete ptr;
    }
};

template<typename T>
class sharedPointer;

template<typename F>
inline void swap(sharedPointer<F>& lhs,sharedPointer<F>& rhs)
{
    using std::swap;
    swap(lhs.raw_ptr,rhs.raw_ptr);
    swap(lhs.use_cnt,rhs.use_cnt);
    swap(lhs.del, rhs.del);
    return;
}

template<typename T>
class sharedPointer{
    template<typename F>
    friend void swap(sharedPointer<F>& lhs,sharedPointer<F>& rhs);
    private:
        T* raw_ptr;
        size_t* use_cnt;
        function<void(T*)> del;
        void free(){
            (*use_cnt)--;     
            if((*use_cnt)==0)
            {
                del(raw_ptr);
                delete use_cnt; 
                raw_ptr = nullptr;
                use_cnt = nullptr;
            }
        }
    public:
        explicit sharedPointer(T* ptr):raw_ptr(ptr),use_cnt(new size_t(1)),del(debugDelete()) {} 
        sharedPointer(T* ptr,const function<void(T*)>& d):raw_ptr(ptr),use_cnt(new size_t(1)),del(d){}
        sharedPointer(const sharedPointer& other):raw_ptr(other.raw_ptr),use_cnt(other.use_cnt),del(other.del){
            (*use_cnt)++;
        } 
        sharedPointer(sharedPointer&& other) noexcept:raw_ptr(other.raw_ptr),use_cnt(other.use_cnt),del(move(other.del)){
            other.raw_ptr = nullptr;
            other.use_cnt = nullptr;
        }
        sharedPointer& operator=(const sharedPointer& other){
           if(this!=&other) 
           {
              free(); 
              raw_ptr = other.raw_ptr;
              use_cnt = other.use_cnt;
              del     = other.del;
              (*use_cnt)++;
           }
           return *this;
        }
        sharedPointer& operator=(sharedPointer&& other) noexcept{
           raw_ptr = other.raw_ptr;
           use_cnt = other.use_cnt; 
           del     = move(other.del);
           other.use_cnt = nullptr;
           other.raw_ptr = nullptr;
           return *this;
        }
        void reset(){
            free();
            return;
        }
        void reset(T* new_ptr,function<void(T*)> new_del=debugDelete()){
            if(new_ptr!=raw_ptr)
            {
                free();
                raw_ptr = new_ptr;
                use_cnt = new size_t(1);
            }
            del = new_del;
            return;
        } 
        T& operator*() const{
            return *raw_ptr;
        }
        
        T* operator->() const{
           return &this->operator*(); 
        } 
        operator bool() const{
            return raw_ptr!=nullptr;
        } 
        
        ~sharedPointer(){
            free();
        }
};

class String{
    friend ostream& operator<<(ostream&,const String& str);
    public:
        String():element(nullptr),first_free(nullptr),cap(nullptr){} 
        String(const char* p_str=""){
           size_t len = strlen(p_str); 
           auto data = allocate_n_copy(p_str,p_str+len);
           element = data.first;
           cap = first_free = data.second;
        }
        String(const String& other){
            auto data = allocate_n_copy(other.element,other.first_free);
            element = data.first;
            first_free = cap = data.second; 
        }
        String(String&& other) noexcept:element(other.element),first_free(other.first_free),cap(other.cap){
            other.element = nullptr;
            other.first_free = nullptr;
            other.cap = nullptr;
        }
        String& operator=(const String& other){
            
            auto data = allocate_n_copy(other.element,other.first_free); 
            free(); 
            element = data.first;
            first_free = cap = data.second; 
            return *this; 
        }
        String& operator=(String&& other) noexcept{
            if(this!=&other)
            {
                free();
                element = other.element;
                first_free = other.first_free;
                cap = other.cap; 
                other.element = other.first_free = other.cap = nullptr;
            }
            return *this; 
        }
        void push_back(char c){
            check_n_alloc();
            *first_free++ = c;
            return;
        }
        size_t size() const{
            return first_free - element;
        }
        size_t capacity() const{
            return cap - element;
        }
        char* begin() const{
            return element;
        }
        char* end() const{
            return first_free;
        }
    private:
        static allocator<char> alloc_;
        char* element;
        char* first_free;         
        char* cap;
        void free(){
           if(element){
               alloc_.deallocate(element,cap-element);
           } 
           element = first_free = cap = nullptr;
           return;
        }
        pair<char*,char*> allocate_n_copy(const char* b,const char* e){
           auto data = alloc_.allocate(e-b); 
           return {data,uninitialized_copy(b,e,data)};
        }
        void check_n_alloc(){
            if(size()==capacity())
                reallocate();
            return;
        } 
        void reallocate(){
            size_t n_sz = (capacity()==0)?1:capacity()*2;
            auto data = alloc_.allocate(n_sz);
            char* new_element = data;
            for(char* p = element;p!=first_free;p++)
            {
               *data  = *p;
               data++;
            }
            free(); 
            element = new_element;
            first_free = data;
            cap = new_element + n_sz;
            return;
        }
};
allocator<char> String::alloc_;

ostream& operator<<(ostream& os,const String& str){
    for(char* p=str.element;p!=str.first_free;p++)    
        os<<*p;
    return os;
}

template<typename T,typename D>
class uniquePointer;

template<typename T,typename D>
void swap(uniquePointer<T,D>& lhs,uniquePointer<T,D>& rhs)
{
    using std::swap;
    swap(lhs.raw_ptr,rhs.raw_ptr);    
    swap(lhs.del, rhs.del);
    return;
}


template<typename T,typename D=debugDelete>
class uniquePointer{
    template<typename U,typename F>
    friend void swap(uniquePointer<U,F>& lhs,uniquePointer<U,F>& rhs);
    private:
       T* raw_ptr;      
       D  del;
    public:   
       uniquePointer():raw_ptr(nullptr),del(D()){} 
       explicit uniquePointer(T* ptr):raw_ptr(ptr),del(D()){}   
       uniquePointer(T* ptr,D d):raw_ptr(ptr),del(d){}
       uniquePointer(uniquePointer&& other) noexcept:raw_ptr(other.raw_ptr),del(move(other.del)){
           other.raw_ptr = nullptr;
       }
       uniquePointer& operator=(uniquePointer&& other) noexcept{
           raw_ptr = other.raw_ptr;
           other.raw_ptr = nullptr;
           del = move(other.del);
           return *this;
       }
       uniquePointer(const uniquePointer& other) = delete;
       uniquePointer& operator=(const uniquePointer& other) = delete;
       ~uniquePointer(){
           del(raw_ptr);
       }
       T* release(){
           T* rtn = raw_ptr;
           raw_ptr = nullptr;
           return rtn;
       } 
       void reset(){
           del(raw_ptr);
           raw_ptr = nullptr;
       }
       void reset(T* p){
           del(raw_ptr);
           raw_ptr = p;
       }
       T& operator*() const{
           return (*raw_ptr);
       }
       T* operator->() const{
           return &(this->operator*());
       }
       operator bool() const{
           return raw_ptr!=nullptr;
       }
};

class zemaphore{
    private:
        int used;
        condition_variable cv;
        mutex mtx;
    public:
        zemaphore(int n):used(n){} 
        void wait(){
           unique_lock<mutex> lck(mtx); 
           used--;
           while(used<0){
               cv.wait(lck);
           }
        }
        void post(){
            unique_lock<mutex> lck(mtx);    
            used++;  
            cv.notify_one();
        }
};

class read_write_lock_t{
    private:
        int read_num;
        sem_t lock;
        sem_t wr_lck;   
    
    public:
        read_write_lock_t():read_num(0){
            sem_init(&lock,0,1);
            sem_init(&wr_lck,0,1);
        }
        
        void accquire_read_lock(){
           sem_wait(&lock); 
           read_num++;
           if(read_num==1)
                sem_wait(&wr_lck);
            sem_post(&lock);
        }
        
        void release_read_lock(){
           sem_wait(&lock); 
           read_num--;
           if(read_num==0)
                sem_post(&wr_lck);
            sem_post(&lock);
        }
        
        void accquire_write_lock(){
            sem_wait(&wr_lck); 
        }
        void release_write_lock(){
            sem_post(&wr_lck); 
        }
};

struct listNode{
    int key;
    int val;
    listNode* next;
    listNode(int k,int v):key(k),val(v),next(nullptr){}
    listNode():key(0),val(0),next(nullptr){}
    void set(int k,int v){
        key = k;
        val = v;
        return;
    }
};

class myList{
    private:
        mutex mtx;
        listNode* head;
    public:
        myList():head(nullptr){}
        void push(int key,int val){
            unique_lock<mutex> lck(mtx);
            listNode* node = new listNode(key ,val);
            node->next = head;
            head = node;
        } 
        void pop(){
            unique_lock<mutex> lck(mtx);
            listNode* node = head; 
            head = head->next;
            delete node;
        }
        listNode* top() {
            unique_lock<mutex> lck(mtx);
            if(isEmpty())
                throw runtime_error("Empty!");
            return head;
        }
        bool list_lookup(int key,int& val){
            unique_lock<mutex> lck(mtx);
            listNode* ptr = head; 
            while(ptr->key!=key)
                ptr = ptr->next;
            if(ptr==nullptr) 
                return false;
            val = ptr->val;
            return true;
        }
        bool isEmpty(){
            unique_lock<mutex> lck(mtx);
            return head==nullptr;
        }
};

class HashTable{
    private:
        vector<myList> bins; 
        size_t bin_sz;
    public:   
        HashTable(int n):bins(n),bin_sz(n){}
        void set(int key,int val){
            bins[key%bin_sz].push(key,val);
            return;
        }
        bool get(int key,int& val){
           if(bins[key%bin_sz].list_lookup(key,val)) 
                return true;
            return false;
        }
        
};

int main() {
    //myAlloc myalloc(100,64);
    //sharedPointer<myAlloc> myalloc(new myAlloc(100,64)); 
    uniquePointer<myAlloc> myalloc(new myAlloc(100,64));
    char* ptr = static_cast<char*>(myalloc->allocate(20));
    printf("%p \n",ptr);
    myalloc->deallocate(ptr);
    priorityQueue<int,greater<int>> pq;
    for(int i=0;i<10;i++)
        pq.push(rand());
    cout<<pq.size()<<"size queue"<<endl;
    while(!pq.empty()) 
    {
        cout<< pq.top()<<" ";
        pq.pop();
    }
    String str("Hello,World! wuhao hoho");
    cout<<str<<endl;
    printf("Hello, World!\n");
    genChristmasTree(4);
    return 0;
}

