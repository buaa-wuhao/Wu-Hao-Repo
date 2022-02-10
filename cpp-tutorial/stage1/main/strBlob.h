#include <memory>
#include <initializer_list>
#include <vector>
using namespace std;
class strBlobPtr;
class strBlob{
	typedef vector<string>::size_type size_type;
	friend class strBlobPtr;
	public:
		strBlob(){}
		strBlob(const strBlob& str);
		strBlob& operator=(const strBlob& rhs);
		strBlob(const initializer_list<string>& il ):data(make_shared<vector<string>>(il)) {}
		size_type size() const {return data->size();}
		bool empty() const {return data->empty();}
		void push_back(const string& str) {data->push_back(str);}
		void pop_back();
		string& front() const;
		string& back() const;
		strBlobPtr begin() const; 
		strBlobPtr end() const; 
	private:
		shared_ptr<vector<string>> data;
		void check(size_type i, const string& msg) const;

};
