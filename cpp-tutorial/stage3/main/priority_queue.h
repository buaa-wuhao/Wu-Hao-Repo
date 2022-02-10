#include <vector>
#include <initializer_list>
#include <functional>

template<typename T,typename U=std::less<T>>
class priorityQueue{
	public:
		priorityQueue(const std::initializer_list<T>& lst):comp(U()){
			for(auto& it:lst)	
				data.push_back(it);
			heapify();
		}
		T top() const{
			if(empty())
				throw std::out_of_range("Empty!");
			return data[0];	
		}
		void push_back(const T& elem){
			data.push_back(elem);
			auto sz  = data.size();
			bubble_up(sz-1);
			return;	
		}
		void pop(){
			if(empty())	
				throw std::out_of_range("Empty");
			data[0] = data.back();
			data.pop_back();
			bubble_down(0);
			return;
		}
		bool empty() const{
			return data.empty();	
		}
		size_t size() const{
			return data.size();	
		}
	private:
		std::vector<T> data;
		U comp;
		void heapify(){
			size_t sz = data.size();
			for(int i=parent(sz-1);i>=0;i--)	
				bubble_down(i);	
			return;
		}
		void bubble_up(int idx){
			while(parent(idx)!=-1)	
			{
				if(comp(data[parent(idx)],data[idx]))	
				{
					std::swap(data[parent(idx)],data[idx]);
					idx = parent(idx);
					continue;
				}
				break;
			}
			return;
		}
		void bubble_down(int idx){
			auto sz = data.size();
			while(young_child(idx)<sz){
				int young = young_child(idx);
				int tgt_idx = idx;
				for(int i=0;i<2;i++)
				{
					if(young+i<sz && !comp(data[young+i],data[tgt_idx]))	
						tgt_idx = young+i;
				}
				if(tgt_idx==idx)
					break;
				std::swap(data[tgt_idx],data[idx]);
				idx = tgt_idx;
			}	
			return;
		}
		int young_child(int i){
			return i*2+1;	
		}
		int parent(int i){
			if(i==0)	
				return -1;
			return (i-1)/2;
		}
};
