#include <functional>
#include <queue>
#include <vector>
#include <chrono>
#include <unordered_set>
#include <mutex>
#include <condition_variable>
#include <thread>
class Task{
	private:
		long startTime;
		long taskID;
		std::function<void()> task;
	public:
		Task(const std::function<void()>& t,long task_id):task(t),taskID(task_id){}
		void run(){
			task();
		}
		void setStartTime(long time) {
			startTime = time;
			return;
		}
		long getStartTime() const{
			return startTime;
		}
		long getTaskID() const{
			return taskID;
		}

};

class TaskScheduler{
	private:
		struct mycomp{
			bool operator()(const Task& lhs,const Task& rhs){
				return lhs.getStartTime() > 
				       rhs.getStartTime();
			}
		};
		std::priority_queue<Task,std::vector<Task>,mycomp> pq;
		std::unordered_set<long> task_ids;
		std::unordered_set<long> cancelled_task_ids;
		std::mutex mtx;
		std::condition_variable cv;
		void run_background_deamon(){
			while(true)
			{
				std::unique_lock<std::mutex> lck(mtx);
				while(pq.empty())
					cv.wait(lck);
				long now = get_current_time();
				Task task = pq.top();
				if(task.getStartTime()<=now)
				{
					pq.pop();
					if(!cancelled_task_ids.count(task.getTaskID()))
						std::thread(task.run());
					else{
						cancelled_task_ids.erase(task.getTaskID());
					}
				}
				else{
					cv.wait_for(lck,pq.top().getStartTime()-now);
				}
				
			}
		}
	public:
		TaskScheduler(){
			std::thread(run_background_deamon);
		}
		void schedule(std::function<void()> cmd,long delay){
			long startTime = delay + std::chrono::system_clock::now().time_since_epoch.count();
			long task_id;
			do{
				task_id = rand();
			}while(task_ids.count(task_id));
			task_ids.insert(task_id);
			Task task(cmd,task_id);	
			task.setStartTime(startTime);
			std::unique_lock<std::mutex> lck(mtx);
			pq.push(task);
			cv.notify_all();
		}
		void cancelTask(long taskID)
		{
			std::unique_lock<std::mutex> lck(mtx);
			cancelled_task_ids.insert(taskID);
			return;
		}
};