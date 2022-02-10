#include <new>
#include <assert>

class Allocator{
	public:
		Allocator(size_t blkSZ,size_t numOfBlk):blk_sz(blkSZ),num_of_blk(numOfBlk){
			blk_head_ptr = new char [blk_sz*num_of_blk] ;
			list_head = nullptr;
			allocatedNumOfBlks = 0;
		}
		void* allocate(size_t sz){
			assert(sz > blk_sz);
			void* allocated_ptr = Pop();
			if(allocated_ptr)	
				return allocated_ptr;
			if(allocatedNumOfBlks < num_of_blk)	
			{
				allocated_ptr = (void*)(blk_head_ptr + 
						allocatedNumOfBlks*blk_sz);
				allocatedNumOfBlks++;
			}
			else
			{
				std::new_handler handler = std::set_new_handler(0);
				std::set_new_handler(handler);	
				if(handle)
					(*handler)();	
				else
					assert(0);
			}
			return allocated_ptr;
		}

		void free(void* ptr){
			Push(ptr);
			return;
		}
		~Allocator(){
			delete []  blk_head_ptr;
		}
	private:
		struct Block{
			Block* next;	
		};
		void Push(void* ptr)
		{
			if(ptr==nullptr)	
				return;
			Block* b_ptr = (Block*) ptr;
			b_ptr->next = list_head;
			list_head = b_ptr;
			return;
		}
		void* Pop()
		{
			void* rtn_ptr = nullptr;
			if(list_head)
			{
				Block* next = list_head->next;
				rtn_ptr = (void*) list_head;
				list_head = next;
			}
			return rtn_ptr;
		}


		char*   blk_head_ptr;
		size_t  blk_sz;
		size_t  num_of_blk;
		Block*  list_head;
		size_t  allocatedNumOfBlks;



};
