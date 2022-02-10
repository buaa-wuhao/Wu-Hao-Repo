
class mySingleton{
	private:
		static mySingleton* singleton_;
	protected:
		mySingleton()=default;
	public:
		static	mySingleton* instance();
		mySingleton(const mySingleton& other) = delete;
		mySingleton& operator=(const mySingleton& other) = delete;
};

mySingleton* mySingleton::singleton_ = nullptr;
mySingleton* mySingleton::instance()
{
	if(singleton_==nullptr)
	{
		singleton_ = new mySingleton();
	}
	return singleton_;
}

