#include <new>
#include <assert.h>
#include <iostream>
class my_allocator{
	public:
		my_allocator(size_t numsOfBlks,size_t blkSz):blk_nums(numsOfBlks),blk_sz(blkSz) {
			blk_head = new char[blk_nums*blkSz];
			blk_list_head = nullptr;
			blk_used = 0;
		}

		void* allocate(size_t sz) {
			assert(sz<=blk_sz);
			std::cout<< "before pop()"<<std::endl;
			void* rtn_ptr = pop();
			std::cout<< "after pop()"<<std::endl;
			if(rtn_ptr==nullptr)
			{
				if(blk_used<blk_nums){
					rtn_ptr = (void*)(blk_head+blk_used*blk_sz);	
					blk_used++;
					std::cout<< "blk_used<blk_nums"<<std::endl;
				}
				else{
					std::new_handler handler = std::set_new_handler(0);
					std::set_new_handler(handler);	
					std::cout<< "new_handler"<<std::endl;
					if(handler){
						(*handler)();	
					}
					else{
						assert(0);	
					}
				
				}
			}

			return (void*)rtn_ptr;
		}
		void free(void* ptr){
			push(ptr);
			return;
		}
		~my_allocator(){
			delete [] blk_head;	
		}

	private:
		struct  Block{
			Block* next;
		};
		void push(void* blk_ptr){
			if(blk_ptr==nullptr) return;
			Block* ptr = (Block*) blk_ptr;
			ptr->next = blk_list_head;
			blk_list_head = ptr;	
		}
		Block* pop(){
			Block* ptr = blk_list_head;
			if(blk_list_head)
			{
				blk_list_head = blk_list_head->next;	
				ptr->next = nullptr;
			}
			return ptr; 
		}

		size_t blk_nums;
		size_t blk_sz;
		size_t blk_used;

		char* blk_head;
		Block* blk_list_head;




};
