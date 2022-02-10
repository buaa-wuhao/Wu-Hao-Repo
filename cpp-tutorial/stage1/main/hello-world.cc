#include "strBlob.h"
#include "strBlobPtr.h"
#include <sys/types.h>
#include <unistd.h>
#include <ctime>
#include <string>
#include <iostream>
//#include <initializer_list>
#include "blob.h"
#include "BlobPtr.h"
#include <algorithm>
#include "strVec.h"
#include <queue>
#include "String.h"
#include "Allocator.h"
#include "vec.h"
#include "sharedPointer.h"
#include "pid.h"
#include "singleton.h"
#include "shared_pointer.h"
#include "unique_pointer.h"
#include "priorityQueue.h"
using namespace std;

class NoVirtual{
	char a;
	public:
	void x() const {}
	int i() const {return 1;}
};

class OneVirtual{
	int a;
	public:
	virtual void x() const {}
	int i() const {return 1;}
};

class TwoVirtual{
	int a;
	public:
		virtual void x() const {}
		virtual void y() const {}
		int i() const {return 1;}
};

std::string get_greet(const std::string& who) {
  return "Hello " + who;
}

void print_localtime() {
  std::time_t result = std::time(nullptr);
  std::cout << std::asctime(std::localtime(&result));
}

class NoDefault{
	public:
		NoDefault(int i):ii(i) {}
	private:
		int ii;

};
class C{
	public:
	 	C(int i=0):nod(i) {}	
		NoDefault nod;
};

class HasPtr{
	friend void swap(HasPtr& lhs,HasPtr& rhs);
 	private:
		string* ps;
		int i;
		size_t* use;	
	public:
		HasPtr(const string& s=""):ps(new string(s)),i(0),use(new size_t(1)){}
		HasPtr(const HasPtr& rhs):ps(new string(*rhs.ps)),i(rhs.i),use(rhs.use)
		{
			(*use)++;	
		}
		HasPtr(HasPtr&& rhs) noexcept:ps(rhs.ps),i(rhs.i),use(rhs.use)
		{
			rhs.ps = nullptr;
			rhs.i = 0;
			rhs.use = nullptr;
		}
		HasPtr& operator=(HasPtr&& rhs) noexcept
		{
			if(this!=&rhs)
			{
				free();
				ps = rhs.ps;
				i = rhs.i;
				use = rhs.use;
				rhs.ps = nullptr;
				i = 0;
				use = nullptr;		
			}
			return *this;
		}
		HasPtr& operator=(const HasPtr& lhs)
		{
			(*lhs.use)++;
			(*use)--;
			if((*use)==0)
			{
				delete ps;
				delete use;
			}
			ps = lhs.ps;
			use = lhs.use;
			i = lhs.i;
			return (*this);
		}
		HasPtr& operator=( HasPtr lhs)
		{
			swap(*this,lhs);	
			return (*this);
		}
		void free()
		{
			(*use)--;
			if((*use)==0)
			{
				delete ps;
				delete use;
			}	
		
		}
		~HasPtr(){
			free();
		}


};
inline void swap(HasPtr& lhs,HasPtr& rhs)
{
	using std::swap;
	swap(lhs.ps,rhs.ps);
	swap(lhs.use,rhs.use);
	swap(lhs.i,rhs.i);
}
template<unsigned N>
constexpr int getSize(const char (&p)[N])
{	
	return N;																
}

template<typename T, unsigned N>
T* begin(const T (&p)[N])
{
	return p;
}
template<typename T,unsigned N>
T* end(const T (&p)[N])
{
	return p+N;	
}
template<typename T, typename V>
T find(T b, T e, const V& val)
{
	for(T it=b;it!=e;++it) 
	{
		if((*it)!=val)
			continue;
		return it;
	}
	return e;
}
template<typename T,unsigned N>
void myPrint(const T (&p)[N])
{
	for(int i=0;i<N;i++)
	{
		cout<< p[i] << " " ;	
	}
}

template <unsigned N, unsigned M>
bool mycomp(const char (&p)[N],const char (&q)[M]) 
{
	return strcmp(p,q);
}

class Vehicle{
	public:
		Vehicle() = default;
		Vehicle(const string& licence, size_t sz):licenceID(licence),vehicleSize(sz){}
		string getID(){
			return licenceID;
		}
		size_t getSize(){
			return vehicleSize;	
		}
		virtual ~Vehicle() = 0;
	private:
		string licenceID;
		size_t vehicleSize;
};

