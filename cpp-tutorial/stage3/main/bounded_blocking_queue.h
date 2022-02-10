#include <vector>
#include <mutex>
#include <condition_variable>
#include <unistd.h>
#include <atomic>
//https://andrea.lattuada.me/blog/2019/the-design-and-implementation-of-a-lock-free-ring-buffer-with-contiguous-reservations.html
//ssize_t read(int fd, void *buf, size_t count)
//ssize_t write(int fd, const void *buf, size_t count);
class BoundedBlockingQueue{
	private:
		std::atomic<char*> read_ptr;
		std::atomic<char*> write_ptr;
		std::atomic<char*> water_mark;
		char* buffer;
		size_t total_sz;
		size_t sz_per_read;

		std::mutex mtx_begin,mtx_end;
	public:
		BoundedBlockingQueue(size_t buffer_sz,size_t unitSZ):total_sz(buffer_sz),sz_per_read(unitSZ) {
			buffer = new char[total_sz];
			//read_ptr = buffer;
			read_ptr.store(buffer);
			write_ptr.store(buffer);
		}
		void enqueue(int fd){
			char* write_ptr_copy = write_ptr.load();
			char* read_ptr_copy = read_ptr.load();
			if(write_ptr_copy >= read_ptr_copy){
				if(total_sz+buffer-write_ptr_copy>=sz_per_read)	
				{
					size_t actual_read_bytes = read(fd,write_ptr_copy,sz_per_read);
					write_ptr.store(write_ptr_copy+actual_read_bytes);
				}
				else{
					water_mark.store(write_ptr_copy);
					if(read_ptr_copy - buffer >= sz_per_read)
					{
						//write_ptr = buffer;
						size_t actual_read_bytes = read(fd,buffer,sz_per_read);
						write_ptr.store(buffer+ actual_read_bytes);
					}
				}
			}
			else{
				if(write_ptr_copy+sz_per_read <= read_ptr_copy)
				{
					size_t actual_read_bytes = read(fd,write_ptr_copy,sz_per_read);
					write_ptr.store(write_ptr_copy+sz_per_read);
				}
			}
		}
		void dequeue(int fd){
			char* write_ptr_copy = write_ptr.load();
			char* read_ptr_copy = read_ptr.load();
			size_t actual_write_bytes;
			if(read_ptr_copy<=write_ptr_copy)
			{
				if(read_ptr_copy+sz_per_read <= write_ptr_copy)
				{
					actual_write_bytes = write(fd,read_ptr_copy,sz_per_read);
					read_ptr.store(read_ptr_copy + actual_write_bytes);	
				}
			}
			else{
				if(read_ptr_copy+sz_per_read<=water_mark.load())
				{
					actual_write_bytes = write(fd,read_ptr_copy,sz_per_read);
					read_ptr.store(read_ptr_copy + actual_write_bytes);
				}
				else{
					size_t already_write = water_mark.load() - read_ptr_copy;
					write(fd,read_ptr_copy,already_write);
					if(write_ptr_copy-buffer>=(sz_per_read-already_write))
					{
						actual_write_bytes = write(fd,buffer,sz_per_read-already_write);
						read_ptr.store( buffer + actual_write_bytes);
					}
				}
			}
			return;
		}
};