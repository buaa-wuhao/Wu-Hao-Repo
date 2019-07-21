#include<string>
#include<iostream>
#include<vector>

using namespace std;
class Screen;
class windowMgr{
	
	private:
		vector<Screen> window;
	
	public:
		void clear(int idx);
		windowMgr();

};

class Screen
{
	friend void windowMgr::clear(int idx);
	public:
		using pos = string::size_type;
	Screen() = default;
	Screen(pos ht,pos wdth,char c):height(ht),width(wdth),content(ht*wdth,c) {}
	Screen(pos ht,pos wdth):height(ht),width(wdth),content(ht*wdth,' '){};
	char get() const {return content[cur];}
	inline char get(pos r,pos c) const;
	Screen move(pos r,pos c); 
	Screen set(char c) 
	{
		content[cur] = c;
		return (*this);
	}
	Screen display(ostream& os)
	{
		os << content;
		return (*this);
	} 

	private:
		pos cur = 0;
		pos height = 0,width = 0;
		string content;
};

windowMgr::windowMgr()
{
	window.push_back(Screen(24,80,' '));
}

void windowMgr::clear(int idx)
{
	if(idx>=window.size()) return;
	Screen& s = window[idx]; 
	s.content = string(s.height * s.width,' ');
}

Screen Screen::move(pos r,pos c)
{
	cur = r*width + c;
	return (*this);
}

char Screen::get(pos r,pos c) const
{
	return content[r*width+c];
}

int main()
{
	Screen myScreen(5,5,'X');
	myScreen.move(4,0).set('#').display(cout);
	cout << "\n";
	myScreen.display(cout);
	cout << "\n";
	return 0;
}