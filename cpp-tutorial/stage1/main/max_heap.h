class max_heap {
	public:
		max_heap(int n):data(n),sz(0){}
		max_heap(const initializer_list<int>& il);
		int top()
		{
			if(empty())	
				throw out_of_range("empty heap!");			
			return data[0];
		}
		void pop()
		{
			if(empty())	
				throw out_of_range("empty heap");
			data[0] = data[sz-1];
			sz--;
			bubble_down(0);
		}
		void push(int elem)
		{
			if(full())	
				throw out_of_range("full heap");
			data[sz++] = elem;
			bubble_up();
		}
		bool full()
		{
			return sz==data.size();	
		}
		bool empty() 
		{
			return sz==0;	
		}
	private:
		vector<int> data;
		int sz;
		void bubble_down(int cur_id);
		void bubble_up();
		int get_parent(int i)
		{
			if(i==0)	
				return -1;
			return (i-1)/2;
		}
};

max_heap::max_heap(const initializer_list<int>& il)
{
	for(int i=0;i<il.size();i++)
		data.push_back(il[i]);	
	sz = data.size();
	for(int i=(sz-2)/2;i>=0;i--)
		bubble_down(i);
}
void max_heap::buble_up()
{
	int cur_id = sz-1;
	while(cur_id >0)	
	{
		int parent_id = get_parent(cur_id);
		if(data[parent_id] < data[cur_id])
		{
			swap(data[parent_id],data[cur_id]);	
			cur_id = parent_id;
		}
		break;
	}
	return;
}

void max_heap::bubble_down(int cur_id)
{
	int max_id = cur_id;
	int young_child = cur_id*2+1;
	for(int i=0;i<2;i++)
	{
		if(young_child+i < sz && data[max_id] < data[young_child+i])	
			max_id = young_child+i;
	}
	if(cur_id!=max_id)
	{
		swap(data[max_id],data[cur_id]);
		bubble_down(max_id);
	}
	return;	
}
