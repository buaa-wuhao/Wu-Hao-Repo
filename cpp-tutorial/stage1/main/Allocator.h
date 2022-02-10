#ifndef ALLOCATOR_H_
#define ALLOCATOR_H_
#include <new>
#include <assert.h>
class Allocator{

	public:
		Allocator(size_t blockSZ,size_t numOfBlocks):blockSZ_(blockSZ),
							     numOfBlocks_(numOfBlocks),
							     allocatedBlocks_(0),
							     freeList_(nullptr) 
		{
			blockHead_ = (char*)new char[blockSZ*numOfBlocks];	
		}
		~Allocator(){
			delete [] blockHead_;	
		}
		void* malloc(size_t sz)
		{
			assert(sz <= blockSZ_);
			void* pBlock = 	Pop();
			if(!pBlock)
			{
				if(allocatedBlocks_ < numOfBlocks_)
				{
					pBlock = (void*) (blockHead_ + allocatedBlocks_*blockSZ_); 	
					allocatedBlocks_++;
				}
				else
				{
					std::new_handler handler = std::set_new_handler(0);
					std::set_new_handler(handler);
					(*handler)();
				}
			}
			return pBlock;

		}
		void free(void* pBlock)
		{
			push(pBlock);		
			return;
		}
	private:
		struct Block{
			Block* next;	
		};
		void push(void* ptr)
		{
			Block* pBlock = (Block*) ptr;
			pBlock->next = freeList_;
			freeList_ = pBlock;	
		}
		void* Pop()
		{
			Block* pBlock = nullptr;
			if(freeList_)
			{
				pBlock = freeList_;
				freeList_ = freeList_->next;	
			}
			return (void*)pBlock;
		}
		char*   blockHead_;
		size_t  blockSZ_;
		size_t  numOfBlocks_; 
		size_t  allocatedBlocks_;
		Block*  freeList_;


};



#endif//ALLOCATOR_H_
