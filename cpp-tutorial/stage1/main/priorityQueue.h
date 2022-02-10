#include <vector>
#include <initializer_list>

template<typename T,typename U=less<T>>
class priorityQueue{
	public:
		priorityQueue():comp(U()){}
		priorityQueue(std::initializer_list<T>& il):comp(U()) {
			for(auto& it: il)				
				data.push_back(it);
			heapify();
		}
		//return T or T&??
		const T& top() const
		{
			if(data.empty())
				throw std::out_of_range("Empty queue!");	
			return data[0];	
		}
		//should we add const in the end?
		size_t size() const
		{
			return data.size();	
		}
		bool empty() const			
		{
			return data.empty();	
		}
		void pop(){
			if(data.empty())			
				throw std::out_of_range("Empty queue!");		
			data[0] = data.back();
			data.pop_back();
			bubble_down(0);
		}
		void push(const T& elem){
			int idx = data.size();
			data.push_back(elem);
			bubble_up(idx);
			return;
		}
	private:
		void heapify(){
			int idx = data.size()-1;
			int parent_idx = parent(idx);
			for(int i=parent_idx;i>=0;i--)		
				bubble_down(i);	
			return;
		}
		int parent(int i){
			if(i==0)	
				return -1;
			else
				return (i-1)/2;
		}
		int child(int i){
			int kid = 2*i+1;
			if(kid>=data.size())	
				return -1;
			else
				return kid;
		}
	 	void bubble_up(int idx){
			int parent_idx = parent(idx);
			//always compare(lower_element, top_element) in binary tree
			while(parent_idx!=-1 && !comp(data[idx],data[parent_idx]))
			{
				std::swap(data[idx],data[parent_idx]);		
				idx = parent_idx;
				parent_idx = parent(idx);
			}
		}	
		void bubble_down(int idx){
			int tgt_idx = idx;
			int child_idx = idx*2+1;
			for(int i=0;i<2 && child_idx<data.size();i++)	
			{
				child_idx += i;
				//always compare(lower_element, top_element) in binary tree
				if(!comp(data[child_idx],data[tgt_idx]))		
					tgt_idx = child_idx;
			}
			if(tgt_idx!=idx)
			{
				std::swap(data[idx],data[tgt_idx]);
				bubble_down(tgt_idx);	
			}
			return;
		}
		vector<T> data;
		U comp;
};
