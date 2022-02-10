#include <list>

class Subscriber{
	public:
		virtual ~Subscriber(){}
		virtual void update(void* what) = 0;
};

class Publisher{
	public:
		Publisher()=default;
		virtual ~Publisher(){}
		void registerSubscriber(Subscriber* sub) {
			subscribers.push_back(sub);	
			return;
		}
		void unregisterSubscriber(Subscriber* sub)
		{
			subscribers.remove(sub);	
			return;
		}
		void notify(void* what)
		{
			for(auto it=subscribers.begin();it!=subscribers.end();it++)	
			{
				(*it)->update(what);
			}
			return;
		}
	private:
		std::list<Subscriber*> subscribers;
};
