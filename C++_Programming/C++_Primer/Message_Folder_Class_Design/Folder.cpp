class Folder
{
	private:
		set<Message*> msgs;
	public:
		Folder();
		~Folder();
		void save_msg(Message*);
		void remove_msg(Message*);
};

void Folder::save_msg(Message* msg);
{
	msgs.insert(msg);
	return;
}

void Folder::remove_msg(Message* msg)
{
	msgs.erase(msg);
	return;
}