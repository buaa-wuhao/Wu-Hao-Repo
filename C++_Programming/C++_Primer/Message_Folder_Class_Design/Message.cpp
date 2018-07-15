class Message{
friend class Folder;
public:
	Message(string s=""):content(s) {}
	Message(const Message& msg);
	Message& operator=(Message& msg);
	~Message();

	void save(Folder& folder);
	void remove(Folder& folder);
	void insert(Folder*);
	void rm_folder(Folder*);

private:
	string        content;
	set<Folder*>  folder;
};

Message& Message::operator=(Message& msg)
{
	content = msg.content;
	for(auto& fd:folder)
	{
		fd->remove_msg(this);
	}
	folder = msg.folder;
	for(auto& fd:folder)
	{
		fd->save(this);
	}
	return (*this);
}

Message::Message(const Message& msg)
{
	content = msg.content;
	folder  = msg.folder;
	for(auto& fd:folder)
	{
		fd->save(this);
	}
}

Message::~Message()
{
	for(auto& fd: folder)
	{
		fd->remove_msg(this);
	}
}

void Message::save(Folder& folder)
{
	this->folder.insert(&folder);
	folder.save_msg(this);
	return;
}

void Message::remove(Folder& folder)
{
	this->folder.erase(&folder);
	folder.remove_msg(this);
	return;
}

void Message::insert(Folder* f)
{
	folder.insert(f);
	return;
}

void Message::rm_folder(Folder* f)
{
	folder.erase(f);
	return;
}

