#include <iostream>

class shape{
	public:
	virtual void draw() const = 0;
	virtual void error(const std::string& msg)
	{
		std::cout<<msg<<std::endl;	
	}
	int objectID() const{
		return 0;	
	}
};
void shape::draw() const {
	std::cout<<"shape::draw()"<< std::endl;
}
class square:public shape{
	public:
	void draw() const override
	{
		std::cout<<"square::draw " <<  std::endl;
		shape::draw();
	}
	
};
