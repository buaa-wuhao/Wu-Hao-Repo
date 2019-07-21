
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>

// class HasPtr{
// 	friend void swap(HasPtr& lhs,HasPtr& rhs);
// 	//friend bool operator<(const HasPtr& lhs,const HasPtr& rhs);
// 	public:
// 		HasPtr(const std::string &s= std::string()):ps(new std::string(s)),i(0) {}
// 		HasPtr(const char *cs) : ps(new std::string(cs)), i(0) {}
// 		HasPtr(const HasPtr& hasptr);
// 		HasPtr& operator=(const HasPtr& hasptr);
//  		bool operator<(const HasPtr &rhs) const;		
// 		~HasPtr() {delete ps;}
// 		std::string& get() {return (*ps);}
// 		void swap(HasPtr& rhs);
// 	private:
// 		std::string *ps;
// 		int           i;
// };

// void HasPtr::swap(HasPtr& rhs)
// {
// 	std::cout<<"void HasPtr::swap(HasPtr& rhs)" << std::endl;
// 	using std::swap;
// 	swap(ps,rhs.ps);
// 	swap(i,rhs.i);
// 	return;
// }

// bool HasPtr::operator<(const HasPtr& rhs) const
// {
// 	//std::cout<<"bool operator<(const HasPtr& lhs,const HasPtr& rhs)"<<std::endl;
// 	return *ps < *rhs.ps;
// }

// void swap(HasPtr& lhs,HasPtr& rhs)
// {
// 	std::cout<<"void swap(HasPtr& lhs,HasPtr& rhs)" << std::endl;
// 	//using std::swap;
// 	//swap(lhs.ps,rhs.ps);
// 	//swap(lhs.i,rhs.i);
// 	lhs.swap(rhs);
// }

// HasPtr::HasPtr(const HasPtr& hasptr)
// {
// 	this->i = hasptr.i;
// 	this->ps = new std::string( *(hasptr.ps) );
// }

// HasPtr& HasPtr::operator=(const HasPtr& hasptr)
// {
// 	if(this != &hasptr)
// 	{
// 		i = hasptr.i;
// 		delete ps;
// 		ps = new std::string(*(hasptr.ps));

// 	}
// 	return (*this);
// }


class HasPtr{
  friend void swap(HasPtr& lhs,HasPtr& rhs);
  //friend bool operator<(const HasPtr& lhs,const HasPtr& rhs);
  public:
    HasPtr(const std::string &s= std::string()):ps(new std::string(s)),i(0) {}
    HasPtr(const HasPtr& hasptr);
    //HasPtr(const char *cs) : ps(new std::string(cs)), i(0) {}
    HasPtr& operator=(const HasPtr& hasptr);
    bool operator<(const HasPtr &rhs) const;    
    ~HasPtr() {delete ps;}
    const std::string &get() const { return *ps; }
    void swap(HasPtr& rhs);
  private:
    std::string *ps;
    int           i;
};

void HasPtr::swap(HasPtr& rhs)
{
  std::cout<<"void HasPtr::swap(HasPtr& rhs)" << std::endl;
  using std::swap;
  swap(ps,rhs.ps);
  swap(i,rhs.i);
  return;
}

bool HasPtr::operator<(const HasPtr& rhs) const
{
  //std::cout<<"bool operator<(const HasPtr& lhs,const HasPtr& rhs)"<<std::endl;
  return *ps < *rhs.ps;
}

void swap(HasPtr& lhs,HasPtr& rhs)
{
  std::cout<<"swap between " <<*lhs.ps << " and " << *rhs.ps << std::endl;
  using std::swap;
  swap(lhs.ps,rhs.ps);
  swap(lhs.i,rhs.i);
  //lhs.swap(rhs);
}

HasPtr::HasPtr(const HasPtr& hasptr)
{
  this->i = hasptr.i;
  this->ps = new std::string( *(hasptr.ps) );
}

HasPtr& HasPtr::operator=(const HasPtr& hasptr)
{
  if(this != &hasptr)
  {
    i = hasptr.i;
    delete ps;
    ps = new std::string(*(hasptr.ps));

  }
  return (*this);
}

void testHasPtrVec(std::vector<HasPtr> &v) {
  std::cout << "\nBefore sort " << v.size() << " elements:\n";
  for (const auto &e : v)
    std::cout << e.get() << std::endl;
  std::cout << "\nSorting:\n";
  std::sort(v.begin(), v.end());
  std::cout << "\nAfter sort:\n";
  for (const auto &e : v)
    std::cout << e.get() << std::endl;
}

int main()
{
	//HasPtr hp1("wuhao"),hp2("haowu"),hp3("wuzheng"),hp4("lanjigen");
	//std::vector<HasPtr> vv{HasPtr("wuhao"),HasPtr("haowu"),HasPtr("wuzheng"),HasPtr("lanjigen")};
	std::vector<HasPtr> vv{std::string("wuhao"),std::string("haowu"),std::string("wuzheng"),std::string("lanjigen")};
	// std::sort(v.begin(),v.end());
	// //std::cout<< *(hp.ps) << std::endl;
	// for(auto it:v)
	// {
	// 	std::cout<< it.get() << " ";
	// }
  // std::vector<HasPtr> v{ "Hello", "World", "P", "Q", "R", "S", "T", "U",
  //     "ABC", "DEF", "AB", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L",
  //     "ABC", "DEF", "AB", "C", "D", "E", "J", "K", "L", "M", "N" };
  testHasPtrVec(vv);

	return 0;
}