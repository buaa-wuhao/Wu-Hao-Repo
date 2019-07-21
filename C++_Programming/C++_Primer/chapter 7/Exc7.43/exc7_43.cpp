class noDefault
{
	public:
	noDefault(int i){ data = i;}

	private:
	int data;
};

class C
{
public:
	C():nd(0) {};
private:
	noDefault nd;
}