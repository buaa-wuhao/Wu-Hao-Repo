#include <functional>
#include <unordered_set>
#include <queue>
#include <chrono>
#include <stdlib.h>
#include <mutex>
#include <condition_variable>
using namespace std;

struct Task{
	int task_id;
	function<void()> cmd;
	Task(int id,function<void()>& command):task_id(id),cmd(command){}
};

class taskScheduler{
	private:
		struct mycomp{
			bool operator()(const pair<long,Task>& lhs,
					const pair<long,Task>& rhs){
				return lhs.first > rhs.first;
			}	
		};
		unordered_set<int> running_task_id;
		priority_queue<pair<long,Task>,vector<pair<long,Task>>,mycomp> pq;
	  	mutex mtx;	
		condition_variable cv;
		thread run_background;	
		bool is_alive;

		void backgroundTask(){
				unique_lock<mutex> lck(mtx);
				while(true){
					while(is_alive && pq.empty()){
						cv.wait(lck);	
					}
					if(!is_alive)
						return;
					auto top = pq.top();
					long cur_time = cur_time_count_since_epoch();
					if(top.first <= cur_time)
					{
						pq.pop();
						thread run_cmd(top.second.cmd);	
					}
					else{
						long off_set = top.first-cur_time;
						cv.wait_for(lck,chrono::seconds(off_set));	
					}
				}
		}
		long cur_time_count_since_epoch(){
			auto now = chrono::system_clock::now();	
			auto now_s = chrono::time_point_cast<chrono::seconds>(now);
			auto val = now_s.time_since_epoch().count();
			return val;	
		}
		void stop(){
			unique_lock<mutex> lck(mtx);
			is_alive = false;	
			cv.notify_all();
		}
	public:
		taskScheduler(){
			run_background = thread(&taskScheduler::backgroundTask,this);	
			is_alive = true;
		}
		~taskScheduler(){
			stop();
			run_background.join();	
		}
		void addTask(function<void()>& cmd,long time_exec_offset){
			unique_lock<mutex> lck(mtx);
			int task_id;
			do{
				task_id = rand();
			}while(running_task_id.count(task_id));

			running_task_id.insert(task_id);

			Task task(task_id,cmd);

			auto val = cur_time_count_since_epoch();
		  	pq.push({val+time_exec_offset ,task});	
			cv.notify_all();
			return;
		}
};
