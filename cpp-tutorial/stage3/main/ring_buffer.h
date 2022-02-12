

class ringBuffer{
	private:
		char* read;
		char* write;
		char* water_mark;
		char* buffer;
		size_t cap;
		size_t bytes_per_read_write;

	public:
		ringBuffer(size_t total_sz,size_t bytes){
			buffer = read = write = new char [n];	
			cap = n;
			water_mark = buffer + cap;
			bytes_per_read_write = bytes;
		}
		//read from file  need to be fixed size every read(emulate DMA ,everytime has to fixed size)
	  	void enqueue(int read_from_fd){
			size_t actual_sz;
			if(write>=read){
				if(write+bytes_per_read_write< buffer+cap)	
				{
					actual_sz =read(read_from_fd,write,bytes_per_read_write);
					write += actual_sz;
				}
				else{
					water_mark = write;	
					if(buffer+bytes_per_read_write< read)
					{
						actual_sz =read(read_from_fd,buffer,bytes_per_read_write);	
						write += actual_sz;	
					}
				}
			}
			else{
				if(write+bytes_per_read_write < read){
					actual_sz = read(read_from_fd,write,bytes_per_read_write);	
					write += actual_sz;
				}	
			
			}
			return;
		}	
		//write to file doesn't need to be fixed size every read,this this cpu write,could write different size
		void deque(int write_to_fd){
			size_t actual_sz;
			if(write >= read)	
			{
				actual_sz = write(write_to_fd,read,min(write-read,bytes_per_read_write));	
				read += actual_sz;
			}
			else{
				if(read+bytes_per_read_write<water_mark)
				{
					actual_sz = write(write_to_fd,read,bytes_per_read_write);	
					read += actual_sz;
				}
				else{
					actual_sz = write(write_to_fd,read,water_mark-read);	
					read = buffer;
				}
			}
		}
		~ringBuffer(){
			delete [] buffer;
		}


};