Vehicle::~Vehicle() {}

class Bus : public Vehicle{
	public:
	Bus(const string& licence):Vehicle(licence,3){}
};
class Car : public Vehicle{
	public:
	Car(const string& licence):Vehicle(licence,2){}
};
class Motor:public Vehicle{
	public:
	Motor(const string& licence):Vehicle(licence,1){}
};
int main(int argc, char** argv) {
	mySingleton* p_signleton = mySingleton::instance();	
	//mySingleton p_s3(*p_signleton);
	//mySingleton p_s2 = (*p_signleton);
	//Vehicle veh;

	Bus bus("A1380");
	cout<<bus.getID() << " " << bus.getSize() << endl;
	mySingleton *single = mySingleton::instance();
 	PIDController pid;
	pid.kp = 1;	
	shared_pointer<Bus> ptrBus(new Bus("A1580"));
	shared_pointer<Bus> ptrBus1(ptrBus);
 	cout<< ptrBus->getID() << "  " << ptrBus->getSize() << " " << ptrBus.use_count()<<endl;	
 	ptrBus1.reset();	
 	cout<< ptrBus->getID() << "  " << ptrBus->getSize() << " " << ptrBus.use_count()<<endl;	

	unique_pointer<Bus> uptrBus1(new Bus("A350")),uptrBus2(new Bus("A450"));
  	swap(uptrBus1,uptrBus2);	
	cout<<uptrBus1->getID() << "   " << uptrBus2->getID() << endl;

	shared_ptr<int> pointer;
	sharedPointer<int> ptr_int(new int(0));
	sharedPointer<int> ptr_int2;
	ptr_int2 = ptr_int;
	cout<<"pointer reference " << pointer.use_count() << endl;
	cout<< "wuhao !"<<endl;
	vec<char> vec_char {'a','b','c'};
	vec<int> vec_int = {1,2,3,4,5};
	vec<char> vec_char1(vec_char);
	for(auto it:vec_char1)
		cout<<it << endl;
	//return 0;
	String str("wuhao");
	HasPtr hp ,hp2;
//	hp = hp2;
//	hp = std::move(hp2);
	priorityQueue<pair<string,int>,less<pair<string,int>>> pq;
	pq.push({"a",10});
	pq.push({"b",20});
	pq.push({"b",30});
	pq.push({"c",5});
	while(!pq.empty())
	{
		auto& top = pq.top();
		cout<< "top: " << top.first << " " << top.second<< endl;
		pq.pop();
	}
	//cout<< "top again: " << top << endl;
	vector<int> vv {1,3,4,7};
 	auto low = lower_bound(vv.begin(),vv.end(),8);
	auto upper = upper_bound(vv.begin(),vv.end(),8);
	cout<<"low" << " " << low-vv.begin() << endl; 
	cout<<"upper" << " " << upper-vv.begin()<< endl; 
	strVec str_vec,str_vec1;
	str_vec.push_back("wuhao");
	str_vec.push_back("wuhao2");
        str_vec1 = str_vec;
	cout<<str_vec.size()<<endl;
	cout << "int :" << sizeof(int) << endl;
	cout << "NoVirtual :"<< sizeof(NoVirtual) << endl;
	cout << "void* :" << sizeof(void*) << endl;
	cout<< "oneVirutal :"<<sizeof(OneVirtual) << endl;
	cout<< "twoVirtual :"<<sizeof(TwoVirtual) << endl;
	//NoDefault ndf; //Illegal, because no default constructor
        //vector<NoDefault> v(9);	// Illagal, no default constructor
	vector<C> v(10); // OK, C has default constructor
	C cc;
	
  std::string who = "world";
  strBlob b1={"a","an","the"};
  strBlobPtr b1_ptr(b1);
  Blob<string> b2 = {"ab","bc","the"};
  BlobPtr<string> b2_ptr(b2);
  size_t sz; 
  if (argc > 1) {
    //who = argv[1];
    sz = stoi(argv[1]);
  }
  //std::cout << get_greet(who) << std::endl;
  print_localtime();
  char *p = new char[sz*1024*1024];
  cout<< getpid() << endl;
  while(1)
  {
 	for(int i=0;i<sz*1024*1024;i++) 
		p[i]=0;
  }
  delete [] p;

  return 0;
}
