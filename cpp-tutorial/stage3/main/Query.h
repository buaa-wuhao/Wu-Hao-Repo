#include <string>
#include <unordered_set>

#ifndef TEXT_QUERY_H_
#include "text_query.h"
#endif

class query;
class query_base{
	friend class query;
	protected:
		virtual ~query_base() {} 
	private:
		virtual string rep() const = 0;
		virtual queryResult eval(const textQuery& tq) const =0;
};

class wordQuery:public query_base{
	friend query;
	private:
		wordQuery(const string& word):sought(word){}
		virtual queryResult eval(const textQuery& tq) const override{
			return tq.query(sought);
		}
		virtual string rep() const override{
			return sought;
		}
		string sought;
};

class query{
	friend query operator|(const query& lhs,const query& rhs);
	friend query operator&(const query& lhs,const query& rhs);
	friend query operator~(const query& other);
	public:
		query(const string& word):qb(new wordQuery(word)){}
		queryResult eval(const textQuery& tq) const{
			return qb->eval(tq);
		}
		string rep() const{
			return qb->rep();
		}
	private:
		shared_ptr<query_base> qb;
		query(shared_ptr<query_base> q):qb(q){}
};

class binaryQuery:public query_base{
	protected:
		binaryQuery(const query& l,const query& r,
				string op):lhs(l),rhs(r),op(op){}
		virtual string rep() const override{
			return lhs.rep() +" "+op+" "+rhs.rep();
		}
		query lhs,rhs;
		string op;

};





class notQuery:public query_base{
	friend query operator~(const query& other);
	public:
		virtual queryResult eval(const textQuery& tq) const override{
			queryResult qr = q.eval(tq);
			auto file = qr.getFile();
			auto sz = file->size();
			unordered_set<int> seen(qr.begin(),qr.end());
			auto unseen = make_shared<vector<int>>();
			for(int i=0;i<sz;i++)
			{
				if(seen.count(i))	
					continue;
				unseen->push_back(i);
			}
			return queryResult(rep(),unseen,file);
		}
		virtual string rep() const override{
			return "~"+q.rep();
		}
	private:
		notQuery(const query& other):q(other){};
		query q;

};

query operator~(const query& other){
	return shared_ptr<query_base>(new notQuery(other));
}

class orQuery:public binaryQuery{
	friend query operator|(const query& lhs,const query& rhs);
	public:
		virtual queryResult eval(const textQuery& tq) const{
			queryResult l = lhs.eval(tq);
			queryResult r = rhs.eval(tq);
			auto l_ptr = l.begin();
			auto r_ptr = r.begin();
			auto merged = make_shared<vector<int>>();
			while(l_ptr!=l.end() && r_ptr!=r.end())
			{
				if(*l_ptr < *r_ptr)
				{
					merged->push_back(*l_ptr);
					l_ptr++;
				}
				else if(*l_ptr > *r_ptr)
				{
					merged->push_back(*r_ptr);
					r_ptr++;
				}
				else{
					merged->push_back(*l_ptr);
					l_ptr++;
					r_ptr++;
				}
			}
			while(l_ptr!=l.end())
			{
				merged->push_back(*l_ptr);
				l_ptr++;
			}
			while(r_ptr!=r.end())
			{
				merged->push_back(*r_ptr);
				r_ptr++;
			}
			return queryResult(rep(),merged,l.getFile());
		}	
	private:
		orQuery(const query& lhs,const query& rhs):binaryQuery(lhs,rhs,"|") {}
};
query operator|(const query& lhs,const query& rhs){
	return shared_ptr<query_base>(new orQuery(lhs,rhs));
}

class andQuery:public binaryQuery{
	friend query operator&(const query& lhs,const query& rhs);
	public:
		virtual queryResult eval(const textQuery& tq) const{
			queryResult l = lhs.eval(tq);
			queryResult r = rhs.eval(tq);
			auto merged = make_shared<vector<int>>();
			auto l_ptr = l.begin();
			auto r_ptr = r.begin();
			while(l_ptr!=l.end() && r_ptr!=r.end())
			{
				if(*l_ptr < *r_ptr)
					l_ptr++;
				else if(*l_ptr > *r_ptr)
					r_ptr++;
				else{
					merged->push_back(*l_ptr);
					l_ptr++;
					r_ptr++;
				}
			}
			return queryResult(rep(),merged,l.getFile());
		}
	private:
		andQuery(const query& lhs,const query& rhs):binaryQuery(lhs,rhs,"&"){}
};

query operator&(const query& lhs,const query& rhs){
	return shared_ptr<query_base>(new andQuery(lhs,rhs)); 
}
