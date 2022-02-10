#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <algorithm>
#include <unordered_set>

using namespace std;

struct File{
	string file_name;
	int type;
	size_t sz;
	bool is_directory;
	vector<File*> directory;
};

class base_filter{
	public:
		virtual bool operator()(const File& file) const = 0;
		virtual ~base_filter() = default;
};

class searchBySize:public base_filter{
	public:
		searchBySize(int sz):file_sz(sz){}
		virtual bool operator()(const File& file) const override{
			if(file.sz>=file_sz)
				return true;
			return false;
		}
	private:
		int file_sz;

};

class searchByType:public base_filter{
	public:
		searchByType(int type):file_type(type){}
		bool operator()(const File& file) const{
			if(file.type==file_type)
				return true;
			return false;
		}
	private:
		int file_type;
};

class fileFind{
	public:
		fileFind(const File& file):file_properties(new unordered_map<string,const File*>){
			file_search(file);	
		}
		vector<string> find(base_filter& filter) const{
			vector<string> ans;
			for(auto& it:(*file_properties))
			{
				if(filter(*(it.second)))
					ans.push_back(it.first);
			}
			return ans;
		}
		shared_ptr<unordered_map<string,const File*>> getAllFile() const{
			return file_properties;
		}

	private:
		shared_ptr<unordered_map<string,const File*>> file_properties;
		void file_search(const File& file){
			(*file_properties)[file.file_name] = &file;
			if(file.is_directory)
			{
				for(auto& it:file.directory)
					file_search(*it);
			}
		}

};

class Find;
class base_find{
	friend class Find;
	protected:
		virtual ~base_find() = default;
	private:
		virtual vector<string> eval(const fileFind& ff) const = 0;
};


class single_find:public base_find{
	public:
		single_find(const shared_ptr<base_filter>& cbf):bf(cbf){}
		virtual vector<string> eval(const fileFind& ff) const override{
			return ff.find(*bf);
		}
	private:
		shared_ptr<base_filter> bf;
};
class Find{ 
	friend Find operator&(const Find& lhs,const Find& rhs);
	friend Find operator|(const Find& lhs,const Find& rhs);
	friend Find operator~(const Find& other);
	public:
		Find(const shared_ptr<base_filter>& bf):bf(new single_find(bf)){}
		vector<string> eval(const fileFind& ff) const{
			return bf->eval(ff);
		}	
	private:
		Find(const shared_ptr<base_find>& cbf):bf(cbf){}
		shared_ptr<base_find> bf;
};

class binary_find:public base_find{
protected:
	binary_find(const Find& l,const Find& r):lhs(l),rhs(r){}
	Find lhs;
	Find rhs;
};

class not_find:public base_find{
	friend Find operator~(const Find& other);
	public:
	virtual vector<string> eval(const fileFind& ff) const override{
		vector<string> res = fd.eval(ff);
		auto allFile = ff.getAllFile();
		unordered_set<string> res1(res.begin(),res.end());
		vector<string> ans;
		for(auto& it:*allFile)
		{
			if(res1.count(it.first)) continue;
			ans.push_back(it.first);
		}
		return ans;
	}
	private:
	not_find(const Find& other):fd(other){}
	Find fd;
};

Find operator~(const Find& other)
{
	return Find(shared_ptr<base_find>(new not_find(other)));
}

class and_find:public binary_find{
	friend Find operator&(const Find& lhs,const Find& rhs);
	public:
	virtual vector<string> eval(const fileFind& ff) const override{
		vector<string> left = lhs.eval(ff);
		vector<string> right = rhs.eval(ff);
		sort(left.begin(),left.end());
		sort(right.begin(),right.end());
		vector<string> merged;
		set_intersection(left.begin(),left.end(),right.begin(),right.end(),merged.begin());
		return merged;
	}
	private:
	and_find(const Find& lhs,const Find& rhs):binary_find(lhs,rhs){}
};
Find operator&(const Find& lhs, const Find& rhs){
	return Find(shared_ptr<base_find>(new and_find(lhs,rhs)));
}
class or_find:public binary_find{
	friend Find operator|(const Find& lhs,const Find& rhs);
	or_find(const Find& lhs,const Find& rhs):binary_find(lhs,rhs){}

	public:
	virtual vector<string> eval(const fileFind& ff) const override{
		vector<string> left = lhs.eval(ff);
		vector<string> right = rhs.eval(ff);
		unordered_set<string> merged(left.begin(),left.end());
		merged.insert(right.begin(),right.end());
		vector<string> ans;
		for(auto& it:merged)
			ans.push_back(it);
		return ans;

	}
};

Find operator|(const Find& lhs, const Find& rhs){
	return Find(shared_ptr<base_find>(new or_find(lhs,rhs)));
}
