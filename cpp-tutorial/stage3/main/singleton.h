#include <mutex>
template<typename T>
class singleton{
	public:
		singleton(const singleton&) = delete;
		singleton(singleton&&) = delete;
		singleton& operator=(const singleton&) = delete;
		singleton& operator=(singleton&&) = delete;
		static T* instance();
		static std::mutex mtx;
	private:
		static T* ptr_T;
		singleton()=default;

};

template<typename T>
T* singleton<T>::ptr_T = nullptr;

template<typename T>
std::mutex singleton<T>::mtx;

template<typename T>
T* singleton<T>::instance(){
	std::unique_lock<std::mutex> lck(mtx);
	if(ptr_T==nullptr)
	{
		ptr_T = new T();
	}
	return ptr_T;
}
