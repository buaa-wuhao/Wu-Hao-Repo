#include <string>
#include <set>
using namespace std;

class Folder;
class Message{
	public:
		Message(const string& s=""):content(s) {}
		Message(const Message& msg);
		void operator=(const Message& msg);
		~Message();
		void save(Folder* folder);
		void remove(Folder* folder);
		void insert(Folder* folder);
		void rmfolder(Folder* folder);
	private:
		string       content;
		set<Folder*> folders;
		void addToFolder(set<Folder*>);
		void rmvFrmFolder();

};

class Folder
{
	public:
		//Folder();
		void addMsg(Message* msg);
		void rmvMsg(Message* rmv);
	private:
		set<Message*> messages;
};

void Message::insert(Folder* folder)
{
	folders.insert(folder);
}

void Message::rmfolder(Folder* folder)
{
	folders.erase(folder);
}


void Folder::addMsg(Message* msg)
{
	messages.insert(msg);
}

void Folder::rmvMsg(Message* msg)
{	
	messages.erase(msg);
}

void Message::rmvFrmFolder()
{
	for(auto it:folders)
		it->rmvMsg(this);
	return;
}

void Message::addToFolder(set<Folder*> folders)
{
	for(auto it:folders)
		it->addMsg(this);
	return;
}

Message::~Message()
{
	rmvFrmFolder();
}

Message::Message(const Message& msg)
{
	content = msg.content;
	folders = msg.folders;
	addToFolder(msg.folders);
}

void Message::operator=(const Message& msg)
{
	content = msg.content;
	rmvFrmFolder();
	addToFolder(msg.folders);
	folders = msg.folders;
	//return (*this);
}

void Message::save(Folder* folder)
{
	folders.insert(folder);
	folder->addMsg(this);
	return;
}

void Message::remove(Folder* folder)
{
	folders.erase(folder);
	folder->rmvMsg(this);
	return;
}

int main()
{
	Message msg("test");
	Folder  folder;
	return 0;
}