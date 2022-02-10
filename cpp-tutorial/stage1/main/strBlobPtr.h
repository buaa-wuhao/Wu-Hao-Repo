//#include "strBlob.h"
#include <memory>
#include <initializer_list>
#include <vector>
class strBlob;
using namespace std;
class strBlobPtr{
	typedef vector<string>::size_type size_type;
	public:
		strBlobPtr();
		strBlobPtr(const strBlob& a,size_type sz=0);
		string& deref() const;
		strBlobPtr incr();	
	private:
		weak_ptr<vector<string>> wptr;
		size_type cur;
		shared_ptr<vector<string>> check(size_type i,const string& msg) const;
};
