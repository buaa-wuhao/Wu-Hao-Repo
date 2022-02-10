#include <assert.h>
#include <new>

class Allocator{
	public:
		Allocator(size_t blockSZ, size_t numOfBlocks):blockUnitSZ(blockSZ),
					totalBlocks(numOfBLocks),numOfAllocatedBlocks(0),freeList(nullptr){
			ptrHead = (char*) new char[blockSZ*numOfBlocks]; 				
		}
		Allocator(const Allocator& rhs) = delete;
		Allocator& operator=(const Allocator& rhs) = delete;
		//does myAllocate have input parameter? like memory size needed to be allocated?
		void* myAllocate(size_t sz){
			assert(sz<=blockUnitSZ);
			void* assignedBlk = pop();
			if(!assignedBlk)
			{
				if(numOfAllocatedBlocks < totalBlocks)		
				{
					assignedBlk = (void*)(ptrHead + numOfAllocatedBlocks*blockUnitSZ);			
					numOfAllocatedBlocks++;
				}
				else
				{
					std::new_handler handle = std::set_new_handler(0);
					std::set_new_handler(handle);
					if(handle)					
						(*handle)();
					else
						assert(0);
				}
			}
			return (void*)assignedBlk;
		}

		void myFree(void* ptr)
		{
			push((Block*)ptr);			
			return;
		}

		~Allocator() {
			delete [] ptrHead;	
		}
	private:
		void* pop(){
			Block* rtnBlk==nullptr;
			if(freeList)	
			{
				Block* next = freeList->next;
				rtnBlk = freeList;
				freeList = next;	
			}
			return (void*)rtnBlk;
		}
		void push(Block* pBlock){
			if(pBlock==nullptr)	
				return;
			pBlock->next = freeList;
			freeList = pBlock;
			return;
		}
		struct Block{
			Block* next;
		};
		size_t blockUnitSZ;
		size_t totalBlocks;
		size_t numOfAllocatedBlocks;
		Block* freeList;
		//why it is char* not void*?
		char*  ptrHead;


};
